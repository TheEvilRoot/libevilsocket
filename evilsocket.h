#ifndef _EVILSOCKET_H
#define _EVILSOCKET_H

#if defined(WIN32)
#define EVILSOCK_WIN
#define EVILSOCK_TYPE "WINDOWS"
#define ERROR_INVALID_HANDLE WSA_INVALID_HANDLE
#elif defined(__APPLE__)
#define EVILSOCK_APPLE
#define EVILSOCK_POSIX
#define EVILSOCK_TYPE "POSIX"
#define INVALID_SOCKET ((socket_t) -1)
#define ERROR_INVALID_HANDLE EBADF
#else
#define EVILSOCK_UNIX
#define EVILSOCK_POSIX
#define EVILSOCK_TYPE "POSIX"
#define INVALID_SOCKET ((socket_t) -1)
#define ERROR_INVALID_HANDLE EBADF
#endif

#ifdef EVILSOCK_WIN
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
typedef SOCKET socket_t;
typedef SOCKADDR_IN address_t;
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
typedef int socket_t;
typedef sockaddr_in address_t;
#endif

#include <cerrno>
#include <string>
#include <cstdint>
#include <utility>

class SocketException: public std::exception {
 private:
  std::string message;
 public:
  explicit SocketException(std::string  message): message{std::move(message)} {}

  [[nodiscard]] const char * what() const noexcept override {
    return message.c_str();
  }
};

/**
* Phase 0. Initiate socket library.
* 
* Server and Client.
* 
* @return 0 on success, anything else on error
**/
errno_t es_init();

/**
* Last phase. Deinitialize socket library.
* 
* Server and Client.
**/
void es_deinit();

/**
* Get error from last operation performed.
* 
* Client and Server.
* 
* @return error integer
**/
errno_t es_last_error();

/**
* Localize integer error to string
* 
* Client and Server.
* 
* @return string with message
**/
std::string es_error_message(errno_t error_code);

/**
* Connect to server at host:port via TCP protocol.
* 
* Client.
* 
* @return nullptr and set errno on failure, valid socket handle on success.
**/
socket_t es_connect(const std::string& host, int port, int keepalivesec);

/**
* Close socket handle if it's valid.
* 
* Client and Server.
**/
void es_close(socket_t handle);

/**
* Write string to socket handle if it's valid.
* 
* Client and Server.
* 
* @return 0 and set error on failure, number of bytes total written on success
**/
size_t es_write_string(socket_t handle, const std::string& string);

/**
* Read string from socket with specified limit.
* 
* Client and Server.
* 
* @return empty string and set error on failure, string with data on success
**/
std::string es_recv_string(socket_t handle, int limit);

/**
* Read string from socket with specified terminator.
*
* Client and Server.
*
* @return empty string and set error on failure, string with data on success
**/
std::string es_recv_string(socket_t handle, char terminator);

/**
 * Create a server socket, bint it to the port and start listening with backlog
 *
 * @param port server port to bind
 * @param backlog listen backlog
 * @return server socket handle
 */
socket_t es_listen(int port, int backlog);

/**
 * If wait is true, wait until client connects, accept connection and return address and client handle
 * If wait is false, return invalid socket value and empty address if there's no connection request from client or client
 * if there's one.
 *
 * @param handle server socket handle
 * @param wait need to wait client or return right after call
 * @return either empty address and invalid socket value  or valid client address and client socket handle if client has been accepted
 */
std::pair<address_t, socket_t> es_accept(socket_t handle, int keepalivesec, bool wait);


#endif //guard