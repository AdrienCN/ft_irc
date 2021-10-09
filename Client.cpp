#include "Channel.hpp"
#include "Client.hpp"
#include "Commands.hpp"

Client::Client() : _message_status(DISCONNECT), _greetings(0), _hostname("defaultconstructorHOSTNAMEvalue"), _registration_status(false)
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

}

// GETTERS

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

bool			const & Client::isRegistered() const
{
	return this->_registration_status;
}

//SETTERS
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

void Client::setMessage(std::string const& src)
{
    this->_message = src;
}

void Client::setMessageStatus(bool const& src)
{
    this->_message_status = src;
}

void	Client::setRegistration(bool const& src)
{
	this->_registration_status = src;
}

//OTHERS


void Client::clearCommand()
{
	_command.clear();
}

void Client::clearStr(std::string str)
{
    str.clear();
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

int	const &	Client::getGreetings()
{
	return this->_greetings;
}

void	Client::incrGreetings()
{
	this->_greetings++;
}
void Client::recvMessage()
{
	char buf[MAX_CHAR];
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
	//A changer pour voir si cmd_lenght > 512
	if (ret > 512)
	{
		std::cout << "Error : buff > 512" << std::endl;
		this->_message_status = INCOMPLETE;
		return;
	}
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

void Client::analyzeMessage()
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
/*
void Client::analyzeCommand()
{	
		Commands tmp_cmd;
		std::vector<Client *> tmp_c;
		std::vector<Channel *> tmp_chan;
		std::vector<std::string> input_tmp;
		std::istringstream str(message);
		std::string tmp;

		//1. Split du sub_str message par les whitespaces dans un vector
		while (std::getline(str, tmp,' ')) // on met dans tmp tout jusqu'a l"espace  + tant que l'on trouve des espaces
		{
			if (std::strstr(tmp.c_str(), END_CHAR)) // pour enlver le retoru charriot
			{
					tmp.erase(tmp.length() - 1);
					//tmp.erase(tmp.length() - 1); //en fonction de /r /n
			}
			if (tmp != "\0") // si je suis pas une chaine vide
			{
				_command.push_back(tmp);
				tmp.clear(); // on enleve tout le contenu et size = 0
			}
		}
		print_vector(_command);
		if (this->_registration_status == false)
		{
			tmp_cmd.find_command(_command, this, tmp_c, tmp_chan);
		//	this->clearCommand();
		}
}
*/
