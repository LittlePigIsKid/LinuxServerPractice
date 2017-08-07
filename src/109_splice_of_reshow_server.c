#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<stdio.h>
/*
*i am not able to compile this program for the reason that i don't know how to fix the problem that the splice() was not declared implicate. 
*if anyono could compile this program, please let me know, thank u a lot.
*/
int
main (int argc, char* argv[])
{
	if (argc <= 2) {
		printf("usage : %s ip_address port_nmuber\n", argv[0]);
		return 1;
	}

	const char* ip = argv[1];
	int port = atoi(argv[2]);
	
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(sock >= 0);

	int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);
	
	ret = listen(sock, 5);
	assert(ret != -1);

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	int connfd = accept(sock, (struct sockaddr*) &client, &client_addrlength);
	if (connfd < 0) {
		printf("errno is: %d\n", errno);
	} else {
		int pipefd[2];
		assert(ret != -1);
		ret = pipe(pipefd);
		ret = splice(connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
		assert(ret != -1);
		ret = splice(pipefd[0], NULL, connfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
		close(connfd);
	}
	close(sock);
	return 0;
}
