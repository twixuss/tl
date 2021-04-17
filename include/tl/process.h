#pragma once
#include "common.h"

namespace TL {

TL_DECLARE_HANDLE(Process);

TL_IMPL Process execute(Span<utf8> path_, Span<utf8> arguments_, bool visible);
TL_IMPL bool wait(Process process, u32 timeout = -1);
TL_IMPL u32 get_exit_code(Process process);
TL_IMPL void terminate(Process process);
//
// Ends the lifetime of the handle
// Does not terminate the process!
//
void free(Process &process);

#ifdef TL_IMPL

#if OS_WINDOWS

Process execute(Span<utf8> path_, Span<utf8> arguments_, bool visible) {
	auto path = utf8_to_utf16(path_, true);
	defer { free(path); };
	
	auto arguments = utf8_to_utf16(arguments_, true);
	defer { free(arguments); };

	SHELLEXECUTEINFOW ShExecInfo = {};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = (wchar *)path.data;
	ShExecInfo.lpParameters = (wchar *)arguments.data;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = visible ? SW_SHOW : SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	if (!ShellExecuteExW(&ShExecInfo)) {
		auto error = GetLastError();
		print("ShellExecuteExA failed with error code 0x% (%)\n", FormatInt(error, 16), error);
		return 0;
	}

	return (Process)ShExecInfo.hProcess;
}

bool wait(Process process, u32 timeout) {
	return WaitForSingleObject((HANDLE)process, timeout) == WAIT_OBJECT_0;
}

u32 get_exit_code(Process process) {
	DWORD exit_code;
	GetExitCodeProcess((HANDLE)process, &exit_code);
	return (u32)exit_code;
}

void terminate(Process process) {
	TerminateProcess(process, 1);
}

void free(Process &process) {
	CloseHandle((HANDLE)process);
	process = 0;
}

#endif

#endif
}
