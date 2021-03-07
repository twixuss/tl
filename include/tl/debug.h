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

TL_API bool is_debugger_attached();

}

#ifdef TL_IMPL

#if OS_WINDOWS

#pragma warning(push, 0)
#include <DbgHelp.h>
#include <debugapi.h>
#pragma warning(pop)

#pragma comment(lib, "dbghelp")

namespace TL {

HANDLE debug_process; 

bool debug_init() {
	debug_process = GetCurrentProcess();
	if (!SymInitialize(debug_process, 0, true))
		return false;
	
	DWORD options = SymGetOptions();
	SymSetOptions(options & ~SYMOPT_UNDNAME | SYMOPT_PUBLICS_ONLY);

	return true;
}

void debug_deinit() {
	SymCleanup(debug_process);
}


StackTrace get_stack_trace() {
#ifdef TL_TRACK_ALLOCATIONS
	track_allocations = false;
	defer { track_allocations = true; };
#endif

	StackTrace stack_trace;

	STACKFRAME64 frame = {};
 
	CONTEXT context = {};
	context.ContextFlags = CONTEXT_ALL;
	RtlCaptureContext(&context);

	frame.AddrPC.Offset    = context.Rip;
	frame.AddrStack.Offset = context.Rsp;
	frame.AddrFrame.Offset = context.Rbp;
	frame.AddrPC.Mode      = AddrModeFlat;
	frame.AddrStack.Mode   = AddrModeFlat;
	frame.AddrFrame.Mode   = AddrModeFlat;
 
	auto thread = GetCurrentThread();

	u32 frameIndex = 0;
	while (StackWalk64(IMAGE_FILE_MACHINE_AMD64, debug_process, thread, &frame, &context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {
		if (!frameIndex++)
			continue;
		constexpr u32 maxNameLength = MAX_SYM_NAME;
		DWORD64 displacement;

		static u8 buffer[sizeof(SYMBOL_INFO) + maxNameLength * sizeof(char)] = {};
		PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = maxNameLength;
		
		if (!SymFromAddr(debug_process, frame.AddrPC.Offset, &displacement, symbol)) {
			break;
		}

		CallStackEntry entry;

		char name[256];
		auto name_length = UnDecorateSymbolName(symbol->Name, name, (DWORD)count_of(name), UNDNAME_NAME_ONLY);
		if (name_length) {
			entry.name.data = (char *)stack_trace.string_buffer.size;
			entry.name.size = name_length;

			stack_trace.string_buffer += Span(name, name_length);
		}

		IMAGEHLP_LINE64 line = {};
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
		DWORD line_displacement;

		char const *file;
		if (SymGetLineFromAddr64(debug_process, symbol->Address, &line_displacement, &line)) {
			entry.line = line.LineNumber;
			file = line.FileName;
		} else {
			file = "unknown";
		}
		auto file_length = length(file);

		entry.file.data = (char *)stack_trace.string_buffer.size;
		entry.file.size = file_length;

		stack_trace.string_buffer += Span(file, file_length);

		stack_trace.call_stack.add(entry);
	}

	for (auto &call : stack_trace.call_stack) {
		call.name.data = stack_trace.string_buffer.data + (umm)call.name.data;
		call.file.data = stack_trace.string_buffer.data + (umm)call.file.data;
	}
 
	return stack_trace;
}

void free(StackTrace &stack_trace) {
	free(stack_trace.string_buffer);
}


bool is_debugger_attached() {
	return IsDebuggerPresent();
}

}

#endif

#endif

#pragma warning(pop)
