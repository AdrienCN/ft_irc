
// accept, bind, listen, socket, getaddrinfo
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>
#include <vector>

//Pour non blocking fcntl
#include "unistd.h"
#include "fcntl.h"

//poll
#include "poll.h"

//printf
#include <stdio.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <errno.h>


#include <exception>
#include <cstring>
#include <iostream>
#include <istream>
