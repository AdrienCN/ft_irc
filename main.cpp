//C 
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>

//C++
#include <iostream>
#include <string>
#include <vector>

#define BUFFSIZE 32
/*
struct sockaddr_in {
    short int          sin_family;  // Address family
    unsigned short int sin_port;    // Port number
    struct in_addr     sin_addr;    // Internet address (IP)
    unsigned char      sin_zero[8]; // Same size as struct sockaddr
}; 
*/

int		main(void)
{
	int listensck;
	size_t	addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in myaddr;


	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(6667);
	myaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // INADDR_ANY -> myipaddress
	memset(&myaddr.sin_zero, 0, sizeof(8 * sizeof(unsigned char)));

	listensck = socket(AF_INET, SOCK_STREAM, 0);
	bind(listensck, (struct sockaddr *)&myaddr, addrlen);
	listen(listensck, 10);
	std::cout << "Listening to the fucking world" << std::endl;
	while(1)
	{
		int ret;
		int tmpsck;
		char buf[BUFFSIZE + 1];
		char *end;
		socklen_t			client_size = sizeof(struct sockaddr_in);
		struct sockaddr_in	clientaddr;
	
		memset(buf, 0, 10);
		tmpsck = accept(listensck, (struct sockaddr *)&clientaddr, &client_size);
		if (tmpsck > 0)
		{
			std::cout << "Hello New Client Nbr [" << tmpsck << "]" << std::endl;
		}
		else
		{
			std::cout << "No client" << std::endl;
			break;
		}
		ret = 0;
		end = NULL;
		while ((ret = recv(tmpsck, buf, BUFFSIZE, 0)) > 0)
		{
			std::cout << "ret = " << ret << std::endl;
			buf[BUFFSIZE] = '\0';
			std::cout << buf;
			memset(buf, 0, BUFFSIZE);
		}
			if (ret == -1)
			std::cout << "Error while recv()" << std::endl;
		if (ret == 0)
		{
			close(tmpsck);
			std::cout << "Communication over. Good bye" << std::endl;
		}
	}
	close(listensck);
	return (0);
}






