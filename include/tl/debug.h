#pragma once
#include "list.h"
#include "string.h"
#include "console.h"
#include "hash.h"

#pragma warning(push)
#pragma warning(disable: 4820)

namespace tl {

struct StringizedCallStack {
	struct Entry {
		Span<char> name;
		Span<char> file;
		u32 line = 0;

		inline constexpr bool operator==(Entry const &that) const {
			return
				this->line == that.line &&
				this->name == that.name &&
				this->file == that.file;
		}
	};
	List<Entry> call_stack;
	List<char> string_buffer;
};

TL_API bool debug_init();
TL_API void debug_deinit();

TL_API List<void *> get_call_stack(umm frames_to_skip = 0);

TL_API StringizedCallStack resolve_names(Span<void *> call_stack);
TL_API void free(StringizedCallStack &call_stack);

inline StringizedCallStack get_stack_trace() {
	return resolve_names(TL_TMP(get_call_stack()));
}

#ifdef CreateWindow
TL_API StringizedCallStack get_stack_trace(CONTEXT context, umm frames_to_skip);
#endif

TL_API bool debugger_attached();

inline umm append(StringBuilder &builder, StringizedCallStack::Entry const &entry) {
	return append_format(builder, "{}:{}: {}", entry.file, entry.line, entry.name);
}
inline umm append(StringBuilder &builder, StringizedCallStack const &call_stack) {
	umm result = 0;
	for (auto entry : call_stack.call_stack) {
		result += append_format(builder, "{}\n", entry);
	}
	return result;
}

}

template <>
inline tl::u64 get_hash(tl::StringizedCallStack const &s) {
	return get_hash(s.call_stack);
}

template <>
inline tl::u64 get_hash(tl::StringizedCallStack::Entry const &e) {
	return get_hash(e.file) ^ get_hash(e.line) ^ get_hash(e.name);
}

#ifdef TL_IMPL

#if OS_WINDOWS

#include "compiler.h"

#pragma warning(push, 0)
#pragma push_macro("OS_WINDOWS")
#undef OS_WINDOWS
#include <DbgHelp.h>
#pragma pop_macro("OS_WINDOWS")
#pragma warning(pop)

#pragma comment(lib, "dbghelp")

namespace tl {

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

static List<void *> get_call_stack(CONTEXT context, u32 frames_to_skip) {
	STACKFRAME64 frame = {};
#if ARCH_X64
	frame.AddrPC.Offset    = context.Rip;
	frame.AddrStack.Offset = context.Rsp;
	frame.AddrFrame.Offset = context.Rbp;
#else
	frame.AddrPC.Offset    = context.Eip;
	frame.AddrStack.Offset = context.Esp;
	frame.AddrFrame.Offset = context.Ebp;
#endif
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

	List<void *> call_stack;
	while (StackWalk64(image_type, debug_process, thread, &frame, &context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {
		if (frames_to_skip) {
			--frames_to_skip;
			continue;
		}

		call_stack.add((void *)frame.AddrPC.Offset);
	}

	return call_stack;
}
List<void *> get_call_stack(umm frames_to_skip) {
	CONTEXT context = {};
	context.ContextFlags = CONTEXT_CONTROL;
	RtlCaptureContext(&context);

	return get_call_stack(context, 1 + frames_to_skip);
}

StringizedCallStack resolve_names(Span<void *> call_stack) {
	StringizedCallStack result;
	for (auto &call : call_stack) {

		StringizedCallStack::Entry entry;
		Span<char> file = "unknown"s;
		Span<char> name = "unknown"s;

        if (call == 0) {
			file = "NULL"s;
			name = "ATTEMPT TO EXECUTE AT ADDRESS 0"s;
		} else {
			constexpr u32 maxNameLength = MAX_SYM_NAME;
			DWORD64 displacement;

			static u8 buffer[sizeof(SYMBOL_INFO) + maxNameLength] = {};
			auto symbol = (SYMBOL_INFO*)buffer;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = maxNameLength;

			if (!SymFromAddr(debug_process, (DWORD64)call, &displacement, symbol)) {
				auto error = GetLastError();
				print("SymFromAddr failed with code: 0x{} ({})\n", FormatInt{.value = error, .radix = 16}, error);
				break;
			}

#if 1
			DWORD const flags = UNDNAME_NO_MS_KEYWORDS
				| UNDNAME_NO_ACCESS_SPECIFIERS 
				| UNDNAME_NO_CV_THISTYPE 
				| UNDNAME_NO_FUNCTION_RETURNS 
				| UNDNAME_NO_MEMBER_TYPE 
				| UNDNAME_NO_ALLOCATION_LANGUAGE 
				| UNDNAME_NO_ALLOCATION_MODEL 
				| UNDNAME_NO_MS_THISTYPE 
				| UNDNAME_NO_RETURN_UDT_MODEL
				| UNDNAME_NO_SPECIAL_SYMS
				| UNDNAME_NO_THISTYPE
				| UNDNAME_NO_THROW_SIGNATURES
			;
			char name_buffer[256];
			name.data = name_buffer;
			name.count = UnDecorateSymbolName(symbol->Name, name_buffer, (DWORD)count_of(name_buffer), flags);
#else
			name = TL_TMP(demangle(symbol->Name));
#endif

			IMAGEHLP_LINE64 line = {};
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			DWORD line_displacement;

			if (SymGetLineFromAddr64(debug_process, (ULONG64)call, &line_displacement, &line)) {
				entry.line = line.LineNumber;
				file = as_span(line.FileName);
			}
		}

		entry.name.data = (char *)result.string_buffer.count;
		entry.name.count = name.count;
		result.string_buffer.add(name);

		entry.file.data = (char *)result.string_buffer.count;
		entry.file.count = file.count;
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
	auto call_stack = TL_TMP(get_call_stack(context, frames_to_skip));
	return resolve_names(call_stack);
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
