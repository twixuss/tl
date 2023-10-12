#pragma once
#include "file.h"
#include "stream.h"

namespace tl {

struct MemoryInfo {
	u64 current_usage = 0;
	u64 peak_usage = 0;
};

MemoryInfo get_memory_info();

struct Process {
	void *handle = 0;
	Stream *standard_out = 0;
};

inline bool is_valid(Process process) {
	return process.handle != 0;
}

struct ExecuteParams {
	bool visible = true;
};

TL_API Process execute(utf16 const *path, utf16 const *arguments, ExecuteParams params = {});
inline Process execute(utf16 const *path, ExecuteParams params = {}) {
	return execute(path, 0, params);
}

inline Process execute(Span<utf16> path, Span<utf16> arguments, ExecuteParams params = {}) {
	scoped(temporary_allocator_and_checkpoint);
	return execute(null_terminate(path).data, null_terminate(arguments).data, params);
}
inline Process execute(Span<utf16> path, ExecuteParams params = {}) {
	scoped(temporary_allocator_and_checkpoint);
	return execute(null_terminate(path).data, 0, params);
}
inline Process execute(Span<utf8> path, ExecuteParams params = {}) {
	return execute(to_utf16(path, true).data, 0, params);
}

TL_API bool wait(Process process, u32 timeout = -1);
TL_API bool is_running(Process process);
TL_API u32 get_exit_code(Process process);
TL_API void terminate(Process process);
//
// Ends the lifetime of the handle
// Does not terminate the process!
//
void free(Process &process);

TL_API Process start_process(utf16 const *command_line);
inline Process start_process(Span<utf16> command_line) {
	return start_process(with(temporary_allocator, null_terminate(command_line)).data);
}
inline Process start_process(Span<utf8> command_line) {
	return start_process(with(temporary_allocator, to_utf16(command_line, true)).data);
}

inline List<u8> start_process_and_get_output(Span<utf8> command_line) {
	auto process = start_process(command_line);
	StringBuilder builder;
	while (true) {
		u8 buffer[256];
		auto bytes_read = process.standard_out->read(array_as_span(buffer));
		if (bytes_read == 0) {
			break;
		}
		append(builder, Span(buffer, bytes_read));
	}
	free(process);
	return to_string(builder);
}

}

#ifdef TL_IMPL

#if OS_WINDOWS
#define NOMINMAX
#include <Psapi.h>
#endif

#include "console.h"

namespace tl {

#if OS_WINDOWS

#pragma comment(lib, "Shell32.lib")

MemoryInfo get_memory_info() {
	MemoryInfo result;
	PROCESS_MEMORY_COUNTERS counters;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters))) {
		result.current_usage = counters.WorkingSetSize;
		result.peak_usage = counters.PeakWorkingSetSize;
	}
	return result;
}
Process execute(utf16 const *path, utf16 const *arguments, ExecuteParams params) {
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
		with(ConsoleColor::red, errln("ShellExecuteExA failed with error code 0x{} ({})\n", FormatInt{.value = error, .radix = 16}, error));
		return {};
	}

	return {.handle = ShExecInfo.hProcess};
}

bool wait(Process process, u32 timeout) {
	return WaitForSingleObject((HANDLE)process.handle, timeout) == WAIT_OBJECT_0;
}

bool is_running(Process process) {
	DWORD exit_code;
	GetExitCodeProcess((HANDLE)process.handle, &exit_code);
	return exit_code == STILL_ACTIVE;
}

u32 get_exit_code(Process process) {
	DWORD exit_code;
	GetExitCodeProcess((HANDLE)process.handle, &exit_code);
	return (u32)exit_code;
}

void terminate(Process process) {
	TerminateProcess(process.handle, 1);
}

void free(Process &process) {
	CloseHandle((HANDLE)process.handle);
	free(process.standard_out);
	process = {};
}

// TODO: confusing name, this is another process' stdout
struct StdoutStream : Stream {
	void *handle;
	umm read(Span<u8> destination) {
		DWORD bytes_read;
		if (!ReadFile(handle, destination.data, (DWORD)destination.count, &bytes_read, 0))
			return 0;
		return bytes_read;
	}
	void free() {
		CloseHandle(handle);
	}
};

Process start_process(utf16 const *command_line) {
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(saAttr);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT.
	HANDLE child_stdout_read;
	HANDLE child_stdout_write;
	if (!CreatePipe(&child_stdout_read, &child_stdout_write, &saAttr, 0)) {
		return {};
	}

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(child_stdout_read, HANDLE_FLAG_INHERIT, 0)) {
		return {};
	}

	auto child_stdout_stream = create_stream<StdoutStream>();
	child_stdout_stream->handle = child_stdout_read;


	PROCESS_INFORMATION piProcInfo = {};

	STARTUPINFOW siStartInfo = {};
	siStartInfo.cb = sizeof(siStartInfo);
	siStartInfo.hStdError = child_stdout_write;
	siStartInfo.hStdOutput = child_stdout_write;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	if (!CreateProcessW(NULL,
		(wchar *)command_line,
		NULL,          // process security attributes
		NULL,          // primary thread security attributes
		TRUE,          // handles are inherited
		0,             // creation flags
		NULL,          // use parent's environment
		NULL,          // use parent's current directory
		&siStartInfo,  // STARTUPINFO pointer
		&piProcInfo)   // receives PROCESS_INFORMATION
	) {
		free(child_stdout_stream);
		return {};
	}

	CloseHandle(piProcInfo.hThread);
	CloseHandle(child_stdout_write);

	Process result;
	result.handle = piProcInfo.hProcess;
	result.standard_out = child_stdout_stream;
	return result;
}

#endif

}

#endif
