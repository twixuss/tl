#pragma once

#include "common.h"
#include "list.h"
#include "stream.h"

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

TL_DECLARE_HANDLE(Socket);

TL_API bool init();
TL_API void deinit();
TL_API Socket create_socket(ConnectionKind kind);
TL_API void close(Socket);
TL_API void bind(Socket s, u16 port);
TL_API void listen(Socket s, u32 n = 0x7fff'ffff);
TL_API Socket accept(Socket listener);
TL_API bool connect(Socket s, u32 ip, u16 port);
TL_API bool connect(Socket s, char const *ip, u16 port);
TL_API umm send(Socket s, void const *data, u32 size);
TL_API umm send(Socket s, sockaddr_in const &destination, void const *data, u32 size);
TL_API umm receive(Socket socket, void *data, u32 size);

inline umm send(Socket s, Span<u8> span) {
	return send(s, span.data, (u32)span.count);
}
inline umm receive(Socket s, Span<u8> span) {
	return receive(s, span.data, (u32)span.count);
}
inline bool send_all(Socket s, Span<u8> span) {
	umm bytes_sent = 0;
	do {
		int n = send(s, span.data + bytes_sent, (u32)(span.count - bytes_sent));
		if (n < 0)
			return false;
		bytes_sent += n;
	} while (bytes_sent < span.count);
	return true;
}
template <class T>
inline umm send_bytes(Socket s, T const &val) {
	return send(s, std::addressof(val), sizeof(val));
}
template <class T>
inline void send_bytes(Socket s, sockaddr_in const &destination, T const &val) {
	send(s, destination, std::addressof(val), sizeof(val));
}

struct TcpServer {
	Socket listener = 0;
	List<Socket> clients;
	void (*on_client_connected)(TcpServer &server, void *context, Socket s, char *host, char *service, u16 port) = 0;
	void (*on_client_disconnected)(TcpServer &server, void *context, Socket s) = 0;
	void (*on_client_message_received)(TcpServer &server, void *context, Socket s, u8 *data, u32 size) = 0;
	bool running = false;
};

TL_API void run(TcpServer *server, void *context = 0);
TL_API void run_udp(TcpServer *server, void *context = 0);

}

struct NetStream : Stream {
	net::Socket socket;
	umm read(Span<u8> destination) { return net::receive(socket, destination); }
	umm write(Span<u8> source) { return net::send(socket, source); }
};

inline Stream *create_file_stream(net::Socket socket) {
	auto result = create_stream<NetStream>();
	result->socket = socket;
	return result;
}

}


#ifdef TL_IMPL
#if OS_WINDOWS

#pragma warning(push, 0)
#include <WS2tcpip.h>
#include <iphlpapi.h>
#pragma warning(pop)

#pragma comment(lib, "iphlpapi")
#pragma comment(lib, "ws2_32")

namespace tl { namespace net {

bool init() {
	WSADATA wsaData = {};
	return ::WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}
void deinit() {
	::WSACleanup();
}

Socket create_socket(ConnectionKind kind) {
	SOCKET s = ::socket(AF_INET, kind == Connection_tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) {
		return {};
	}
	return (Socket)s;
}
void close(Socket s) {
	closesocket((SOCKET)s);
}

void bind(Socket s, u16 port) {
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	::bind((SOCKET)s, (SOCKADDR *)&addr, sizeof(addr));
}

void listen(Socket s, u32 n) {
	::listen((SOCKET)s, n);
}

bool connect(Socket s, u32 ip, u16 port) {
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = ip;
	return ::connect((SOCKET)s, (SOCKADDR*)& addr, sizeof(addr)) == 0;
}

bool connect(Socket s, char const *ip, u16 port) {
	return connect(s, inet_addr(ip), port);
}

umm send(Socket s, void const *data, u32 size) {
	return ::send((SOCKET)s, (char *)data, (int)size, 0);
}

umm send(Socket s, sockaddr_in const &destination, void const *data, u32 size) {
	return ::sendto((SOCKET)s, (char *)data, (int)size, 0, (sockaddr *)&destination, sizeof(destination));
}

umm receive(Socket socket, void *data, u32 size) {
	return ::recv((SOCKET)socket, (char *)data, (int)size, 0);

}

Socket accept(Socket listener) {
	SOCKADDR_IN client;
	int clientSize = sizeof(client);
	auto socket = accept((SOCKET)listener, (SOCKADDR*)&client, &clientSize);
	if (socket == INVALID_SOCKET) {
		return {};
	}
	return (Socket)socket;
}

void run(TcpServer &server, void *context) {
	server.running = true;
	while (server.running) {
		fd_set fdSet;
		fdSet.fd_count = (u_int)server.clients.count + 1;
		fdSet.fd_array[0] = (SOCKET)server.listener;
		for (u32 i = 0; i < server.clients.count; ++i) {
			fdSet.fd_array[i + 1] = (SOCKET)server.clients[i];
		}

		timeval timeout = {};
		timeout.tv_sec = 1;
		int socketCount = ::select(0, &fdSet, nullptr, nullptr, &timeout);
		for (int i = 0; i < socketCount; i++) {
			SOCKET socket = fdSet.fd_array[i];
			if (socket == (SOCKET)server.listener) {
				SOCKADDR_IN client;
				int clientSize = sizeof(client);
				SOCKET clientSocket = ::accept((SOCKET)server.listener, (SOCKADDR *)&client, &clientSize);
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
				server.clients.add((Socket)clientSocket);
				server.on_client_connected(server, context, (Socket)clientSocket, host, service, ntohs(client.sin_port));
			} else {
				u8 buf[4096];
				int bytesReceived = recv(socket, (char *)buf, sizeof(buf), 0);
				if (bytesReceived <= 0) {
					server.on_client_disconnected(server, context, (Socket)socket);
					find_and_erase(server.clients, (Socket)socket);
					continue;
				}
				server.on_client_message_received(server, context, (Socket)socket, buf, (u32)bytesReceived);
			}
		}
	}
}

}}

#endif
#endif

#pragma warning(pop)
