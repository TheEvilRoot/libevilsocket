#ifndef _EVILSOCKET_H
#define _EVILSOCKET_H

#if defined(WIN32)
#define EVILSOCK_WIN
#define EVILSOCK_TYPE "WINDOWS"
#pragma comment(lib, "ws2_32")
#elif defined(__APPLE__)
#define EVILSOCK_APPLE
#define EVILSOCK_POSIX
#define EVILSOCK_TYPE "POSIX"
#else
#define EVILSOCK_UNIX
#define EVILSOCK_POSIX
#define EVILSOCK_TYPE "POSIX"
#endif

#include <cerrno>
#include <string>
#include <cstdint>

#ifdef EVILSOCK_WIN
#include <WinSock2.h>
#endif

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
SOCKET es_connect(const std::string& host, int port, int keepalivems);

/**
* Close socket handle if it's valid.
* 
* Client and Server.
**/
void es_close(SOCKET handle);

/**
* Write string to socket handle if it's valid.
* 
* Client and Server.
* 
* @return 0 and set error on failure, number of bytes total written on success
**/
size_t es_write_string(SOCKET handle, std::string string);

/**
* Read string from socket with specified limit.
* 
* Client and Server.
* 
* @return empty string and set error on failure, string with data on success
**/
std::string es_recv_string(SOCKET handle, int limit);

/**
* Read string from socket with specified terminator.
*
* Client and Server.
*
* @return empty string and set error on failure, string with data on success
**/
std::string es_recv_string(SOCKET handle, char terminator);

SOCKET es_listen(int port, int backlog);

std::pair<SOCKADDR_IN, SOCKET> es_accept(SOCKET handle, bool wait);


#endif //guard