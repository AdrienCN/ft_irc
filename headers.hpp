#ifndef HEADERS_HPP
#define HEADERS_HPP

// accept, bind, listen, socket, getaddrinfo
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>
#include <vector>
#include <map>

//Pour non blocking fcntl
#include "unistd.h"
#include "fcntl.h"

//poll
#include "poll.h"

//printf
#include <stdio.h>
#include <unistd.h> //write

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
#define RESET "\033[0m"
void print_vector(std::vector<std::string> inputs);
#endif
