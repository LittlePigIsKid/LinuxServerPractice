#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

#define DEBUG 1
int
timeout_connect(const char* ip, int port, int time)
{
	int ret = 0;
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);
	
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(sockfd >= 0);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = time;
	socklen_t len = sizeof(timeout);
	ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, len);
	assert(ret != -1);

	ret = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
	#ifdef DEBUG
	printf("the ret of connect is %d\n", ret);
	#endif
	if(ret == -1) {
		#ifdef DEBUG
		printf("the function is return from connect\n");
		#endif
		if (errno == EINPROGRESS) {
			printf("connecting timeout, process timeout logic\n");
			return -1;
		} else {
			printf("errno occur when connecting to server\n");
			printf("%d\n", errno);
			return -1;
		}
	}
	return sockfd;
}

int
main(int argc, char* argv[])
{
	if (argc <= 3) {
		printf("usage: %s ip_address port_number time\n", argv[0]);
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);

	int sockfd = timeout_connect(ip, port, atoi(argv[3]));
	
	if (sockfd < 0) {
		return 1;
	}
	return 0;
}
