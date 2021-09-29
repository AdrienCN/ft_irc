  #include "Server.hpp"
/*
struct addrinfo 
{
    int              ai_flags; //Lots of flag options AI_PASSIVE is ours
    int              ai_family;	  //Type of address (IP) AF_INET | AF_INET6 |AF_UNSPEC 
    int              ai_socktype; // Type of socket
    int              ai_protocol; //Type of protocol for socket 0 = anyprotocol
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
	struct addrinfo *ai_next;
};
*/
Server::Server(std::string port, std::string password) : _domain("NULL"), _port(port), _serv_info(NULL), _password(password)
{
	memset(&_hints, 0, sizeof(_hints));
	//Prepare hints sur la stack pour getaddrinfo()
	_hints.ai_family = AF_UNSPEC; //Accept IPv4 & IPv6
	_hints.ai_socktype = SOCK_STREAM; //Sock type
	_hints.ai_flags = AI_PASSIVE; //Puts my Ip as default + NULL in getaddrinfo
}

Server::~Server()
{
	std::cout << "Server destruction" << std::endl;
	//garder ?  ou creer une fonction Server.end() qui close tout proprement
	close(_socket);
	freeaddrinfo(_serv_info);
    return;
} 

int const & Server::getSocket() const
{
    return _socket;
}

struct addrinfo* Server::getServInfo() const
{
	return _serv_info;
}

std::string const & Server::getPort() const
{
	return _port;
}

std::string const & Server::getPassword() const
{
	return _password;
}

//On catchera une erreur de construction dans le main au start
void Server::init()
{
	int ret;
	
	// 1. Prepare for launch!
	//Rempli notre structure serv_info qui contient 
	//tout les parametres pour call socket(), bind(), listen()
	ret = getaddrinfo(NULL, "6667"/* _port.c_str()*/, &_hints, &_serv_info);
	if (ret != 0)
		throw Server::ExceptGetaddr();
	
	// 2. Get the FD
	//Creer ce qui sera notre socket d'ecoute
	//domain = type d'address = ai_family | type = socketype = ai_socktype |  protocole = ai_protocole
	_socket = socket(_serv_info->ai_family, _serv_info->ai_socktype /*| SOCK_NONBLOCK*/, _serv_info->ai_protocol);
	if (_socket == -1)
		throw Server::ExceptInit();
		/*
		 * Dit que la socket peut etre retutiliser si elle fail ou mal ferme ?
			ret = 1;
			setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret));
		*/

	//Pour passer notre socket en non blocking 
	fcntl(_socket, F_SETFL, O_NONBLOCK); // c'est la seule maniere dont on a le droit de l'utiliser

	//3. What port Am I on?
	//Lie notre socket avec un port
	if ((bind(_socket, _serv_info->ai_addr, _serv_info->ai_addrlen)) == -1)
		throw Server::ExceptInit();

	// 4. WIll somebody call me?
	// sockfd et backlog = numbers of connection allowed in the back queue
	if ((listen(_socket, MAX_CLIENT)) == -1)
		throw Server::ExceptInit();
	std::cout << "Server init success" << std::endl;
	std::cout << "Listening for clients ..." << std::endl;

}
