#include "Client.hpp"

Client::Client(std::string server_name, std::string server_ipaddress, std::string server_creation_date): _message_status(DISCONNECT),  _hostname("defaultconstructorHOSTNAMEvalue"), _username("Default_Username"), _nickname("Default_Nickname"), _realname("Default_realname"), _server_name(server_name), _server_ipaddress(server_ipaddress), _server_creation_date(server_creation_date),  _away_message(""), _registration_status(false), _reg_pass(false), _reg_nick(false), _reg_user(false), _away(false), _oper(false)

{
    return;
}

Client::~Client()
{
    return;
}

void Client::present()
{
    std::cout << "CLIENT: user = " << getUsername() << " | nickname = " << getNickname() << std::endl; 
	std::cout << "My channels are : ";
	std::vector<Channel*>::iterator it = _channels.begin();
	std::vector<Channel*>::iterator ite = _channels.end();
	while (it != ite)
	{
		(*it)->present();
		it++;	
	}
	std::cout << std::endl;
	
}

// GETTERS
bool const &		Client::getOper() const
{
	return this->_oper;
}

bool const &					 Client::getRegPass() const
{
	return this->_reg_pass;
}

bool const &					 Client::getRegNick() const
{
	return this->_reg_nick;
}

bool const &					 Client::getRegUser() const
{
	return this->_reg_user;
}

std::string const &				Client::getHostname() const
{
	return this->_hostname;
}

std::vector<std::string> const & Client::getCommand() const
{
	return this->_command;
}

std::string const &		Client::getPassword() const
{
    return this->_password;
}

std::string const &		Client::getNickname() const
{
    return this->_nickname;
}

std::string const &		Client::getUsername() const
{
    return this->_username;
}

std::string const &		Client::getRealname() const
{
    return this->_realname;
}


std::string const &		Client::getMessage() const
{
    return this->_message;
}

int	 const &		Client::getMessageStatus() const
{
    return this->_message_status;
}

int         const &     Client::getSocket() const
{
    return this->_socket;
}

struct pollfd   const & Client::getPoll() const
{
    return this->_poll;
}

std::string const & Client::getServerName() const
{
	return this->_server_name;
}

std::string const & Client::getServerIpAddress() const
{
	return this->_server_ipaddress;
}

std::string const & Client::getServerCreationDate() const
{
	return this->_server_creation_date;
}
bool			const & Client::isRegistered() const
{
	return this->_registration_status;
}

int  Client::getNbChannels() const
{
	return this->_channels.size();
}

bool const & Client::getAway() const
{
	return this->_away;
}

std::string const & Client::getAwayMessage() const
{
	return this->_away_message;
}


//SETTERS

void	Client::setOper(bool const & src)
{
	this->_oper = src;
}

void  Client::setAwayMessage(std::string const & src)
{
	this->_away_message = src;
}

void  Client::setServerName(std::string const & src)
{
	this->_server_name = src;
}

void  Client::setServerIpaddress(std::string const & src)
{
	this->_server_ipaddress = src;
}

void  Client::setServerCreationDate(std::string const & src)
{
	this->_server_creation_date = src;
}


void Client::setRegPass(bool const & src)
{
	this->_reg_pass = src;
}

void Client::setRegNick(bool const & src)
{
	this->_reg_nick = src;
}
void Client::setRegUser(bool const & src)
{
	this->_reg_user = src;
}
void Client::setPassword(std::string const& src)
{
    this->_password = src;
}

void Client::setNickname(std::string const& src)
{
    this->_nickname = src;
}

void Client::setUsername(std::string const& src)
{
    this->_username = src;
}

void Client::setRealname(std::string const& src)
{
    this->_realname = src;
}

void Client::setMessage(std::string const& src)
{
    this->_message = src;
}

void Client::setMessageStatus(int const& src)
{
    this->_message_status = src;
}

void	Client::setRegistration(bool const& src)
{
	this->_registration_status = src;
}

void	Client::setAway(bool const& src)
{
	this->_away = src;
}


//OTHERS

void Client::clearCommand()
{
	_command.clear();
}

void Client::clearMessage()
{
    _message.clear();
}

void Client::init(int const & socket)
{
    this->_socket = socket;
    this->_poll.fd = _socket;
    this->_poll.events = POLLIN | POLLHUP;
    this->_poll.revents = 0;
}

void Client::recvMessage()
{
	char buf[MAX_CHAR + 1];
	int ret; // sisze_t?

	memset(buf, 0, MAX_CHAR);
	if(this->_message_status == COMPLETE)
	{
		this->clearMessage();
		this->_message_status = INCOMPLETE;
	}
	ret = recv(this->getSocket(), buf, MAX_CHAR, 0);
	//Err systeme quitter abruptement
	if (ret == -1)
	{
		this->_message_status = INCOMPLETE;
		std::cout << "ERROR : FATAL : RECV -1" << std::endl;
		return;
		//throw Server::ExceptErrno();
	}
	//Demande de DECO sur linux
	if (ret == 0)
	{
		this->_message_status = DISCONNECT;
		this->clearMessage();
		return;
	}
	//A changer pour voir si cmd_lenght > 512 --> dans Commands. analyze directement (requete complete)
	/*
	if (ret > 512)
	{
		std::cout << "Error : buff > 512" << std::endl;
		this->_message_status = INCOMPLETE;
		return;
	}
	*/
	//Qq chose a lire :
	this->_message  +=  buf;
	//Si on trouve CRCL et qu'on est deja enregistre. Alors CRCL = fin de message.
	
	//Rajouter condiont CRCL DOIT etre LE last char (&& LAST_CHAR == END_CHAR)
	if (std::strstr(buf, END_CHAR) != NULL) // END CHAR
		this->_message_status = COMPLETE;
	//Si pas de CRCL - msg incomplet. Si CRCL mais pas registreded alors le premier msg qui contient plusieurs CRCL est peut etre scinde
	else
		this->_message_status = INCOMPLETE;
}

void Client::analyzeMessage() // Parsing du END_CHAR
{
	int i = 0;
	int init = 0;
	while (_message[i] && _message[i+1])
	{
		if (_message[i] ==  '\r' && _message[i + 1] == '\n')
		{
			this->_command.push_back(_message.substr(init, i - init));
			//manage_substr(_message.substr(init, i - init));
			init = i + 2;
			i += 2;
		}
		else
			i++;
	}
}

void Client::add_channel(Channel* channel)
{
	std::vector<Channel*>::iterator it = _channels.begin();
	std::vector<Channel*>::iterator ite = _channels.end();
	
	while(it != ite)
	{
		if (*it == channel)
			return; // already exists
		it++;
	}
	_channels.push_back(channel);
	return;
}


void Client::remove_channel(Channel* channel)
{
	std::vector<Channel*>::iterator it = _channels.begin();
	std::vector<Channel*>::iterator ite = _channels.end();
	
	while(it != ite)
	{
		if (*it == channel)
		{
			_channels.erase(it);
			return;
		}
		it++;
	}
	return;
}

void Client::leaveAllChannels()
{
	_channels.clear();
}
