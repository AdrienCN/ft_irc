#include "Server.hpp"
#include <iostream>
#include <fstream>

Server::Server(std::string port, std::string password) : _domain("NULL"), _port(port), _serv_info(NULL), _password(password), _server_name("irc.irc90s.com"), _server_ipaddress("127.0.0.1"), _server_creation_date(""), _nbClients(0), _command_book(password, "", _server_ipaddress, _server_creation_date) 
{
	time_t	raw_time;
	time(&raw_time);
	_server_creation_date = ctime(&raw_time);
	_server_creation_date.erase((_server_creation_date.end() - 1));
	_command_book.setServerCreationDate(_server_creation_date);
	memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_UNSPEC; //Accept IPv4 & IPv5
	_hints.ai_socktype = SOCK_STREAM; //Sock type
	_hints.ai_flags = AI_PASSIVE; //Puts my Ip as default + NULL in getaddrinfo
	std::ifstream image("pic.txt");
	if (image.is_open())
	{
		while(image.good())
		{
			std::string tmp;
			std::getline(image, tmp);
			tmp += "\n";
			_pokemon += tmp;
		}
	}
	else
		_pokemon = "";
	std::cout << _pokemon << std::endl;
}

Server::~Server()
{
	std::cout << "<< Server destruction >>" << std::endl;

	std::vector<Client*>::iterator it = _all_clients.begin();
	std::vector<Client*>::iterator ite = _all_clients.end();
	while (it != ite)
	{
		delete *it;
		it++;
	}
	_all_clients.clear();

	std::vector<Channel*>::iterator itb = _all_channels.begin();
	std::vector<Channel*>::iterator itbe = _all_channels.end();
	while (itb != itbe)
	{
		delete *itb;
		itb++;
	}
	_all_channels.clear();

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

std::string const & Server::getServerName() const
{
	return this->_server_name;
}

std::string const & Server::getServerIpaddress() const
{
	return this->_server_ipaddress;
}

std::string const & Server::getServerCreationDate() const
{
	return this->_server_creation_date;
}

void  Server::setServerName(std::string const & src)
{
	this->_server_name = src;
}

void  Server::setServerIpaddress(std::string const & src)
{
	this->_server_ipaddress = src;
}

void  Server::setServerCreationDate(std::string const & src)
{
	this->_server_creation_date = src;
}

void Server::init()
{
	int ret;

	// 1. Prepare for launch!
	//Rempli notre structure serv_info qui contient 
	//tout les parametres pour call socket(), bind(), listen()
	ret = getaddrinfo("localhost", this->_port.c_str(), &_hints, &_serv_info);
	if (ret != 0)
		throw Server::ExceptGetaddr();

	void	*addr;
	char	ip_final[INET6_ADDRSTRLEN];
	std::string str;

	if (_serv_info->ai_family == AF_INET) 
	{ 
		// IPV4
		struct sockaddr_in *tmp = (struct sockaddr_in*)_serv_info->ai_addr;
		addr = &(tmp->sin_addr);
	} 
	else 
	{
		// IPV6
		struct sockaddr_in6 *tmp = (struct sockaddr_in6*)_serv_info->ai_addr;
		addr = &(tmp->sin6_addr);
	}
	if (inet_ntop(_serv_info->ai_family, addr, ip_final, sizeof(ip_final)) == NULL)
		throw Server::ExceptErrno();
	else
	{
		_server_ipaddress = ip_final;
		_command_book.setServerIpaddress(ip_final);
	}

	// 2. Get the FD
	//Creer ce qui sera notre socket d'ecoute
	//domain = type d'address = ai_family | type = socketype = ai_socktype |  protocole = ai_protocole
	_server_socket = socket(_serv_info->ai_family, _serv_info->ai_socktype , _serv_info->ai_protocol);
	if (_server_socket == -1)
		throw Server::ExceptErrno();
	ret = 1;
	if ((setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret))) == -1)
	{
		throw Server::ExceptErrno();
	}

	//Pour passer notre socket en non blocking, portable sur mac
	if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1)
		throw Server::ExceptErrno();

	//3. What port Am I on?
	//Lie notre socket avec un port
	if ((bind(_server_socket, _serv_info->ai_addr, _serv_info->ai_addrlen)) == -1)
		throw Server::ExceptErrno();

	// 4. WIll somebody call me?
	// sockfd et backlog = numbers of connection allowed in the back queue
	if ((listen(_server_socket, MAX_CLIENT)) == -1)
		throw Server::ExceptErrno();
	std::cout << "Server init success!!" << std::endl;
	std::cout << "Listening for clients ..." << std::endl;

	_poll.fd = _server_socket;
	_poll.events = POLLIN;                                           
}

/*
   void	Server::pollInfo(std::vector<struct pollfd> const & src)
   {
   std::vector<struct pollfd>::const_iterator it = src.begin();
   while (it != src.end())
   {
   std::cout << "FD : " << it->fd  << " | EVENTS: " << it->events << " | REVENTS: "<< it->revents << std::endl;
   it++;
   }
   return ;
   }
   */

void	Server::run() 
{	
	std::vector<struct pollfd>		tmp;
	//Useless ?
	// tmp.push_back(this->_poll);
	while (1)
	{
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, signal_handler);

		//std::cout << YELLOW << "FROM MAIN SERVER" << RESET << std::endl;
		//print_client_list(_all_clients);
		//print_channel_list(_all_channels);

		tmp.push_back(this->_poll);
		for (std::vector<Client*>::iterator it = _all_clients.begin(); it != _all_clients.end(); it++)
		{
			tmp.push_back((*it)->getPoll());
		}

		std::vector<pollfd>::iterator it = tmp.begin();
		//		std::cout << "Starting new poll ..." << std::endl;
		int poll_count = poll(&(*it), _nbClients + 1, -1);
		if (poll_count == -1)
			throw Server::ExceptErrno();
		//	this->pollInfo(tmp);

		std::vector<pollfd>::iterator itb = tmp.begin();
		std::vector<pollfd>::iterator ite = tmp.end();

		while (itb != ite)
		{
			if ((*itb).revents & POLLHUP)
			{
				if ((*itb).fd != _server_socket)
					this->removeClient((*itb).fd);
			}
			else if ((*itb).revents & POLLIN)
			{
				if ((*itb).fd == _server_socket) ///Je suis le serveur
				{	
					if (_nbClients == MAX_CLIENT)
						this->refuseClient();
					else
						this->addClient();
				}
				else //Je suis un clien
				{
					Client *client = this->find_client_from_fd((*itb).fd);
					if (client != NULL) 
					{
						client->recvMessage();
						if (client->getMessageStatus() == DISCONNECT)
							this->removeClient(client->getSocket());
						else if (client->getMessageStatus() == COMPLETE) //CLRC received
						{
							client->analyzeMessage();
							if (client->isRegistered() == false)
								welcomeClient(client);
							else
								_command_book.find_command(client->getCommand().front(), client, _all_clients, &_all_channels);
							client->clearMessage();
							client->clearCommand();
							client->clearCommand();
							this->find_to_kill();
						}
					}
				}
			}
			itb++;
		}
		tmp.clear();
	}
}

void	Server::find_to_kill()
{
	std::vector<Client*>::iterator it = _all_clients.begin();
	std::vector<Client*>::iterator ite = _all_clients.end();
	//std::cout << YELLOW << "Hello from FIND_KILL" << RESET << std::endl;
	while (it != ite)
	{
		if ((*it)->getMessageStatus() == DISCONNECT)
		{
			close((*it)->getSocket());
			delete(*it);
			_all_clients.erase(it);
			_nbClients--;
			//std::cout<< YELLOW  << "KILL SUCCESS" << RESET << std::endl;
			return;
		}
		it++;
	}
}

void	Server::refuseClient()
{
	struct sockaddr_storage client_addr;
	socklen_t		addrlen;
	int				socket;

	addrlen = sizeof(client_addr);
	socket = accept(_server_socket, (struct sockaddr *)&client_addr, &addrlen);
	if (socket == -1)
		throw Server::ExceptErrno();
	if (fcntl(socket, F_SETFL,  O_NONBLOCK) == -1)
		throw Server::ExceptErrno();

	Client*	new_client = new Client(_server_name, _server_ipaddress, _server_creation_date);
	new_client->init(socket);
	std::cout << RED << "ERROR : refuseClient : Client already maximum" << RESET << std::endl;
	ft_reply(RPL_BOUNCE,  new_client, NULL, "");
	close(socket);
	delete new_client;
}

void	Server::addClient()
{
	struct sockaddr_storage client_addr;
	socklen_t				addrlen;
	int						socket;

	addrlen = sizeof(client_addr);
	socket = accept(_server_socket, (struct sockaddr *)&client_addr, &addrlen);
	if (socket == -1)
		throw Server::ExceptErrno();
	if (fcntl(socket, F_SETFL,  O_NONBLOCK) == -1)
		throw Server::ExceptErrno();

	Client*	new_client = new Client(_server_name, _server_ipaddress, _server_creation_date);
	new_client->init(socket);
	_all_clients.push_back(new_client);
	_nbClients++;
	std::cout << YELLOW << "New client not registered yet" << RESET << std::endl;
	return;
}

void	Server::removeClient(int const & fd)
{
	std::vector<Client*>::iterator it = _all_clients.begin();
	std::vector<Client*>::iterator ite = _all_clients.end();

	Client* tmp = find_client_from_fd(fd);
	if (tmp == NULL)
		return;
	while (it != ite)
	{
		if (*it == tmp)
		{
			close((*it)->getSocket());
			delete (*it);
			_all_clients.erase(it);
			break;
		}
		it++;
	}
	_nbClients--;
	return;	
}

Client* Server::find_client_from_fd(int fd)
{
	std::vector<Client*>::iterator it = _all_clients.begin();
	std::vector<Client*>::iterator ite = _all_clients.end();

	while (it !=  ite)
	{
		if ((*it)->getSocket() == fd)
			return *it;
		it++;
	}
	return NULL;
}
/*
   static void	ft_registration_failed(Client *client)
   {
   std::string tmp("Registration is not complete : Please enter the following command(s):\n");
   if (client->getRegNick() == false)
   tmp += "/NICK <nickname>\n";
   if (client->getRegUser() == false)
   tmp += "/USER <username> <mode> <unused> :<realname>\n";
   tmp += "\r\n";
   send(client->getSocket(), tmp.c_str(), tmp.size(), 0);
   }
   */

void	Server::welcomeClient(Client *client)
{
	std::vector<std::string> full_command(client->getCommand());
	std::vector<std::string> tmp = full_command;
	while (full_command.empty() == false)
	{

		_command_book.find_command(full_command.front(), client, _all_clients, &_all_channels);
		full_command.erase(full_command.begin());
	}
	if (client->getRegNick() == true && client->getRegUser() == true)
	{
		std::cout << GREEN << "********REGISTRATION SUCCESS for " << client->getNickname() << "**********" << RESET << std::endl;
		client->setRegistration(true);
		ft_reply("1", client, NULL, "");
		ft_reply("2", client, NULL, "");
		ft_reply("3", client, NULL, "");
		ft_reply("4", client, NULL, "");
		ft_reply(RPL_CUSTOMMOTD, client, NULL, _pokemon);
		if (client->getRegPass() == true)
		{
			if (client->getPassword() != this->_password)
			{
				ft_error(ERR_PASSWDMISMATCH, client, NULL, "");
				client->setRegPass(false);
			}
		}
	}
	//Evite le msg d'erreur si CAP ou PASS
	//
	////Checker si les commandes NICK et USER sont presente dans les cmd
	/*while (tmp.empty() == false)
	  {
	//Si l'une des deux ou les deux sont presente checker l'etat de l'enregistrement
	if (std::strstr(tmp.front().c_str(), "USER") || std::strstr(tmp.front().c_str(), "NICK"))
	{
	if (client->getRegNick() == true && client->getRegUser() == true)
	{
	std::cout << GREEN << "********REGISTRATION SUCCESS for " << client->getNickname() << "**********" << RESET << std::endl;
	client->setRegistration(true);
	ft_reply("1", client, NULL, "");
	ft_reply("2", client, NULL, "");
	ft_reply("3", client, NULL, "");
	ft_reply("4", client, NULL, "");
	if (client->getRegPass() == true)
	{
	if (client->getPassword() != this->_password)
	{
	ft_error(ERR_PASSWDMISMATCH, client, NULL, "");
	client->setRegPass(false);
	}
	}
	}
	else
	return;
	}
	tmp.erase(tmp.begin());
	}
	*/
}
