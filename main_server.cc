#include "tcpclient.h"
#include "tcpserver.h"

#include <cstdio>


int main() {
	TcpServer server{ 9000 };
	printf("error: %ld\n", server.error);
	printf("socket: %08x\n", server.handle);
	printf("message: %s\n", es_error_message(server.error).c_str());
	
	auto client = server.accept_client(true);
	printf("client: %ld\n", client);
	printf("error: %ld\n", server.error);
	if (client == nullptr)
		return 1;

	auto resp = client->recv_string('\n');
	printf("resp: %ld %s\n", resp.length(), resp.c_str());
	printf("error: %ld\n", client->error);

	auto res = client->write_string("HTTP/1.1 200 OK\r\n");
	resp = client->recv_string('\n');
	printf("res: %ld\n", res);
	printf("error: %ld\n", client->error);

	res = client->write_string("Server: EvilSocket/1.0\r\n");
	resp = client->recv_string('\n');
	printf("res: %ld\n", res);
	printf("error: %ld\n", client->error);
}