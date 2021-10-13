#ifndef HEADERS_HPP
#define HEADERS_HPP
class Channel;
class Client;

// accept, bind, listen, socket, getaddrinfo
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>
#include <vector>
#include <map>
#include <ctime>

//Pour non blocking fcntl
#include "unistd.h"
#include "fcntl.h"

//poll
#include "poll.h"

//printf
#include <stdio.h>
#include <unistd.h> //write

//atoi
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <errno.h>


#include <exception>
#include <cstring>
#include <iostream>
#include <istream>
#include <sstream>



#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

void ft_error(int nb, std::vector<std::string> params, Client* client, Channel* channel, std::vector<Client *> client_list, std::vector<Channel*> channel_list);
void ft_reply(int nb, std::vector<std::string> params, Client* client, Channel* channel, std::vector<Client *> client_list, std::vector<Channel*> channel_list);

/*#define 
#define 
#define 
#define 
#define 
#define 
#define 
#define 
#define 
#define 
#define 
*/
#endif
