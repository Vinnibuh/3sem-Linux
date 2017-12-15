#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define handle_error(msg) \
	do {perror(msg); exit(EXIT_FAILURE); } while (0)


int main(int argc, char *argv[]) {
	struct in_addr serv_addr;
	switch (argc) {
		case 3 :
			inet_aton(argv[1], &serv_addr);
			break;
		default :
			printf("Wrong number of arguments\n");
			return(-1);
	}
	int sock = socket( AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		handle_error("socket_create");
	struct sockaddr_in remote_addr = {
	       .sin_family = AF_INET,
	       .sin_port = htons(80),
	       .sin_addr = serv_addr
	};
	int err = connect(sock, (const struct sockaddr *)&remote_addr, sizeof(remote_addr));
	if (err == -1) 
		handle_error("connect");
	int exit_code = 0;
       	char *request =(char *) malloc(sizeof(char)*100);
	strcpy(request, "GET / HTTP1.1\r\n"
		"Host: ");
	printf("1\n");
	strcat(request, argv[2]);
	printf("2\n");
	strcat(request, "\r\nConnection: close\r\n\r\n");
		//"judge.mipt.ru\r\n"
		//"Connection: close\r\n"
		//"\r\n";
	size_t reg_len = strlen((const char *)request);
	printf("dfs\n");
	if ( (ssize_t)reg_len != send(sock, (const char *)request, reg_len, MSG_NOSIGNAL)) {
		perror("send\n");
		close(sock);
		return 1;
	}
	while (1) {
		char reply[4096];
		ssize_t res = recv(sock, reply, sizeof(reply), MSG_NOSIGNAL);
		if (res < 0) {
			perror("recv\n");
			exit_code = 1;
			break;
		}
		if (res == 0)
			break;
		write(fileno(stdout), reply, res);
	}
	free(request);
	close(sock);
	return exit_code;
}

