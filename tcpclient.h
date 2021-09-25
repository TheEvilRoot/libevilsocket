#ifndef _EVILSOCK_TCP_CLIENT
#define _EVILSOCK_TCP_CLIENT

#include "evilsocket.h"

class TcpClient {
public:
	SOCKET handle = INVALID_SOCKET;
	errno_t error = 0;
	bool closed = false;

	TcpClient(SOCKET handle) : handle{ handle }, error{ 0 } {
		if (handle == INVALID_SOCKET) {
			set_error(WSA_INVALID_HANDLE);
		}
	}

	TcpClient(const std::string& host, int port) {
		auto init_res = es_init();
		if (init_res != 0) {
			set_error(init_res);
			return;
		}
		auto socket = es_connect(host, port, 60000);
		if (socket == INVALID_SOCKET) {
			set_error(es_last_error());
			es_close(socket);
			return;
		}
		error = 0;
		closed = false;
		handle = socket;
	}

	TcpClient(const TcpClient& copy) = delete;

	TcpClient(TcpClient&& move) noexcept : handle{ move.handle }, error{ move.error } {
		move.handle = INVALID_SOCKET;
		move.error = 0;
	}

	~TcpClient() {
		es_close(handle);
		es_deinit();
	}

	TcpClient& operator=(const TcpClient& copy) {
		throw std::exception("TcpClient cannot be copied");
	}

	TcpClient& operator=(TcpClient&& move) noexcept {
		if (this == &move) return *this;
		es_close(handle);
		handle = move.handle;
		error = move.error;
		move.handle = INVALID_SOCKET;
		move.error = 0;
		move.closed = false;
		return *this;
	}

	size_t write_string(const std::string& string) {
		auto res = es_write_string(handle, string);
		if (res == 0) {
			set_error(es_last_error());
		}
		return res;
	}

	std::string recv_string(int limit) {
		auto res = es_recv_string(handle, limit);
		if (res.empty()) {
			set_error(es_last_error());
		}
		return res;
	}

	std::string recv_string(char terminator) {
		auto res = es_recv_string(handle, terminator);
		if (res.empty()) {
			set_error(es_last_error());
		}
		return res;
	}

private:
	void set_error(errno_t er) {
		error = er;
		if (er == 10054) {
			closed = true;
		}
	}

};

#endif // guard