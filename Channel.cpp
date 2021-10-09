#include "Channel.hpp"

Channel::Channel(std::string name, Client* client) :_name(name)
{
    _operators.push_back(client);
    _members.push_back(client);
    return;
}

Channel::~Channel()
{
    return;
}

void Channel::present()
{
    std::cout << "Channel: name = " << getName() << std::endl; 

}

// GETTERS

std::string const & Channel::getName() const
{
	return this->_name;
}

std::string const &		Channel::getKey() const
{
    return this->_key;
}

std::vector<char> const &	Channel::getMode() const
{
    return this->_mode;
}

std::vector<Client*> const &	Channel::getMemberList() const
{
    return this->_members;
}

std::vector<Client*> const &	Channel::getOperatorList() const
{
    return this->_members;
}

std::vector<Client*> const &	Channel::getBannedList() const
{
    return this->_members;
}

//SETTERS

void Channel::setName(std::string const& src)
{
    this->_name = src;
}

void Channel::setKey(std::string const& src)
{
    this->_key = src;
    this->_has_key = true;
}

//OTHERS

int Channel::isUserBanned(Client* client)
{
    std::vector<Client*>::iterator it = _banned.begin();
    std::vector<Client*>::iterator ite = _banned.end();
    
    while (it != ite)
    {
        if ((*it)->getUsername() == client->getUsername())
            return (1);
        it++;
    }
    return (0);
}

int Channel::isUserOp(Client* client)
{
    std::vector<Client*>::iterator it = _operators.begin();
    std::vector<Client*>::iterator ite = _operators.end();
    
    while (it != ite)
    {
        if ((*it)->getUsername() == client->getUsername())
            return (1);
        it++;
    }
    return (0);
}

int Channel::isUserMember(Client* client)
{
    std::vector<Client*>::iterator it = _members.begin();
    std::vector<Client*>::iterator ite = _members.end();
    
    while (it != ite)
    {
        if ((*it)->getUsername() == client->getUsername())
            return (1);
        it++;
    }
    return (0);
}

int Channel::isKeyRight(std::string key)
{
    if (_has_key == false || key == getKey())
        return (1); //no key needed
    return (0); // false
}


