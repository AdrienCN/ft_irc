#include "Channel.hpp"

Channel::Channel(std::string name, Client* client) :_name(name), _nb_members(0), _has_topic(0)
{
    _operators.push_back(client);
    return;
}

Channel::~Channel()
{
    return;
}

void Channel::present()
{
    std::cout << "Channel name = " << getName();
    if (_has_key == true)
     std::cout << " | my key is " << getKey();
    if (_has_topic == true)
        std::cout << " | my topic is " << getTopic();
    std::cout << std::endl << "My members are : ";
    std::vector<Client*>::iterator it = _members.begin();
    std::vector<Client*>::iterator ite = _members.end();
    while (it != ite)
    {
        std::cout << (*it)->getNickname() << " ";
        it++;
    }
    std::cout << std::endl;
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

bool const &	    	Channel::getStatusKey() const
{
    return this->_has_key;
}

unsigned int const &       Channel::getNbMembers() const
{
    return this->_nb_members;
}

std::string const &		Channel::getTopic() const
{
    return this->_topic;
}

bool const &	    	Channel::getStatusTopic() const
{
    return this->_has_topic;
}

std::vector<char> const &	Channel::getMode() const
{
    return this->_mode;
}

std::vector<Client*> const &	Channel::getMemberList() 
{
    return this->_members;
}

std::vector<Client*> const &	Channel::getOperatorList() 
{
    return this->_members;
}

std::vector<Client*> const &	Channel::getBannedList() 
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

void Channel::setTopic(std::string const& src)
{
    this->_topic = src;
    this->_has_topic = true;

}

//OTHERS

int Channel::isUserBanned(Client* client)
{
    std::vector<Client*>::iterator it = _banned.begin();
    std::vector<Client*>::iterator ite = _banned.end();
    
    while (it != ite)
    {
        if ((*it)->getNickname() == client->getNickname())
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
        if ((*it)->getNickname() == client->getNickname())
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
        if ((*it)->getNickname() == client->getNickname())
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

void Channel::addMember(Client *client)
{
    _members.push_back(client);
    _nb_members++;

}

void Channel::addOp(Client *client)
{
    _operators.push_back(client);
}

void Channel::addBanned(Client *client)
{
    _banned.push_back(client);
}

void Channel::removeMember(Client *client)
{
    std::vector<Client*>::iterator it = _members.begin();
    std::vector<Client*>::iterator ite = _members.end();
    
    while (it != ite)
    {
        if ((*it)->getNickname() == client->getNickname())
        {
            _members.erase(it);
            _nb_members--;
            return;
        }
        it++;
    }
}

void Channel::removeOp(Client *client)
{   
    std::vector<Client*>::iterator it = _operators.begin();
    std::vector<Client*>::iterator ite = _operators.end();
    
    while (it != ite)
    {
        if ((*it)->getNickname() == client->getNickname())
        {
            _operators.erase(it);
            return;
        }
        it++;
    }
}

void Channel::removeBanned(Client *client)
{
    std::vector<Client*>::iterator it = _banned.begin();
    std::vector<Client*>::iterator ite = _banned.end();
    
    while (it != ite)
    {
        if ((*it)->getNickname() == client->getNickname())
        {
            _banned.erase(it);
            return;
        }
        it++;
    }
}

