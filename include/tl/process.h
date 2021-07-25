#pragma once
#include "file.h"

namespace tl {

TL_DECLARE_HANDLE(Process);

TL_API Process execute(pathchar const *path, pathchar const *arguments, bool visible);
inline Process execute(Span<pathchar> path, Span<pathchar> arguments, bool visible) {
	assert(path.back() == 0);
	assert(arguments.back() == 0);
	return execute(path.data, arguments.data, visible);
}
TL_API bool wait(Process process, u32 timeout = -1);
TL_API u32 get_exit_code(Process process);
TL_API void terminate(Process process);
//
// Ends the lifetime of the handle
// Does not terminate the process!
//
void free(Process &process);

#ifdef TL_IMPL

#if OS_WINDOWS

Process execute(pathchar const *path, pathchar const *arguments, bool visible) {
	SHELLEXECUTEINFOW ShExecInfo = {};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = (wchar *)path;
	ShExecInfo.lpParameters = (wchar *)arguments;
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
