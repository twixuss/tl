#pragma once
#include "list.h"
#include "string.h"

#pragma warning(push)
#pragma warning(disable: 4820)

namespace TL {

struct CallStackEntry {
	String<> name;
	String<> file;
	u32 line = 0;
};

using StackTrace = List<CallStackEntry>;

TL_API StackTrace getStackTrace();
TL_API bool isDebuggerAttached();

}

#ifdef TL_IMPL

#if OS_WINDOWS

#pragma warning(push, 0)
#include <DbgHelp.h>
#include <debugapi.h>
#pragma warning(pop)

#pragma comment(lib, "dbghelp")

namespace TL {

StackTrace getStackTrace() {
	StackTrace result;

	auto process = GetCurrentProcess();
	if (!SymInitialize(process, 0, true)) {
		return {};
	}
 
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

	DWORD options = SymGetOptions();
	SymSetOptions(options & ~SYMOPT_UNDNAME | SYMOPT_PUBLICS_ONLY);

	u32 frameIndex = 0;
	while (StackWalk64(IMAGE_FILE_MACHINE_AMD64, process, thread, &frame, &context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {
		if (!frameIndex++)
			continue;
		constexpr u32 maxNameLength = MAX_SYM_NAME;
		DWORD64 displacement;

		static u8 buffer[sizeof(SYMBOL_INFO) + maxNameLength * sizeof(char)] = {};
		PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = maxNameLength;
		
		if (!SymFromAddr(process, frame.AddrPC.Offset, &displacement, symbol)) {
			break;
		}

		CallStackEntry entry;

		char demangledName[256];
		auto demangledNameLength = UnDecorateSymbolName(symbol->Name, demangledName, (DWORD)count_of(demangledName), UNDNAME_NAME_ONLY);
		if (demangledNameLength) {
			entry.name = {demangledName, demangledNameLength};
		}

		IMAGEHLP_LINE64 line = {};
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
		DWORD lineDisplacement;
		if (SymGetLineFromAddr64(process, symbol->Address, &lineDisplacement, &line)) {
			entry.file = line.FileName;
			entry.line = line.LineNumber;
		} else {
			entry.file = "unknown";
		}

		result.push_back(std::move(entry));

	}
 
	SymSetOptions(options);
	SymCleanup(process);

	return result;
}

bool isDebuggerAttached() {
	return IsDebuggerPresent();
}

}

#endif

#endif

#pragma warning(pop)
