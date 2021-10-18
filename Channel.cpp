#include "Channel.hpp"

Channel::Channel(std::string name, Client* client) :_name(name), _nb_members(0), _has_topic(0), _channel_operator(client)
{
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
    if (_channel_operator)
        std::cout << "My operator is :" << _channel_operator->getNickname();
    else
        std::cout << "I don't have any operator left";
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

std::vector<Client*> const &	Channel::getMemberList() 
{
	return this->_members;
}

// A bouger dans printhelp?
void	Channel::printMembers() { // iterateur sur membres
	std::vector<Client*>::iterator it = _members.begin();
	std::vector<Client*>::iterator ite = _members.end();
	std::string rpl;
	std::cout << "printMembers function" << std::endl;
	while (it != ite) {
		rpl = this->getName() + " ";
		rpl += (*it)->getUsername() + " ";
		rpl += (*it)->getServerIpaddress() + " ";
		rpl += (*it)->getServerName() + " ";
		rpl += (*it)->getNickname() + " ";
		// lettre change en fonction du mode away (" H")/(" G") - user operator ("*")/ member operator ("@") 
		rpl += "H";
		if (isUserOp(*it))
			rpl += "@";
		rpl += " :0 " + (*it)->getRealname() + "\r\n"; // + real name
		send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
		// lettre change en fonction du mode away (" H")/(" G") - user operator ("*")/ member operator ("@") 
		// Adrien -> fait la fonction away qui va me donner les infos 
		rpl = "";
		it++;
	}
}

void	Channel::printOperators() { // iterateur sur membres
	std::vector<Client*>::iterator it = _operators.begin();
	std::vector<Client*>::iterator ite = _operators.end();
	std::string rpl;
	std::cout << "printOperators function" << std::endl;
	while (it != ite) {
		rpl = this->getName() + " ";
		rpl += (*it)->getUsername() + " ";
		rpl += (*it)->getServerIpaddress() + " ";
		rpl += (*it)->getServerName() + " ";
		rpl += (*it)->getNickname() + " ";
		// lettre change en fonction du mode away (" H")/(" G") - user operator ("*")/ member operator ("@") 
		rpl += "H@ :0 " + (*it)->getRealname() + "\r\n"; // + real name
		send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
		// lettre change en fonction du mode away (" H")/(" G") - user operator ("*")/ member operator ("@") 
		// Adrien -> fait la fonction away qui va me donner les infos 
		rpl = "";
		it++;
	}
}

// A bouger dans printhelp?
void	Channel::printMembersNick() { // iterateur sur membres
	std::vector<Client*>::iterator it = _members.begin();
	std::vector<Client*>::iterator ite = _members.end();

	std::string rpl;
	while (it != ite) {
		rpl = ":" + (*it)->getNickname() + "!" + (*it)->getUsername() + "@" + (*it)->getHostname() + " " + "353" + " : ";
		rpl = ":127.0.0.1 ";
		rpl += "353";
		rpl += " " +  (*it)->getNickname();
		std::cout << "chan name :" << getName() << std::endl;
		rpl += (" = " + (*it)->getServerName() + " :");
		rpl += ((*it)->getNickname() + " ");
		rpl +=  "\r\n";
		send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
		rpl = "";
		it++;
	}
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
    if (client == _channel_operator)
        _channel_operator = NULL;
}
