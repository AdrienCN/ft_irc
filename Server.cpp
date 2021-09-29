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
Server::Server(std::string port, std::string password) : _domain("NULL"), _port(port), _serv_info(NULL), _password(password), _nbClients(0)
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
	close(_server_socket);
	freeaddrinfo(_serv_info);
    return;
} 

int const & Server::getSocket() const
{
    return _server_socket;
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
	_server_socket = socket(_serv_info->ai_family, _serv_info->ai_socktype /*| SOCK_NONBLOCK*/, _serv_info->ai_protocol);
	if (_server_socket == -1)
		throw Server::ExceptErrno();
		
		 //Dit que la socket peut etre retutiliser si elle fail ou mal ferme ?
			ret = 1;
			setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret));

	//Pour passer notre socket en non blocking, portable sur mac
	if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1) // c'est la seule maniere dont on a le droit de l'utiliser
		throw Server::ExceptErrno();

	//3. What port Am I on?
	//Lie notre socket avec un port
	if ((bind(_server_socket, _serv_info->ai_addr, _serv_info->ai_addrlen)) == -1)
		throw Server::ExceptErrno();

	// 4. WIll somebody call me?
	// sockfd et backlog = numbers of connection allowed in the back queue
	if ((listen(_server_socket, MAX_CLIENT)) == -1)
		throw Server::ExceptErrno();
	std::cout << "Server init success" << std::endl;
	std::cout << "Listening for clients ..." << std::endl;

	// Set up la struct poll
	_poll.fd = _server_socket;
	_poll.events = POLLIN;                                           
	_fds.push_back(_poll);
		std::cout << "fd = " << _poll.fd << std::endl;

}

void Server::poll_add_client(Client const& new_client)
{
	std::vector<struct pollfd>::iterator it = _fds.begin();
	std::vector<struct pollfd>::iterator ite = _fds.end();
	while (it != ite)
	{
		if (new_client.getSocket() == it->fd)
		{
			std::cout << "Error : Server : adding poll_fds client" << std::endl;
			return;
		}
		it++;
	}
	_fds.push_back(new_client.getPoll());
		return;
}

void	Server::poll_remove_client(int const & fd)
{
	std::vector<struct pollfd>::iterator it = _fds.begin();
	std::vector<struct pollfd>::iterator ite = _fds.end();

	while(it != ite)
	{

		//if (old_client.getSocket() == it->fd)
		if (fd == it->fd)
		{
			_fds.erase(it);
			_nbClients--;
			return;
		}
		it++;
	}
	return;
}


void	Server::run() 
{	
	while (1)
	{
		std::vector<pollfd>::iterator it = _fds.begin();
		int poll_count = poll(&(*it), _nbClients + 1, 2000);
		if (poll_count == -1)
			throw Server::ExceptErrno();

		std::vector<pollfd>::iterator itb = _fds.begin();
	
		std::vector<pollfd>::iterator ite = _fds.end();

		//for (int i = 0; i <= _nbClients; i++)
		while (itb != ite)
		{
			 if ((*itb).revents & POLLHUP)
			{
				if ((*itb).fd != _server_socket)
				{					
					std::cout << "Client " << (*itb).fd << " has disconnected" << std::endl;
					this->poll_remove_client((*itb).fd);
					break;
				}
			}
			//Event est un POLLIN
			if ((*itb).revents & POLLIN)
			{
				//Je suis le serveur
				if ((*itb).fd == _server_socket)
				{	
					this->addClient();
					std::cout << "New client added" <<std::endl;
				}
				//Je suis un client
				else
				{
					//renomme all_user par client_list ? 
					//this->_all_clients[x].read_data();
				}
			}
			// Je suis un POLLOUT
			
			itb++;
		}//if (fd_client == -1)
		std::cout << "time out" << std::endl;
		// error 
	}
}

void	Server::addClient()
{

	//struct sockaddr ?? 
	struct sockaddr_storage client_addr;
	socklen_t		addrlen;
	int				socket;

	addrlen = sizeof(client_addr);
	socket = accept(_server_socket, (struct sockaddr *)&client_addr, &addrlen);
	if (socket == -1)
	{
		std::cout << "Pbm socket function" << std::endl;
		throw Server::ExceptErrno();
	}

	if (_nbClients == MAX_CLIENT)
	{
		std::cout << "Error : Client number already maximum" << std::endl;
		close(socket);
		return;
	}

	Client*			new_client = new Client();
	new_client->init(socket);
	this->poll_add_client(*new_client);
	_all_clients.push_back(new_client);
	_nbClients++;
	}