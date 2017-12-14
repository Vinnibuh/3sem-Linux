#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define handle_error(msg)\
	do {perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
	uint16_t bind_port;
	int flag = 0;
	switch (argc) {
		case 2 :
			bind_port = (uint16_t)atoi(argv[1]);
			break;
		case 3 :
			bind_port = (uint16_t)atoi(argv[1]);
			if (strcmp(argv[2], "-c") == 0)
				flag = 1;
			else handle_error("Wrong mode specifier");
			break;
		case 5 :
			bind_port = (uint16_t)atoi(argv[1]);
			if (strcmp(argv[2], "-f") == 0) 
				flag = 2;
			else handle_error("Wrong mode specifier");
			break;
		default :
			bind_port = 7542;
			break;
	}
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) 
		handle_error("Socket create");
	struct sockaddr_in local_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(bind_port),
		.sin_addr.s_addr = htonl(INADDR_ANY)
	};
	if (bind(sock, (const struct sockaddr *)&local_addr, sizeof(local_addr)))
		handle_error("bind");
	while(1) {
		struct sockaddr_in remote_addr;
		socklen_t remote_addr_size = sizeof(remote_addr);
		char data[2048];
		ssize_t data_len = recvfrom(sock, data, sizeof(data), 0, (struct sockaddr *)&remote_addr, &remote_addr_size);
		if (data_len < 0) {
			perror("recv");
			break;
		}
		char remote_addr_str[INET_ADDRSTRLEN];
		inet_ntop(remote_addr.sin_family, &remote_addr.sin_addr, remote_addr_str, sizeof(remote_addr_str));
		printf("received packet from %s:%d, content: '%.*s'\n", remote_addr_str, ntohs(remote_addr.sin_port), (int) data_len, data);
		if (flag != 1) {
			char reply[1024];
			snprintf(reply, sizeof(reply), "Got your request of %zd bytes!\n", data_len);
			sendto(sock, reply, strlen(reply), 0, (const struct sockaddr *)&remote_addr, remote_addr_size);
		}
		if (flag == 2) {
			uint16_t frwd_port = (uint16_t)atoi(argv[4]);
			struct in_addr in_frwd_addr;
			inet_aton(argv[3], &in_frwd_addr);
			struct sockaddr_in frwd_addr = {
				.sin_family = AF_INET,
				.sin_port = htons(frwd_port),
				.sin_addr = in_frwd_addr
			};
			socklen_t frwd_addr_size = sizeof(frwd_addr);
			printf("sometnihg\n");
			ssize_t size = sendto(sock, data, strlen(data), 0, (const struct sockaddr *)&frwd_addr, frwd_addr_size);
			if (size != (ssize_t)(strlen(data))) {
				if (size == -1)
					printf("Fatal error in forwarding");
				else
					printf("Troubles in forwarding");
			} else
				printf("Forwarding is successful\n");
		}
	}
	close(sock);
	return 0;
}


