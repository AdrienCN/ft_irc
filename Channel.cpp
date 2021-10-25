#include "Channel.hpp"

Channel::Channel(std::string name, Client* client) :_name(name), _has_key(0), _nb_members(0), _has_topic(0), _channel_operator(client)
{
	return;
}

Channel::~Channel()
{
	return;
}

// GETTERS

std::string const &				Channel::getName() const
{
	return this->_name;
}

std::string const &				Channel::getKey() const
{
	return this->_key;
}

bool const &	    			Channel::getStatusKey() const
{
	return this->_has_key;
}

unsigned int const &			Channel::getNbMembers() const
{
	return this->_nb_members;
}

std::string const &				Channel::getTopic() const
{
	return this->_topic;
}

bool const &	    			Channel::getStatusTopic() const
{
	return this->_has_topic;
}

std::vector<Client*> const &	Channel::getMemberList() 
{
	return this->_members;
}

Client*     Channel::getOperator()
{
	return this->_channel_operator;
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

void Channel::unsetTopic()
{
	this->_topic = "";
	this->_has_topic = false;
}

//OTHERS

int Channel::isUserOp(Client* client)
{
	if (client == _channel_operator)
		return (1);
	return (0);
}

int Channel::isUserMember(Client* client)
{
	std::vector<Client*>::iterator it = _members.begin();
	std::vector<Client*>::iterator ite = _members.end();

	if (client == NULL)
		return (0);
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
		return (1);
	return (0);
}

void Channel::addMember(Client *client)
{
	if (client == NULL)
		return;
	_members.push_back(client);
	_nb_members++;
}

void Channel::removeMember(Client *client)
{
	std::vector<Client*>::iterator it = _members.begin();
	std::vector<Client*>::iterator ite = _members.end();
	
	if (client == NULL)
		return;
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
	if (client == _channel_operator)
		_channel_operator = NULL;
}

void	Channel::printMembersNick(Client *client) // iterateur sur membres
{ 
	std::vector<Client*>::iterator it = _members.begin();
	std::vector<Client*>::iterator ite = _members.end();

	if (client == NULL)
		return;
	std::string rpl;
	rpl = ":127.0.0.1 ";
	rpl += "353";
	rpl += (" " +  client->getNickname());
	std::cout << "chan name :" << getName() << std::endl;
	rpl += (" = " + getName() + " :");

	while (it != ite) {
		if (isUserOp(*it) == 1)
			rpl += "@";
		rpl += ((*it)->getNickname() + " ");
		it++;
	}
	rpl +=  "\r\n";
	send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
}

//Util pout debuguer
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
	if (_channel_operator)
		std::cout << "My operator is :" << _channel_operator->getNickname();
	else
		std::cout << "I don't have any operator left";
	std::cout << std::endl;
}