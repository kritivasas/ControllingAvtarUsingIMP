#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* the port users will be connecting to */
#define MYPORT 4950
#define MAXBUFLEN 500

int sockfd;
/* my address information */
struct sockaddr_in my_addr;
/* connector’s address information */
struct sockaddr_in their_addr;
int addr_len;
int numbytes;
char buf[MAXBUFLEN];

void init_recv()
{

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Server-socket() sockfd error lol!");
		exit(1);
	}else
	    printf("Server-socket() sockfd is OK...\n");

	/* host byte order */
	my_addr.sin_family = AF_INET;

	/* short, network byte order */
	my_addr.sin_port = htons(MYPORT);

	/* automatically fill with my IP */
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/* zero the rest of the struct */
	memset(&(my_addr.sin_zero), '\0', 8);

	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
	    perror("Server-bind() error lol!");
	    exit(1);
	}else
	    printf("Server-bind() is OK...\n");
	

	addr_len = sizeof(struct sockaddr);

}


void recv_data(float *a, float *b, float *c, float *d)
{
 
	if((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, (socklen_t *)&addr_len)) == -1)
	{
	    perror("Server-recvfrom() error lol!");
	    /*If something wrong, just exit lol...*/
	    exit(1);
	}else{
	    printf("Server-Waiting and listening...\n");
	    printf("Server-recvfrom() is OK...\n");
	}

	printf("Server-Got packet from %s\n", inet_ntoa(their_addr.sin_addr));
	printf("Server-Packet is %d bytes long\n", numbytes);
	buf[numbytes] = ' ';
//	printf("Server-Packet contains \"%s\"\n", buf);

	sscanf(buf,"%f %f %f %f", a, b, c, d);
	printf("%f %f %f %f\n", *a, *b, *c, *d);
	return;
}

/*
int main()
{
	init_recv();
	for(int i=0;i<100;i++) printf("%f\n",recv_data());
	return 0;
}*/
