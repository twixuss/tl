#pragma once
#include "list.h"
#include "string.h"
#include "console.h"

#pragma warning(push)
#pragma warning(disable: 4820)

namespace TL {

using CallStack = List<void *>;

struct StringizedCallStack {
	struct Entry {
		Span<char> name;
		Span<char> file;
		u32 line = 0;
	};
	List<Entry> call_stack;
	List<char> string_buffer;
};

TL_API bool debug_init();
TL_API void debug_deinit();

TL_API CallStack get_call_stack();

TL_API StringizedCallStack to_string(CallStack &call_stack);
TL_API void free(StringizedCallStack &call_stack);

inline StringizedCallStack get_stack_trace() {
	auto call_stack = with(temporary_allocator, get_call_stack());
	return to_string(call_stack);
}

#ifdef CreateWindow
TL_API StringizedCallStack get_stack_trace(CONTEXT context, u32 frames_to_skip);
#endif

TL_API bool debugger_attached();

}

#ifdef TL_IMPL

#if OS_WINDOWS

#include "compiler.h"

#pragma warning(push, 0)
#include <DbgHelp.h>
#pragma warning(pop)

#pragma comment(lib, "dbghelp")

namespace TL {

static HANDLE debug_process;

bool debug_init() {
	debug_process = GetCurrentProcess();
	if (!SymInitialize(debug_process, 0, true))
		return false;

	DWORD options = SymGetOptions();
	SymSetOptions((options & ~SYMOPT_UNDNAME) | SYMOPT_PUBLICS_ONLY | SYMOPT_LOAD_LINES | SYMOPT_FAIL_CRITICAL_ERRORS);

	return true;
}

void debug_deinit() {
	SymCleanup(debug_process);
}

static CallStack get_call_stack(CONTEXT context, u32 frames_to_skip) {
	STACKFRAME64 frame = {};

	frame.AddrPC.Offset    = context.Rip;
	frame.AddrStack.Offset = context.Rsp;
	frame.AddrFrame.Offset = context.Rbp;
	frame.AddrPC.Mode      = AddrModeFlat;
	frame.AddrStack.Mode   = AddrModeFlat;
	frame.AddrFrame.Mode   = AddrModeFlat;

#if ARCH_X64
    DWORD image_type = IMAGE_FILE_MACHINE_AMD64;
#else
    DWORD image_type = IMAGE_FILE_MACHINE_I386;
#endif

	HANDLE thread;
	DuplicateHandle(debug_process, GetCurrentThread(), GetCurrentProcess(), &thread, 0, false, DUPLICATE_SAME_ACCESS);
	defer { CloseHandle(thread); };

	CallStack call_stack;
	while (StackWalk64(image_type, debug_process, thread, &frame, &context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {
		if (frames_to_skip) {
			--frames_to_skip;
			continue;
		}

		call_stack.add((void *)frame.AddrPC.Offset);
	}

	return call_stack;
}
CallStack get_call_stack() {
	CONTEXT context = {};
	context.ContextFlags = CONTEXT_CONTROL;
	RtlCaptureContext(&context);

	return get_call_stack(context, 1);
}

StringizedCallStack to_string(CallStack &call_stack) {
	StringizedCallStack result;
	for (auto &call : call_stack) {

		StringizedCallStack::Entry entry;
		Span<char> file = "unknown"s;
		Span<char> name = "unknown"s;

        if (call == 0) {
			file = "NULL"s;
			name = "ATTEMP TO EXECUTE AT ADDRESS 0"s;
		} else {
			constexpr u32 maxNameLength = MAX_SYM_NAME;
			DWORD64 displacement;

			static u8 buffer[sizeof(SYMBOL_INFO) + maxNameLength] = {};
			auto symbol = (SYMBOL_INFO*)buffer;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = maxNameLength;

			if (!SymFromAddr(debug_process, (DWORD64)call, &displacement, symbol)) {
				auto error = GetLastError();
				print("SymFromAddr failed with code: 0x% (%)\n", FormatInt(error, 16), error);
				break;
			}

#if 1
			char name_buffer[256];
			name.data = name_buffer;
			name.size = UnDecorateSymbolName(symbol->Name, name_buffer, count_of(name_buffer), UNDNAME_NO_MS_KEYWORDS | UNDNAME_NO_ACCESS_SPECIFIERS | UNDNAME_NO_CV_THISTYPE | UNDNAME_NO_FUNCTION_RETURNS | UNDNAME_NO_MEMBER_TYPE);
#else
			name = with(temporary_allocator, demangle(symbol->Name));
#endif

			IMAGEHLP_LINE64 line = {};
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			DWORD line_displacement;

			if (SymGetLineFromAddr64(debug_process, (ULONG64)call, &line_displacement, &line)) {
				entry.line = line.LineNumber;
				file = as_span(line.FileName);
			}
		}

		entry.name.data = (char *)result.string_buffer.size;
		entry.name.size = name.size;
		result.string_buffer.add(name);

		entry.file.data = (char *)result.string_buffer.size;
		entry.file.size = file.size;
		result.string_buffer.add(file);

		result.call_stack.add(entry);
	}
	for (auto &call : result.call_stack) {
		call.name.data = result.string_buffer.data + (umm)call.name.data;
		call.file.data = result.string_buffer.data + (umm)call.file.data;
	}
	return result;
}

StringizedCallStack get_stack_trace(CONTEXT context, u32 frames_to_skip) {
	auto call_stack = with(temporary_allocator, get_call_stack(context, frames_to_skip));
	return to_string(call_stack);
}

void free(StringizedCallStack &call_stack) {
	free(call_stack.string_buffer);
	free(call_stack.call_stack);
}


bool debugger_attached() {
	return IsDebuggerPresent();
}

}

#endif

#endif

#pragma warning(pop)
