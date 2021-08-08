#pragma once
#include "file.h"

namespace tl {

TL_DECLARE_HANDLE(Process);

struct ExecuteParams {
	bool visible = true;
};

TL_API Process execute(pathchar const *path, pathchar const *arguments, ExecuteParams params = {});
inline Process execute(pathchar const *path, ExecuteParams params = {}) {
	return execute(path, 0, params);
}

inline Process execute(Span<pathchar> path, Span<pathchar> arguments, ExecuteParams params = {}) {
	tl_check_null_terminator(path);
	tl_check_null_terminator(arguments);
	return execute(path.data, arguments.data, params);
}
inline Process execute(Span<pathchar> path, ExecuteParams params = {}) {
	tl_check_null_terminator(path);
	return execute(path.data, 0, params);
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

#pragma comment(lib, "Shell32.lib")

Process execute(pathchar const *path, pathchar const *arguments, ExecuteParams params) {
	SHELLEXECUTEINFOW ShExecInfo = {};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = (wchar *)path;
	ShExecInfo.lpParameters = (wchar *)arguments;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = params.visible ? SW_SHOW : SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	if (!ShellExecuteExW(&ShExecInfo)) {
		auto error = GetLastError();
		print(Print_error, "ShellExecuteExA failed with error code 0x% (%)\n", FormatInt{.value = error, .radix = 16}, error);
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
