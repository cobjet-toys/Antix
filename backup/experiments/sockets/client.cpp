#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Pack_Unpack.h"

main()
{
    int sockfd, len;
    struct sockaddr_in address;

	int32_t roboId = 3;
	float32_t xpos = 10.01f;
	float32_t ypos = 20.03f;
	

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; /* Give the server address here when running on 2 different machines */
    address.sin_port = 9701;

    len = sizeof(address);

    if (connect (sockfd, (struct sockaddr *) &address, len) == -1) {
        printf("Error Client");
    }

	unsigned char buff[12];
	
	pack(buff, "lff", roboId, xpos, ypos);

    write(sockfd, &buff, 12);
    read(sockfd, &buff, 12);

	unpack(buff,"lff", &roboId, &xpos, &ypos);

    printf ("Robot %i now moves to position %f %f\n", roboId ,xpos, ypos);
    
    close (sockfd);
}
