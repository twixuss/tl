#pragma once
#include "list.h"
#include "string.h"

#pragma warning(push)
#pragma warning(disable: 4820)

namespace TL {

struct CallStackEntry {
	Span<char> name;
	Span<char> file;
	u32 line = 0;
};

struct StackTrace {
	List<CallStackEntry> call_stack;
	List<char> string_buffer;
};

TL_API bool debug_init();
TL_API void debug_deinit();

TL_API StackTrace get_stack_trace();
TL_API void free(StackTrace &stack_trace);

TL_API bool debugger_attached();

}

#ifdef TL_IMPL

#if OS_WINDOWS

#pragma warning(push, 0)
#include <DbgHelp.h>
#include <debugapi.h>
#pragma warning(pop)

#pragma comment(lib, "dbghelp")

namespace TL {

static HANDLE debug_process;

struct ModuleInfo {
    List<char> image_name;
    List<char> module_name;
    void *base_address;
    DWORD load_size;
};

List<ModuleInfo> loaded_modules;

bool debug_init() {
	debug_process = GetCurrentProcess();
	if (!SymInitialize(debug_process, 0, true))
		return false;

	DWORD options = SymGetOptions();
	//SymSetOptions((options & ~SYMOPT_UNDNAME) | SYMOPT_PUBLICS_ONLY | SYMOPT_LOAD_LINES | SYMOPT_FAIL_CRITICAL_ERRORS);
	SymSetOptions((options & ~SYMOPT_UNDNAME) | SYMOPT_PUBLICS_ONLY | SYMOPT_DEBUG | SYMOPT_EXACT_SYMBOLS);

	return true;
}

void debug_deinit() {
	SymCleanup(debug_process);
}

ModuleInfo get_module_info(HMODULE module, HANDLE process) {
	ModuleInfo result;
	char temp[4096];
    MODULEINFO mi;

    GetModuleInformation(process, module, &mi, sizeof(mi));
    result.base_address = mi.lpBaseOfDll;
    result.load_size = mi.SizeOfImage;

    GetModuleFileNameEx(process, module, temp, sizeof(temp));
    result.image_name = as_span(temp);

    GetModuleBaseName(process, module, temp, sizeof(temp));
    result.module_name = as_span(temp);

    SymLoadModule64(process, 0, result.image_name.data, result.module_name.data, (DWORD64)result.base_address, result.load_size);

	return result;
}

static void *load_modules_symbols(HANDLE process, DWORD pid) {
    List<ModuleInfo> modules;
	defer { free(modules); };

    DWORD cbNeeded;

    List<HMODULE> module_handles;
	defer { free(module_handles); };

	module_handles.resize(1);

    EnumProcessModules(process, &module_handles[0], module_handles.size * sizeof(HMODULE), &cbNeeded);
    module_handles.resize(cbNeeded/sizeof(HMODULE));
    EnumProcessModules(process, &module_handles[0], module_handles.size * sizeof(HMODULE), &cbNeeded);

	for (auto module : module_handles) {
		modules.add(get_module_info(module, process));
	}

    return modules[0].base_address;
}

StackTrace get_stack_trace(CONTEXT context, u32 frames_to_skip) {
#ifdef TL_TRACK_ALLOCATIONS
	track_allocations = false;
	defer { track_allocations = true; };
#endif

	STACKFRAME64 frame = {};

	frame.AddrPC.Offset    = context.Rip;
	frame.AddrStack.Offset = context.Rsp;
	frame.AddrFrame.Offset = context.Rbp;
	frame.AddrPC.Mode      = AddrModeFlat;
	frame.AddrStack.Mode   = AddrModeFlat;
	frame.AddrFrame.Mode   = AddrModeFlat;

	//List<>
	//DWORD required_size_for_modules;
	//EnumProcessModules(debug_process, modules, sizeof(HMODULE), &required_size_for_modules);

	void *base = load_modules_symbols(debug_process, GetCurrentProcessId());
    IMAGE_NT_HEADERS *h = ImageNtHeader(base);
    DWORD image_type = h->FileHeader.Machine;

	HANDLE thread;
	DuplicateHandle(debug_process, GetCurrentThread(), GetCurrentProcess(), &thread, 0, false, DUPLICATE_SAME_ACCESS);
	defer { CloseHandle(thread); };

	StackTrace stack_trace;

	while (StackWalk64(image_type, debug_process, thread, &frame, &context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {
		if (frames_to_skip) {
			--frames_to_skip;
			continue;
		}

		CallStackEntry entry;
		Span<char> file = "unknown"s;
		Span<char> name = "unknown"s;

        if (frame.AddrPC.Offset == 0) {
			file = "NULL"s;
			name = "ATTEMP TO EXECUTE AT ADDRESS 0"s;
		} else {
			constexpr u32 maxNameLength = MAX_SYM_NAME;
			DWORD64 displacement;

			static u8 buffer[sizeof(SYMBOL_INFO) + maxNameLength] = {};
			auto symbol = (SYMBOL_INFO*)buffer;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = maxNameLength;

			if (!SymFromAddr(debug_process, frame.AddrPC.Offset, &displacement, symbol)) {
				auto error = GetLastError();
				print("SymFromAddr failed with code: 0x% (%)\n", FormatInt(error, 16), error);
				break;
			}

			char name_buffer[256];

			auto name_length = UnDecorateSymbolName(symbol->Name, name_buffer, (DWORD)count_of(name_buffer), UNDNAME_COMPLETE);
			if (name_length) {
				name = Span(name_buffer, name_length);
			}

			IMAGEHLP_LINE64 line = {};
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			DWORD line_displacement;

			if (SymGetLineFromAddr64(debug_process, symbol->Address, &line_displacement, &line)) {
				entry.line = line.LineNumber;
				file = as_span(line.FileName);
			}
		}

		entry.name.data = (char *)stack_trace.string_buffer.size;
		entry.name.size = name.size;
		stack_trace.string_buffer.add(name);

		entry.file.data = (char *)stack_trace.string_buffer.size;
		entry.file.size = file.size;
		stack_trace.string_buffer.add(file);

		stack_trace.call_stack.add(entry);
	}

	for (auto &call : stack_trace.call_stack) {
		call.name.data = stack_trace.string_buffer.data + (umm)call.name.data;
		call.file.data = stack_trace.string_buffer.data + (umm)call.file.data;
	}

	return stack_trace;
}
StackTrace get_stack_trace() {
	CONTEXT context = {};
	context.ContextFlags = CONTEXT_CONTROL;
	RtlCaptureContext(&context);

	return get_stack_trace(context, 1);
}

void free(StackTrace &stack_trace) {
	free(stack_trace.string_buffer);
	free(stack_trace.call_stack);
}


bool debugger_attached() {
	return IsDebuggerPresent();
}

}

#endif

#endif

#pragma warning(pop)
