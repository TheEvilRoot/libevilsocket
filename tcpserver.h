#ifndef _EVILSOCK_TCP_SERVER
#define _EVILSOCK_TCP_SERVER

#include "evilsocket.h"

#ifndef _EVILSOCK_TCP_CLIENT
class TcpClient;
#endif

class TcpServer {
public:
	int port;
	SOCKET handle = INVALID_SOCKET;
	errno_t error = 0;

	TcpServer(int port) : port{ port } {
		auto init_res = es_init();
		if (init_res != 0) {
			error = init_res;
			return;
		}
		auto socket = es_listen(port, 1);
		if (socket == INVALID_SOCKET) {
			error = es_last_error();
			es_close(socket);
			return;
		}
		error = 0;
		handle = socket;
	}

	TcpServer(const TcpServer& copy) = delete;

	TcpServer(TcpServer&& move) noexcept : port{ move.port }, handle { move.handle }, error{ move.error } {
		move.handle = INVALID_SOCKET;
		move.error = 0;
	}

	~TcpServer() {
		es_close(handle);
		es_deinit();
	}

	TcpServer& operator=(const TcpServer& copy) {
		throw std::exception("TcpServer cannot be copied");
	}

	TcpServer& operator=(TcpServer&& move) noexcept {
		if (this == &move) return *this;
		es_close(handle);
		handle = move.handle;
		error = move.error;
		port = move.port;
		move.handle = INVALID_SOCKET;
		move.error = 0;
		return *this;
	}

	TcpClient* accept_client(bool wait) {
		auto [address, client] = es_accept(handle, wait);
		if (client == INVALID_SOCKET) {
			error = es_last_error();
			return nullptr;
		}
		return new TcpClient(client);
	}

};

#endif // guard