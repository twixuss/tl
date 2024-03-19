#pragma once

#include "common.h"
#include "list.h"
#include "stream.h"
#include "logger.h"
#include "function.h"

#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4996)

struct sockaddr_in;

namespace tl { namespace net {

template <class T>
struct MessageHeader {
	T type = {};
	u32 size;
};

template <class T>
struct Message {
	MessageHeader<T> header;
	Span<u8> data;
};

enum ConnectionKind {
	Connection_tcp,
	Connection_udp,
};

enum class ReceiveResult {
	disconnected,
	error,
};

typedef struct SocketImpl *Socket;

extern TL_API Logger logger;

TL_API bool init(Logger logger);
TL_API bool init();
TL_API bool deinit();
TL_API Socket create_socket(ConnectionKind kind);
TL_API bool close(Socket);
TL_API bool bind(Socket s, u16 port);
TL_API bool listen(Socket s, u32 n = 0x7fff'ffff);
TL_API Socket accept(Socket listener);
TL_API bool connect(Socket s, u32 ip, u16 port);
TL_API bool connect(Socket s, char const *ip, u16 port);
inline bool connect(Socket s, Span<char> ip, u16 port) {
	return connect(s, with(temporary_allocator, null_terminate(ip).data), port);
}
TL_API bool set_blocking(Socket s, bool blocking);
TL_API umm send_some(Socket s, void const *data, u32 size);
TL_API umm send_some(Socket s, sockaddr_in const &destination, void const *data, u32 size);

inline umm send_some(Socket s, Span<u8> span) {
	return send_some(s, span.data, (u32)span.count);
}
inline bool send_all(Socket s, Span<u8> span) {
	umm bytes_sent = 0;
	do {
		int n = send_some(s, span.data + bytes_sent, (u32)(span.count - bytes_sent));
		if (n == 0)
			return false;
		bytes_sent += n;
	} while (bytes_sent < span.count);
	return true;
}

struct ReceiveSomeOptions {
	u32 timeout_milliseconds = 1;
};

TL_API Result<umm, ReceiveResult> receive_some(Socket socket, void *data, u32 size, ReceiveSomeOptions options = {});
inline Result<umm, ReceiveResult> receive_some(Socket s, Span<u8> span, ReceiveSomeOptions options = {}) {
	return receive_some(s, span.data, (u32)span.count, options);
}

inline Result<bool, ReceiveResult> receive_all(Socket s, Span<u8> span) {
	umm bytes_received = 0;
	do {
		auto result = receive_some(s, span.data + bytes_received, (u32)(span.count - bytes_received));
		if (result.is_error()) {
			return result.error();
		}
		bytes_received += result.value();
	} while (bytes_received < span.count);
	return true;
}

struct TL_API TcpServer {
	Socket listener = 0;
	List<Socket> clients;
	void *user_data = 0;
	Function<void(Socket socket, Span<char> host, Span<char> service, u16 port)> on_client_connected = {};
	Function<void(Socket socket)>                                                on_client_disconnected = {};
	Function<void(Socket socket, Span<u8> message)>                              on_client_message_received = {};
	bool running = false;
	
	bool start(u16 port);
	void update();
};

}

struct NetStream : Stream {
	net::Socket socket;
	umm read(Span<u8> destination) { return net::receive_some(socket, destination).value_or(0); }
	umm write(Span<u8> source) { return net::send_some(socket, source); }
};

inline NetStream *create_stream(net::Socket socket) {
	auto result = create_stream<NetStream>();
	result->socket = socket;
	return result;
}

}


#ifdef TL_IMPL
#if OS_WINDOWS

#pragma warning(push, 0)
#pragma warning(disable: 4005) // macro redefinition
#include <WS2tcpip.h>
#include <iphlpapi.h>
#pragma warning(pop)

#pragma comment(lib, "iphlpapi")
#pragma comment(lib, "ws2_32")

#include "default_logger.h"
#include "win32.h"
#include "win32_error.h"

namespace tl { namespace net {

Logger logger;

FormattedWin32Error wsa_error() {
	return {(DWORD)WSAGetLastError()};
}

bool init(Logger logger) {
	tl::net::logger = logger;

	WSADATA wsa_data = {};
	auto error = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (error != 0) {
		logger.error("Failed to startup WSA: {}", FormattedWin32Error{(DWORD)error});
		return false;
	}
	return true;
}
bool init() {
	static DefaultLogger default_logger = {.module = u8"tl::net"s};
	return init(default_logger);
}
bool deinit() {
	if (WSACleanup() != 0) {
		logger.error("Failed to cleanup WSA: {}", wsa_error());
		return false;
	}
	return true;

}

static bool check_error_impl(int code, char const *function_name) {
	if (code == SOCKET_ERROR) {
		logger.error("{} failed: {}", function_name, wsa_error());
		return false;
	}
	return true;
}
static bool check_socket_impl(SOCKET socket, char const *function_name) {
	if (socket == INVALID_SOCKET) {
		logger.error("{} failed: {}", function_name, wsa_error());
		return false;
	}
	return true;
}

#define check_error(code) check_error_impl(code, __FUNCTION__)
#define check_socket(code) check_socket_impl(code, __FUNCTION__)

Socket create_socket(ConnectionKind kind) {
	SOCKET s = ::socket(AF_INET, kind == Connection_tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
	if (!check_socket(s)) {
		return {};
	}
	return (Socket)s;
}
bool close(Socket s) {
	return check_error(closesocket((SOCKET)s));
}

bool bind(Socket s, u16 port) {
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	return check_error(::bind((SOCKET)s, (SOCKADDR *)&addr, sizeof(addr)));
}

bool listen(Socket s, u32 n) {
	return check_error(::listen((SOCKET)s, n));
}

bool connect(Socket s, u32 ip, u16 port) {
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = ip;
	return check_error(::connect((SOCKET)s, (SOCKADDR *)&addr, sizeof(addr)));
}

bool connect(Socket s, char const *ip, u16 port) {
	return connect(s, inet_addr(ip), port);
}

#include <fcntl.h>

bool set_blocking(Socket s, bool blocking) {
	//  https://stackoverflow.com/a/1549344
	unsigned long mode = blocking ? 0 : 1;
	if (ioctlsocket((int)s, FIONBIO, &mode) != 0) {
		logger.error("Failed to set blocking mode on socket.");
		return false;
	}
	return true;
}

umm send_some(Socket s, void const *data, u32 size) {
	auto result = ::send((SOCKET)s, (char *)data, (int)size, 0);
	if (!check_error(result)) {
		return 0;
	}
	assert(result > 0, "send returned negative bullshit");
	return result;
}

umm send_some(Socket s, sockaddr_in const &destination, void const *data, u32 size) {
	auto result = ::sendto((SOCKET)s, (char *)data, (int)size, 0, (sockaddr *)&destination, sizeof(destination));
	if (!check_error(result)) {
		return 0;
	}
	assert(result > 0, "sendto returned negative bullshit");
	return result;
}

Result<umm, ReceiveResult> receive_some(Socket socket, void *data, u32 size, ReceiveSomeOptions options) {
	if (options.timeout_milliseconds != 0) {

		fd_set fdSet;
		fdSet.fd_count = 1;
		fdSet.fd_array[0] = (SOCKET)socket;

		timeval timeout = {};
		timeout.tv_usec = options.timeout_milliseconds * 1000;
		int socketCount = ::select(0, &fdSet, nullptr, nullptr, &timeout);

		if (socketCount < 0) {
			current_logger.error("select: {}", WSAGetLastError());
		}

		if (socketCount == 0) {
			return 0;
		}

		assert_equal(socketCount, 1);
	}

	int result = ::recv((SOCKET)socket, (char *)data, (int)size, 0);

	if (result == 0) {
		logger.info("receive: connection was closed");
		return ReceiveResult::disconnected;
	}

	if (!check_error(result)) {
		return ReceiveResult::error;
	}

	assert(result > 0, "recv returned negative bullshit");

	return (umm)result;
}

Socket accept(Socket listener) {
	SOCKADDR_IN client;
	int clientSize = sizeof(client);
	auto socket = accept((SOCKET)listener, (SOCKADDR*)&client, &clientSize);
	if (!check_socket(socket)) {
		return {};
	}
	return (Socket)socket;
}

bool TcpServer::start(u16 port) {
	listener = create_socket(Connection_tcp);
	if (!listener)
		return false;
	if (!bind(listener, port))
		return false;
	if (!listen(listener))
		return false;

	running = true;
	return true;
}

void TcpServer::update() {
	fd_set fdSet;
	fdSet.fd_count = (u_int)clients.count + 1;
	fdSet.fd_array[0] = (SOCKET)listener;
	for (u32 i = 0; i < clients.count; ++i) {
		fdSet.fd_array[i + 1] = (SOCKET)clients[i];
	}

	timeval timeout = {};
	timeout.tv_sec = 1;
	int socketCount = ::select(0, &fdSet, nullptr, nullptr, &timeout);

	if (socketCount < 0) {
		current_logger.error("select: {}", WSAGetLastError());
	}

	for (int i = 0; i < socketCount; i++) {
		SOCKET socket = fdSet.fd_array[i];
		if (socket == (SOCKET)listener) {
			SOCKADDR_IN client;
			int clientSize = sizeof(client);
			SOCKET clientSocket = ::accept((SOCKET)listener, (SOCKADDR *)&client, &clientSize);
			if (clientSocket == INVALID_SOCKET) {
				continue;
			}

			char host[NI_MAXHOST];
			host[0] = 0;
			char service[NI_MAXSERV];
			service[0] = 0;
			if (getnameinfo((SOCKADDR *)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) != 0) {
				inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			}
			clients.add((Socket)clientSocket);
			if (on_client_connected)
				on_client_connected((Socket)clientSocket, as_span(host), as_span(service), ntohs(client.sin_port));
		} else {
			u8 buf[4096];
			int bytesReceived = recv(socket, (char *)buf, sizeof(buf), 0);
			if (bytesReceived <= 0) {
				if (on_client_disconnected)
					on_client_disconnected((Socket)socket);
				find_and_erase(clients, (Socket)socket);
				continue;
			}
			if (on_client_message_received)
				on_client_message_received((Socket)socket, Span{buf, (umm)bytesReceived});
		}
	}
}

#undef check_error
#undef check_socket

}}

#endif
#endif

#pragma warning(pop)
