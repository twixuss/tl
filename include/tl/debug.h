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
TL_API bool debug_add_module(void *module, Span<char> path);

// 0 context is current
TL_API List<void *> get_call_stack(void *context = 0, umm frames_to_skip = 0);

struct ResolveStackTraceNamesOptions {
	bool demangle = true;
	bool cleanup = false; // Replace msvc keywords with tl types.
};

TL_API StringizedCallStack resolve_names(Span<void *> call_stack, ResolveStackTraceNamesOptions options = {});
TL_API void free(StringizedCallStack &call_stack);

inline StringizedCallStack get_stack_trace() {
	return resolve_names(TL_TMP(get_call_stack()));
}

TL_API bool debugger_attached();

inline void append(StringBuilder &builder, StringizedCallStack::Entry const &entry) {
	append_format(builder, "{}:{}: {}", entry.file, entry.line, entry.name);
}
inline void append(StringBuilder &builder, StringizedCallStack const &call_stack) {
	for (auto entry : call_stack.call_stack) {
		append_format(builder, "{}\n", entry);
	}
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
#include "logger.h"
#include "win32.h"

// NOTE: From https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-undecoratesymbolname
// All DbgHelp functions, such as this one, are single threaded.
// Therefore, calls from more than one thread to this function will likely result in unexpected behavior or memory corruption.
// To avoid this, you must synchronize all concurrent calls from more than one thread to this function.
#include "thread.h"

#pragma warning(push, 0)
#pragma push_macro("OS_WINDOWS")
#undef OS_WINDOWS
#include <DbgHelp.h>
#pragma pop_macro("OS_WINDOWS")
#pragma warning(pop)

#pragma comment(lib, "dbghelp")

namespace tl {

static OsLock debug_lock;
static bool debug_initialized = false;

bool debug_init() {
	if (debug_initialized)
		return true;

	scoped(debug_lock);
	if (!SymInitialize(GetCurrentProcess(), 0, true)) {
		TL_GET_CURRENT(logger).error("SymInitialize failed: {}", win32_error());
		return false;
	}

	debug_initialized = true;

	DWORD options = SymGetOptions();
	SymSetOptions((options & ~SYMOPT_UNDNAME) | SYMOPT_PUBLICS_ONLY | SYMOPT_LOAD_LINES | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_DEBUG);
		
	#if 0
	SymRegisterCallback64(GetCurrentProcess(), [](HANDLE hProcess, ULONG ActionCode, ULONG64 CallbackData, ULONG64 UserContext) -> BOOL {
		switch (ActionCode) {
			case CBA_DEFERRED_SYMBOL_LOAD_START: {
				auto s = (IMAGEHLP_DEFERRED_SYMBOL_LOAD64 *)CallbackData;
				TL_GET_CURRENT(logger).error("DBGHELP: LOAD_START: {}", s->FileName);
				break;
			}
			case CBA_DEFERRED_SYMBOL_LOAD_COMPLETE: {
				auto s = (IMAGEHLP_DEFERRED_SYMBOL_LOAD64 *)CallbackData;
				TL_GET_CURRENT(logger).error("DBGHELP: LOAD_COMPLETE: {}", s->FileName);
				break;
			}
			case CBA_DEFERRED_SYMBOL_LOAD_FAILURE: {
				auto s = (IMAGEHLP_DEFERRED_SYMBOL_LOAD64 *)CallbackData;
				TL_GET_CURRENT(logger).error("DBGHELP: LOAD_FAILURE: {}", s->FileName);
				break;
			}
			case CBA_DEFERRED_SYMBOL_LOAD_CANCEL: {
				auto s = (IMAGEHLP_DEFERRED_SYMBOL_LOAD64 *)CallbackData;
				TL_GET_CURRENT(logger).error("DBGHELP: LOAD_CANCEL: {}", s->FileName);
				break;
			}
			case CBA_DEFERRED_SYMBOL_LOAD_PARTIAL: {
				auto s = (IMAGEHLP_DEFERRED_SYMBOL_LOAD64 *)CallbackData;
				TL_GET_CURRENT(logger).error("DBGHELP: LOAD_PARTIAL: {}", s->FileName);
				break;
			}
			case CBA_SYMBOLS_UNLOADED: break;
			case CBA_DUPLICATE_SYMBOL: {
				auto d = (IMAGEHLP_DUPLICATE_SYMBOL64 *)CallbackData;
				TL_GET_CURRENT(logger).error("DBGHELP: Duplicate symbol: {}", d->Symbol->Name);
				break;
			}
			case CBA_READ_MEMORY: break;
			case CBA_SET_OPTIONS: break;
			case CBA_EVENT: break;
			case CBA_DEBUG_INFO:
				TL_GET_CURRENT(logger).error((char *)CallbackData);
				break;
			case CBA_SRCSRV_INFO: break;
			case CBA_SRCSRV_EVENT: break;
			case CBA_UPDATE_STATUS_BAR: break;
			case CBA_ENGINE_PRESENT: break;
			case CBA_CHECK_ENGOPT_DISALLOW_NETWORK_PATHS: break;
			case CBA_CHECK_ARM_MACHINE_THUMB_TYPE_OVERRIDE: break;
			case CBA_XML_LOG: break;
		}
		return FALSE;
	}, 0);
	#endif

	return true;
}

void debug_deinit() {
	SymCleanup(GetCurrentProcess());
}

bool debug_add_module(void *module, Span<char> path) {
	// I wasted way too much time on this function...
	// I thought module handle would be enough.
	// And SymLoadModuleEx was succeeding but SymFromAddr was failing with ERROR_MOD_NOT_FOUND ??? Like wtf.

	if (!debug_initialized) {
		return false;
	}

	scoped(debug_lock);

	MODULEINFO mi;
	if (!GetModuleInformation(GetCurrentProcess(), (HMODULE)module, &mi, sizeof(mi))) {
		TL_GET_CURRENT(logger).error("debug_add_module({}, \"{}\"): GetModuleInformation failed: {}", module, path, win32_error());
		return false;
	}
	if (!SymLoadModuleEx(GetCurrentProcess(), NULL, with(current_temporary_allocator, null_terminate(path).data), NULL, (DWORD64)mi.lpBaseOfDll, mi.SizeOfImage, NULL, 0)) {
		auto error = win32_error();
		if (error.value != ERROR_SUCCESS) {
			TL_GET_CURRENT(logger).error("debug_add_module({}, \"{}\"): SymLoadModuleEx failed: {}", module, path, win32_error());
			return false;
		}
	}
	return true;
}

List<void *> get_call_stack(void *context_opaque, umm frames_to_skip) {
	scoped(debug_lock);
	
	CONTEXT context;
	
	if (context_opaque) {
		context = *(CONTEXT *)context_opaque;
	} else {
		context.ContextFlags = CONTEXT_CONTROL;
		RtlCaptureContext(&context);
	}

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
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &thread, 0, false, DUPLICATE_SAME_ACCESS);
	defer { CloseHandle(thread); };

	List<void *> call_stack;
	while (StackWalk64(image_type, GetCurrentProcess(), thread, &frame, &context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {
		if (frames_to_skip) {
			--frames_to_skip;
			continue;
		}

		call_stack.add((void *)frame.AddrPC.Offset);
	}

	return call_stack;
}

StringizedCallStack resolve_names(Span<void *> call_stack, ResolveStackTraceNamesOptions options) {
	scoped(debug_lock);

	StringizedCallStack result;
	for (auto &call : call_stack) {

		StringizedCallStack::Entry entry;
		Span<char> file = "unknown"s;
		Span<char> name = "unknown"s;
		char name_buffer[256];

        if (call == 0) {
			file = "nullptr"s;
			name = "nullptr"s;
		} else {
			constexpr u32 maxNameLength = MAX_SYM_NAME;

			static u8 symbol_buffer[sizeof(SYMBOL_INFO) + maxNameLength];
			auto symbol = (SYMBOL_INFO*)symbol_buffer;
			*symbol = {};
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = maxNameLength - 1;

			if (SymFromAddr(GetCurrentProcess(), (DWORD64)call + 1, NULL, symbol)) {
				#if 1

				if (options.demangle) {
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

					name.data = name_buffer;
					name.count = UnDecorateSymbolName(symbol->Name, name_buffer, (DWORD)count_of(name_buffer), flags);
				} else {
					name.data = symbol->Name;
					name.count = strlen(symbol->Name);
				}

				if (options.cleanup) {
					replace_inplace(name, "struct "s, ""s);
					replace_inplace(name, "unsigned __int8"s, "u8"s);
					replace_inplace(name, "unsigned __int16"s, "u16"s);
					replace_inplace(name, "unsigned __int32"s, "u32"s);
					replace_inplace(name, "unsigned __int64"s, "u64"s);
					replace_inplace(name, "__int8"s, "s8"s);
					replace_inplace(name, "__int16"s, "s16"s);
					replace_inplace(name, "__int32"s, "s32"s);
					replace_inplace(name, "__int64"s, "s64"s);
				}

				#else
				name = TL_TMP(demangle(symbol->Name));
				#endif

				IMAGEHLP_LINE64 line = {};
				line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
				DWORD line_displacement;

				if (SymGetLineFromAddr64(GetCurrentProcess(), (ULONG64)call, &line_displacement, &line)) {
					entry.line = line.LineNumber;
					file = as_span(line.FileName);
				}
			} else {
				auto error = GetLastError();
				TL_GET_CURRENT(logger).error("SymFromAddr failed: {}", win32_error());
				file = "SymFromAddr failed"s;
				name = "SymFromAddr failed"s;
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
