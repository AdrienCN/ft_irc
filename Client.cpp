#include "Client.hpp"

Client::Client()
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

void Client::init(int const & socket)
{
    this->_socket = socket;
    this->_poll.fd = _socket;
    this->_poll.events = POLLIN | POLLHUP;
    this->_poll.revents = 0;
}