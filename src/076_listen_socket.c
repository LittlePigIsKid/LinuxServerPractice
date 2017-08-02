#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<stdio.h>
#include<libgen.h>
/*
*this program is used to test the second parameter of function listener(), 
*the backlog has different means on different os.
*the backlog is used to constraint the queue of sys_rcvd and the queue of the established.
*see more on  udp.
*/
#define bool int
#define false (-1)
#define true (1)

static bool stop = false;

static void handle_term(int sig)
{
	stop = true;
}

int
main(int argc, char* argv[])
{
	signal(SIGTERM, handle_term);
	if (argc <= 3)
	{
		printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);
	int backlog = atoi(argv[3]);
	
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(sock >= 0);

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	int ret = bind(sock, (struct sockaddr*) &address, sizeof(address));
	assert(ret != -1);

//the typical three-times shanking-hands is happening here.
//after the shanking-hands, the connection is moved to establish-queue.
//but as a matter of fact, the accept function is not promised the connection 
//is always staying established .
//see more on <<Linux高性能服务器编程>> Page 81.

	ret = listen(sock, backlog);
	assert(ret != -1);

	while( false == stop ) {
		sleep(1);
	}
	close(sock);
	return 0;
}
