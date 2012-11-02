/*senderprog.c - a client, datagram*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/* the port users will be connecting to */

#define MYPORT 4950

int send_data(const char* name,char* mesg)
{

    int sockfd;

    /* connector’s address information */

    struct sockaddr_in their_addr;

    struct hostent *he;

    int numbytes;


    /* get the host info */

	if ((he = gethostbyname(name)) == NULL)
	{
		perror("Client-gethostbyname() error lol!");
		exit(1);
	}else

     

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)

    {

    perror("Client-socket() error lol!");

    exit(1);

    }

    else


     

    /* host byte order */

    their_addr.sin_family = AF_INET;

    /* short, network byte order */


    their_addr.sin_port = htons(MYPORT);

    their_addr.sin_addr = *((struct in_addr *)he->h_addr);

    /* zero the rest of the struct */

    memset(&(their_addr.sin_zero), '\0', 8);

     

    if((numbytes = sendto(sockfd, mesg, strlen(mesg), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1)

    {

    perror("Client-sendto() error lol!");

    exit(1);

    }


     


     

    if (close(sockfd) != 0)

	printf("Client-sockfd closing is failed!\n");


	return 0;

}
