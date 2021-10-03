#include "tcpclient.h"

#include <cstdio>


int main() {
	TcpClient client{ std::string("127.0.0.1"), 90000, 60 };
	printf("error: %ld\n", client.error);
	printf("socket: %08x\n", client.handle);
	printf("message: %s\n", es_error_message(client.error).c_str());
	auto res = client.write_string("GET / HTTP/1.1\r\n\r\n");
	printf("res: %ld\n", res);
	printf("error: %ld\n", client.error);
	auto resp = client.recv_string('\n');
	printf("resp: %ld %s\n", resp.length(), resp.c_str());
	printf("error: %ld\n", client.error);
    getchar();
    resp = client.recv_string('\n');
    printf("resp: %ld %s\n", resp.length(), resp.c_str());
    printf("error: %ld\n", client.error);
    getchar();
    resp = client.recv_string('\n');
    printf("resp: %ld %s\n", resp.length(), resp.c_str());
    printf("error: %ld\n", client.error);
}