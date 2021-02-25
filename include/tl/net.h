#pragma once

#include "common.h"
#include "list.h"

#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4996)

struct sockaddr_in;

namespace TL { namespace net {

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
TL_API void bind(Socket s, u16 port);
TL_API void listen(Socket s);
TL_API bool connect(Socket s, u32 ip, u16 port);
TL_API bool connect(Socket s, char const *ip, u16 port);
TL_API void send(Socket s, void const *data, u32 size);
TL_API void send(Socket s, sockaddr_in const &destination, void const *data, u32 size);
TL_API Span<u8> receive(Socket s, u32 maxSize);

inline void send(Socket s, Span<u8 const> span) {
	send(s, span.data, (u32)span.size);
}
template <class T>
inline void send_bytes(Socket s, T const &val) {
	send(s, std::addressof(val), sizeof(val));
}
template <class T>
inline void send_bytes(Socket s, sockaddr_in const &destination, T const &val) {
	send(s, destination, std::addressof(val), sizeof(val));
}
struct TcpServer {
	Socket listener;
	List<Socket> clients;
	void (*onClientConnected)(TcpServer *server, void *context, Socket s, char *host, char *service, u16 port);
	void (*onClientDisconnected)(TcpServer *server, void *context, Socket s);
	void (*onClientMessageReceived)(TcpServer *server, void *context, Socket s, u8 *data, u32 size);
	bool running;
};

TL_API TcpServer *create_tcp_server(Socket listener);
TL_API void run(TcpServer *server, void *context = 0);
TL_API void run_udp(TcpServer *server, void *context = 0);

}}


#ifdef TL_IMPL
#if OS_WINDOWS

#pragma warning(push, 0)
#include <WS2tcpip.h>
#include <iphlpapi.h>
#pragma warning(pop)

#pragma comment(lib, "iphlpapi")
#pragma comment(lib, "ws2_32")

namespace TL { namespace net {

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

void bind(Socket s, u16 port) {
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	::bind((SOCKET)s, (SOCKADDR *)&addr, sizeof(addr));
}

void listen(Socket s) {
	::listen((SOCKET)s, SOMAXCONN);
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

void send(Socket s, void const *data, u32 size) {
	::send((SOCKET)s, (char *)data, (int)size, 0);
}

void send(Socket s, sockaddr_in const &destination, void const *data, u32 size) {
	::sendto((SOCKET)s, (char *)data, (int)size, 0, (sockaddr *)&destination, sizeof(destination));
}

Span<u8> receive(Socket s, u32 maxSize) {
	u8 *data = ALLOCATE_T(TL_DEFAULT_ALLOCATOR, u8, maxSize, 0);
	int bytesReceived = ::recv((SOCKET)s, (char *)data, (int)maxSize, 0);
	if (bytesReceived <= 0) {
		DEALLOCATE(TL_DEFAULT_ALLOCATOR, data);
		return {};
	}
	return {data, (umm)bytesReceived};

}
Span<u8> receive_into(Socket s, void *data, u32 maxSize) {
	int bytesReceived = ::recv((SOCKET)s, (char *)data, (int)maxSize, 0);
	if (bytesReceived <= 0) {
		return {};
	}
	return {(u8 *)data, (umm)bytesReceived};

}

TcpServer *create_tcp_server(Socket listener) {
	TcpServer *server = construct(ALLOCATE_T(TL_DEFAULT_ALLOCATOR, TcpServer, 1, 0));
	server->listener = listener;

	/*
	List<IP_ADAPTER_ADDRESSES> addresses;
	addresses.resize(50);
	ULONG addressesCount;
	while (1) {
		addressesCount = count_of(addresses);
		if (GetAdaptersAddresses(AF_INET, 0, 0, addresses.data(), &addressesCount) == NO_ERROR) {
			break;
		}
		addresses.resize(addressesCount * 2);
	}
	*/

	return server;
}

void run(TcpServer *server, void *context) {
	server->running = true;
	while (server->running) {
		fd_set fdSet;
		fdSet.fd_count = (u_int)server->clients.size() + 1;
		fdSet.fd_array[0] = (SOCKET)server->listener;
		for (u32 i = 0; i < server->clients.size(); ++i) {
			fdSet.fd_array[i + 1] = (SOCKET)server->clients[i];
		}

		timeval timeout = {};
		timeout.tv_sec = 1;
		int socketCount = ::select(0, &fdSet, nullptr, nullptr, &timeout);
		for (int i = 0; i < socketCount; i++) {
			SOCKET socket = fdSet.fd_array[i];
			if (socket == (SOCKET)server->listener) {
				SOCKADDR_IN client;
				int clientSize = sizeof(client);
				SOCKET clientSocket = ::accept((SOCKET)server->listener, (SOCKADDR *)&client, &clientSize);
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
				server->clients.push_back((Socket)clientSocket);
				server->onClientConnected(server, context, (Socket)clientSocket, host, service, ntohs(client.sin_port));
			} else {
				u8 buf[4096];
				int bytesReceived = recv(socket, (char *)buf, sizeof(buf), 0);
				if (bytesReceived <= 0) {
					server->onClientDisconnected(server, context, (Socket)socket);
					server->clients.erase(std::find(server->clients.begin(), server->clients.end(), (Socket)socket));
					continue;
				}
				server->onClientMessageReceived(server, context, (Socket)socket, buf, (u32)bytesReceived);
			}
		}
	}
}

}}

#endif
#endif

#pragma warning(pop)
