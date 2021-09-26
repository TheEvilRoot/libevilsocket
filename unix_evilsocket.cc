#include "evilsocket.h"

#include <memory>

errno_t es_init() { return 0; }

void es_deinit() { }

errno_t es_last_error() {
  return errno;
}

std::string es_error_message(errno_t error_code) {
  char* s = strerror(error_code);
  std::string string(s);
  return s;
}

socket_t es_connect(const std::string& host, int port, int keepalivems) {
  socket_t handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (handle == INVALID_SOCKET) {
    return INVALID_SOCKET;
  }

  address_t serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(host.c_str());
  serverAddress.sin_port = htons(port);

  auto res = connect(handle, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
  if (res == 0)
    return handle;
  return INVALID_SOCKET;
}

void es_close(socket_t handle) {
  if (handle != INVALID_SOCKET)
    close(handle);
}

size_t es_write_string(socket_t handle, const std::string& string) {
  if (handle == INVALID_SOCKET) {
    errno = ERROR_INVALID_HANDLE;
    return 0;
  }
  if (string.length() == 0) {
    errno = 0;
    return 0;
  }
  auto res = send(handle, string.c_str(), string.length(), 0);
  if (res == -1) {
    return 0;
  }
  return res;
}

std::string es_recv_string(socket_t handle, int limit) {
  if (handle == INVALID_SOCKET) {
    errno = ERROR_INVALID_HANDLE;
    return std::string();
  }
  auto buffer = std::make_unique<char[]>(limit + 1);
  buffer[limit] = 0;

  auto res = recv(handle, buffer.get(), limit, 0);
  if (res == -1) {
    return std::string();
  }

  auto string = std::string(buffer.get());
  return std::move(string);
}

std::string es_recv_string(socket_t handle, char terminator) {
  if (handle == INVALID_SOCKET) {
    errno = ERROR_INVALID_HANDLE;
    return std::string();
  }

  char buffer = 0;
  std::string string;
  do {
    auto res = recv(handle, &buffer, 1, 0);
    if (res == -1) {
      break;
    }
    if (buffer != terminator)
      string += buffer;
  } while (buffer != terminator);
  return std::move(string);
}

socket_t es_listen(int port, int backlog) {
  socket_t handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (handle == INVALID_SOCKET) {
    return INVALID_SOCKET;
  }
  address_t serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);

  auto res = bind(handle, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
  if (res == -1) {
    return INVALID_SOCKET;
  }
  listen(handle, backlog);
  return handle;
}

std::pair<address_t, socket_t> es_accept(socket_t handle, bool wait) {
  address_t clientAddress{};
  socklen_t len = sizeof(clientAddress);
  if (handle == INVALID_SOCKET) {
    errno = ERROR_INVALID_HANDLE;
    return std::pair(clientAddress, INVALID_SOCKET);
  }
  return std::pair(clientAddress, accept(handle, reinterpret_cast<sockaddr*>(&clientAddress), &len));
}