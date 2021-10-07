#include "Client.hpp"

Client::Client() : _message_status(true), _greetings(0), _hostname("defaultconstructorHOSTNAMEvalue")
{
    return;
}

Client::~Client()
{
    return;
}

// GETTERS
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

bool const &		Client::getMessageStatus() const
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

//OTHERS

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
	this->greetings++;
}
