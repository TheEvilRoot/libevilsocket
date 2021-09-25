#include "evilsocket.h"

#include <memory>

errno_t es_init() {
	WSADATA wsaData{};
	return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void es_deinit() {
	WSACleanup();
}

errno_t es_last_error() {
	return WSAGetLastError();	
}

std::string es_error_message(errno_t error_code) {
	char* s = NULL;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, error_code,
		MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
		(LPSTR)&s, 0, NULL);
	std::string string(s);
	LocalFree(s);
	return string;
}

SOCKET es_connect(const std::string& host, int port, int keepalivems) {
	SOCKET handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (handle == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}

	SOCKADDR_IN serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(host.c_str());
	serverAddress.sin_port = htons(port);

	auto res = connect(handle, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress));
	if (res == 0)
		return handle;
	return INVALID_SOCKET;
}

void es_close(SOCKET handle) {
	if (handle != INVALID_SOCKET)
		closesocket(handle);
}

size_t es_write_string(SOCKET handle, std::string string) {
	if (handle == INVALID_SOCKET) {
		WSASetLastError(WSA_INVALID_HANDLE);
		return 0;
	}
	if (string.length() == 0) {
		WSASetLastError(0);
		return 0;
	}
	auto res = send(handle, string.c_str(), string.length(), 0);
	if (res == SOCKET_ERROR) {
		return 0;
	}
	return res;
}

std::string es_recv_string(SOCKET handle, int limit) {
	if (handle == INVALID_SOCKET) {
		WSASetLastError(WSA_INVALID_HANDLE);
		return std::string();
	}
	auto buffer = std::make_unique<char[]>(limit + 1);
	buffer[limit] = 0;

	auto res = recv(handle, buffer.get(), limit, 0);
	if (res == SOCKET_ERROR) {
		return std::string();
	}

	auto string = std::string(buffer.get());
	return std::move(string);
}

std::string es_recv_string(SOCKET handle, char terminator) {
	if (handle == INVALID_SOCKET) {
		WSASetLastError(WSA_INVALID_HANDLE);
		return std::string();
	}

	char buffer = 0;
	std::string string;
	do {
		auto res = recv(handle, &buffer, 1, 0);
		if (res == SOCKET_ERROR) {
			break;
		}
		if (buffer != terminator)
			string += buffer;
	} while (buffer != terminator);
	return std::move(string);
}

SOCKET es_listen(int port, int backlog) {
	SOCKET handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (handle == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}
	SOCKADDR_IN serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);

	auto res = bind(handle, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress));
	if (res == SOCKET_ERROR) {
		return INVALID_SOCKET;
	}
	listen(handle, backlog);
	return handle;
}

std::pair<SOCKADDR_IN, SOCKET> es_accept(SOCKET handle, bool wait) {
	SOCKADDR_IN clientAddress{};
	int len = sizeof(clientAddress);
	if (handle == INVALID_SOCKET) {
		WSASetLastError(WSA_INVALID_HANDLE);
		return std::pair(clientAddress, INVALID_SOCKET);
	}
	return std::pair(clientAddress, accept(handle, reinterpret_cast<SOCKADDR*>(&clientAddress), &len));
}