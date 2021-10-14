#include "Commands.hpp"
/*
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
#define ERR_UNAVAILABLERESOURCE 437
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTRED 462
#define ERR_RESTRICTED 484
*/

Commands::Commands(std::string const & password, std::string server_name, std::string server_ipaddress, std::string server_creation_date): _server_password(password) , _server_name(server_name) , _server_ipaddress(server_ipaddress), _server_creation_date(server_creation_date) 
{
	
	_cmd_list["PASS"] = &Commands::pass;
	_cmd_list["NICK"] = &Commands::nick;
	_cmd_list["USER"] = &Commands::user;
	_cmd_list["JOIN"] = &Commands::join;
	_cmd_list["PART"] = &Commands::part;
	_cmd_list["TOPIC"] = &Commands::topic;
	_cmd_list["PRIVMSG"] = &Commands::privmsg;
	_cmd_list["WHO"] = &Commands::who;
	_cmd_list["NAMES"] = &Commands::names;
	return;
}

Commands::~Commands()
{
	return;
}

//************ PARSING ET TRI DE LA STR COMMAND EN VECTOR ********************

std::string const & Commands::getServerName() const
{
	return this->_server_name;
}

std::string const & Commands::getServerIpaddress() const
{
	return this->_server_ipaddress;
}

std::string const & Commands::getServerCreationDate() const
{
	return this->_server_creation_date;
}

void  Commands::setServerName(std::string const src)
{
	this->_server_name = src;
}

void  Commands::setServerIpaddress(std::string const src)
{
	this->_server_ipaddress = src;
}

void  Commands::setServerCreationDate(std::string const src)
{
	this->_server_creation_date = src;
}

void Commands::find_command(std::string input, Client* client, std::vector<Client*> client_list, std::vector<Channel*>* channel_list) 
{
	// A verifier si on doit gérer les prefixes ":" en premier message (mais je pense pas)
	(void)client;
	(void)client_list;
	(void)channel_list;

	this->analyzeCommand(input);
	std::string key(_parsed_cmd.front());
	std::cout << "key :" << key << std::endl;
	//std::map<std::string , void(Commands::*)(std::vector<std::string>)>::iterator it = _cmd_list.begin();
	if (_cmd_list[key])
		(this->*_cmd_list[key])(_parsed_cmd, client, client_list, channel_list);
	else
		ft_error(ERR_UNKNOWNCOMMAND, _parsed_cmd, client, NULL, client_list, *channel_list);
	this->_parsed_cmd.clear();
}

std::string ft_uppercase(const char* str)
{
	std::string str2;
	char c;

	for (int i = 0; str[i]; i++)
	{
		c = std::toupper(str[i]);
		str2 += &c;
	}
	return str2;
}

void Commands::analyzeCommand(std::string command) // Parsing des whitespaces
{	

		std::istringstream str(command);
		std::string tmp;
		bool first = 1;
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
			/* On en a plus besoin car fait a l'etape d'avant
			if (std::strstr(tmp.c_str(), END_CHAR)) // pour enlver le retoru charriot
			{
					tmp.erase(tmp.length() - 1);
					//tmp.erase(tmp.length() - 1); //en fonction de /r /n
			}
			*/
			if (first == 1)
			{
				std::string tmp2 = ft_uppercase(tmp.c_str());
				tmp.clear();
				tmp = tmp2;
				first = 0;
			}
			if (tmp != "\0") // si je suis pas une chaine vide
			{
				_parsed_cmd.push_back(tmp);
				tmp.clear(); // on enleve tout le contenu et size = 0
			}
		}
	}
	print_vector(_parsed_cmd);
}

//************ COMMANDES A EXECUTER ********************

// ******** PASS *************

void Commands::pass(std::vector<std::string> params, CMD_PARAM)
{
	(void)client;
	(void)client_list;
	(void)channel_list;
	(void)params;

    std::cout << YELLOW << "Hello from PASS function!" << RESET << std::endl;
	if (client->isRegistered() == true)
		return ft_error(ERR_ALREADYREGISTERED, params, client, NULL, client_list, *channel_list);
	if (params.size() < 2)
		return (ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list));
	//quid si le password est faux
	//quid si le password est vide
	
	std::cout << "Password OK" << std::endl;
	client->setPassword(params[1]);
	client->setRegPass(true);
}

// ******** NICK *************

// Faire fonction generique qui retourne un *Client puis NULL au bout de la liste
int		ft_nickname_exist(std::vector<Client*> client_list, std::string nickname)
{
	std::vector<Client*>::iterator it = client_list.begin();

	while (it != client_list.end())
	{
		if ((*it)->getNickname() == nickname)
			return (1);
		it++;
	}
	return (0);
}

Client* 	ft_nickname_exist_return(std::vector<Client*> client_list, std::string nickname)
{
	std::vector<Client*>::iterator it = client_list.begin();

	while (it != client_list.end())
	{
		if ((*it)->getNickname() == nickname)
			return (*it);
		it++;
	}
	return (NULL);
}


void Commands::nick(std::vector<std::string> params, CMD_PARAM)
{
	(void)client;
	(void)client_list;
	(void)channel_list;
	(void)params;

	std::cout << YELLOW << "Hello from NICK function!"<< RESET  << std::endl;
	//Pas de pseudo donne
	if (params.size() < 2)
	{
		return ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list);
	}
		//Pseudo contient des chars non autorise 
		//	ft_error(432)a coder ??
		//Pseudo exist deja
	if (ft_nickname_exist(client_list, params[1]))
	{
		return ft_error(ERR_NICKNAMEINUSE, params, client, NULL, client_list, *channel_list);
	}
	std::string rpl;
	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " NICK " + params[1] + "\r\n";
	send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
	std::cout << rpl << std::endl;
	client->setNickname(params[1]);
	client->setRegNick(true);
}

// ******** USER *************

std::string ft_findUserRealname(std::vector<std::string> params)
{
	std::vector<std::string>::iterator itb = params.begin();
	std::vector<std::string>::iterator ite = params.end();
	std::string realname;
	bool semicolon_found = false;

	while(itb != ite)
	{
			
		if (std::strstr((*itb).c_str(), ":"))
			semicolon_found = true;

		if (semicolon_found == true)
			realname += *itb;
		itb++;
	}
	//effacer le : de :realname
	realname.erase(realname.begin());
	std::cout << "realname = ["<< realname << std::endl;
	return (realname);
}

void Commands::user(std::vector<std::string> params, CMD_PARAM)
{
	(void)client;
	(void)client_list;
	(void)channel_list;
	(void)params;

	std::cout << YELLOW << "Hello from USER function!" << RESET << std::endl;
	if (client->isRegistered() == true)
		return ft_error(ERR_ALREADYREGISTERED, params, client, NULL, client_list, *channel_list);
	if (params.size() < 5)
		return ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list);	
	client->setRegUser(true);
	client->setRealname(ft_findUserRealname(params));
	client->setUsername(params[1]);
}

// ******** Channel Functions *************

int isValidChanName(std::string channel_name)
{
	if (channel_name == "\0") // + ajouter els regles d'un nom de channel
		return (0);
	if (channel_name[0] != '#' && channel_name[0] != '&')
		return (0);
	else
		return (1); 
}

void  commaSplit(std::string str, std::vector<std::string>* vector)
{	
	std::istringstream str2(str);
	std::string tmp;

	while (std::getline(str2, tmp,','))
	{
		if (isValidChanName(tmp))
		{

			//if (tmp[0] == '#' || tmp[0] == '&') // HEXCHAT envoie le nom de channel avec ces args
			//	tmp.erase(0, 1);
			vector->push_back(tmp);
			tmp.clear(); // on enleve tout le contenu et size = 0
		}
		tmp.clear(); // on enleve tout le contenu et size = 0
	}
}

Channel* ft_channel_exist(std::vector<Channel*> channel_list, std::string channel)
{
	std::vector<Channel*>::iterator it = channel_list.begin();

	//if (channel[0] == '#' || channel[0] == '&')  // HEXCHAT envoie le nom de channel avec ces args
	//	channel.erase(0, 1);

	while (it != channel_list.end())
	{
		if ((*it)->getName() == channel)
			return (*it);
		it++;
	}
	return (NULL);
}

void joinChannel(Channel* new_chan, std::vector<std::string> params, CMD_PARAM)
{
	client->add_channel(new_chan);
	new_chan->addMember(client);

	std::string rpl;
	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " JOIN " + new_chan->getName() + "\r\n";
	std::vector<Client*> tmp = new_chan->getMemberList();
	std::vector<Client*>::iterator it = tmp.begin();
	std::vector<Client*>::iterator ite = tmp.end();
	while (it != ite)
	{
		send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
		it++;
	}
	(void)params;
	(void)client;
    (void)client_list;
    (void)channel_list;
	(void)params;

	//A eclaircir si besoin ou pas	
	if (new_chan->getStatusTopic() == false)
		ft_reply(RPL_NOTOPIC, params, client, new_chan, client_list, *channel_list); // RPL_NOTOPIC
	else
		ft_reply(RPL_TOPIC, params, client, new_chan, client_list, *channel_list); // RPL_TOPIC
	ft_reply(RPL_NAMEREPLY, params, client, new_chan, client_list, *channel_list); // RPL_NAMEREPLY
	ft_reply(RPL_ENDOFNAMES, params, client, new_chan, client_list, *channel_list); // RPL_NAMEREPLY
	
}

void leaveChannel(Channel* channel, Client *client, std::string part_message)
{
	client->remove_channel(channel);
	/* cas ou plus d'utilisateurs 

	std::vector<Channel*>::iterator itl = channel_list->begin();
	std::vector<Channel*>::iterator itle = channel_list->end();
	while (itl != itle)
	{
		if ((*itl)->getName() == channel->getName())
			break;
		itl++;
	}
	if (*itl)->getNbMembers() == 0)
	{
		channel_list->removeChannel((*itl->getName());
		delete *itl;
	}
	cas a faire : operateur is leaving
	*/
	std::string rpl;
	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " PART " + channel->getName() + " :" + part_message + "\r\n";
	std::vector<Client*> tmp = channel->getMemberList();
	std::vector<Client*>::iterator it = tmp.begin();
	std::vector<Client*>::iterator ite = tmp.end();
	while (it != ite)
	{
		send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
		it++;
	}
	channel->removeMember(client);
	channel->removeOp(client); // si jamais le client est Operateur
	return;
}

// ******** JOIN *************

void Commands::join(std::vector<std::string> params, CMD_PARAM)
{
	std::cout << YELLOW << "Hello from JOIN function!" << RESET << std::endl;
	(void)client;
	(void)client_list;
	(void)channel_list;
	(void)params;

	if (params.size() == 1)
	{
		std::cout << "Error : JOIN lack of params" << std::endl;
		ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list); //  ERR_NEEDMOREPARAMS
		return;
	}

	//On parse le 2e arg de params (channel name) et le 3e (les keys potentiellement)
	std::vector<std::string>::iterator it = params.begin();
	it++;
	std::vector<std::string> channels_name;
	commaSplit(*it, &channels_name);
	it++;
	std::vector<std::string> channels_key;
	if (it != params.end())
		commaSplit(*it, &channels_key);
	print_vector(channels_name);
	print_vector(channels_key);

	//On s'occupe des noms de chan un a un

	std::vector<std::string>::iterator itn = channels_name.begin();
	std::vector<std::string>::iterator itne = channels_name.end();
	std::vector<std::string>::iterator itk = channels_key.begin();
	std::vector<std::string>::iterator itke = channels_key.end();

	while (itn != itne)
	{
		
		Channel* tmp = ft_channel_exist(*channel_list, *itn);
		if (tmp) // channel already exist
		{
			tmp->present();

			std::cout << YELLOW << "Channel " << tmp->getName() << " already exist" << RESET << std::endl;
			if (tmp->isUserBanned(client) == 1)//user banned
			{
				std::cout << RED << "You are banned from  channel" <<  tmp->getName() << RESET <<  std::endl;
				ft_error(ERR_BANNEDFROMCHAN, params, client, tmp, client_list, *channel_list); // ERR_BANNEDFROMCHANNEL
			}
			else if(tmp->isUserMember(client) == 1)
			{
				std::cout << RED << client->getNickname() << " is already a client of channel " <<  tmp->getName() << RESET << std::endl;
				//pas d'erreurs retournée
			}
			else if (client->getNbChannels() == USER_MAXCHAN)
				ft_error(ERR_TOOMANYCHANNELS, params, client, tmp, client_list, *channel_list); //a faire une fois au debute ou a chaque fois?
			else if(tmp->getNbMembers() == CHAN_MAXCAPACITY)
			{
				ft_error(ERR_CHANNELISFULL, params, client, tmp, client_list, *channel_list); // ERR8CHANNELISFULL
			}
			
			else
			{
				if (tmp->getStatusKey() == true)
				{
					if (itk != itke)
					{
						if (tmp->isKeyRight(*itk) == 0)
						{
							std::cout << RED <<  *itk << " is not the right key for channel " << tmp->getName() << RESET << std::endl;
							ft_error(ERR_BADCHANNELKEY, params, client, tmp, client_list, *channel_list); // ERR_BADCHANNELKEY
						}
						else
						{
							std::cout << GREEN << *itk << " was the right key! Welcome to Channel " << tmp->getName() << RESET << std::endl;
							joinChannel(tmp, params, client, client_list, channel_list);
						}	
						itk++;
					}
					else
					{
						std::cout << RED <<  "The channel " << tmp->getName() << " needs a key"  << RESET << std::endl;
						ft_error(ERR_NEEDMOREPARAMS, params, client, tmp, client_list, *channel_list); //  ERR_NEEDMOREPARAMS (ou bad channel key?)
					}
				}
				else
				{
					std::cout << GREEN << "Welcome to Channel " << tmp->getName() << RESET<< std::endl;
					joinChannel(tmp, params, client, client_list, channel_list);
				}	
			}	
		}
		else // create channel
		{
			if (*itn == "0") // must be like a part command
			{
				std::cout << RED << client->getNickname() << " must leave all channels!" << RESET << std::endl;
				client->leaveAllChannels();
				std::vector<Channel*>::iterator itl = channel_list->begin();
				std::vector<Channel*>::iterator itle = channel_list->end();
				while (itl != itle)
				{
					if ((*itl)->isUserMember(client) == 1)
					{
						leaveChannel(*itl, client, client->getNickname());
					}
					itl++;
				}
			}
			else
			{
				if (isValidChanName(*itn) == 0) //not a channel name
				{
					std::cout << RED << *itn << " is not a valid Channel name" << RESET << std::endl;
					ft_error(ERR_BADCHANMASK, params, client, tmp, client_list, *channel_list); //  ERR_BADCHANMASK			}
				}
				else
				{
					std::cout << GREEN << "Channel to create!" << RESET << std::endl;

					Channel* new_chan = new Channel(*itn, client);
					new_chan->present();
					if (client->getNbChannels() == USER_MAXCHAN)
					{
						ft_error(ERR_TOOMANYCHANNELS, params, client, new_chan, client_list, *channel_list); //a faire une fois au debute ou a c
						delete new_chan;
					}
					else
					{
						if (itk != itke)
						{
							new_chan->setKey(*itk);
							itk++;
						}
						channel_list->push_back(new_chan);
						joinChannel(new_chan, params, client, client_list, channel_list);
					}
				}
			}
		}
		itn++;
	}
}

// ******** PART *************

void Commands::part(std::vector<std::string> params, CMD_PARAM)
{
	std::cout << YELLOW << "Hello from PART function!" << RESET << std::endl;
	(void)client;
	(void)client_list;
	(void)channel_list;
	(void)params;

	if (params.size() == 1)
	{
		std::cout << "Error : PART lack of params" << std::endl;
		ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list); //  ERR_NEEDMOREPARAMS
		return;
	}

	//On parse le 2e arg de params (channel name)
	std::vector<std::string>::iterator it = params.begin();
	std::vector<std::string>::iterator ite = params.end();
	it++;
	std::vector<std::string> channels_name;
	commaSplit(*it, &channels_name);

	//On cherche un potentiel message de départ
	it++;
	std::string part_message;
	if (it != ite)
		part_message = (*it);
	else
		part_message = client->getNickname();

	//Juste pour imprimer
	/*
	print_vector(channels_name);
	std::cout << "Part message: " << part_message << std::endl;
	*/

	//On s'occupe des noms de chan un a un
	std::vector<std::string>::iterator itn = channels_name.begin();
	std::vector<std::string>::iterator itne = channels_name.end();

	while (itn != itne)
	{
		Channel* tmp = ft_channel_exist(*channel_list, *itn);
		if (tmp) 
		{
			std::cout << GREEN << "The Channel to leave \"" << tmp->getName() << "\" exists" << RESET << std::endl;
			if(tmp->isUserMember(client) == 0)
			{
				std::cout << RED << client->getNickname() << " is not a client of channel " <<  tmp->getName() << RESET << std::endl;
				ft_error(ERR_NOTONCHANNEL, params, client, NULL, client_list, *channel_list); //  ERR_NOTONCHANNEL
			}
			else
			{	
				std::cout << YELLOW << client->getNickname() << " is leaving channel " <<  tmp->getName() << RESET << std::endl;
				leaveChannel(tmp, client, part_message);
			}	
		}
		else // no such channel
		{
			ft_error(ERR_NOSUCHCHANNEL, params, client, NULL, client_list, *channel_list); //  ERR_NOSUCHCHANNEL
		}
		itn++;
	}
}

// ******** TOPIC *************

void Commands::topic(std::vector<std::string> params, CMD_PARAM)
{
	std::cout << YELLOW << "Hello from TOPIC function!" << RESET << std::endl;

	if (params.size() == 1)
	{
		std::cout << "Error : TOPIC lack of params" << std::endl;
		ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list); //  ERR_NEEDMOREPARAMS
		return;
	}

	std::vector<std::string>::iterator it = params.begin();
	std::vector<std::string>::iterator ite = params.end();
	it++;
	Channel* tmp = ft_channel_exist(*channel_list, *it);
	if (tmp) // channel exist
	{
		std::cout << GREEN << "The Channel \"" << tmp->getName() << "\" exists" << RESET << std::endl;

		if (tmp->isUserMember(client) == 0) //si je ne suis pas membre du chat
		{
			ft_error(ERR_NOTONCHANNEL, params, client, NULL, client_list, *channel_list); //  ERR_NOTONCHANNEL
			return;
		}
		it++;
		if (it == ite) // Voir le sujet du cannal
		{
			std::cout << YELLOW <<  "Just here to see the TOPIC of channel " << tmp->getName() << RESET << std::endl;
			if (tmp->getStatusTopic() == false)
			{
				std::cout << RED <<  "This Channel has no topic"  << RESET << std::endl;
				ft_reply(RPL_NOTOPIC, params, client, tmp, client_list, *channel_list); //  RPL_NOTOPIC
			}
			else
			{
				std::cout << YELLOW <<  "This Channel topic is "  << tmp->getTopic() << RESET << std::endl;
				ft_reply(RPL_TOPIC, params, client, tmp, client_list, *channel_list); //  RPL_TOPIC
			}
		}
		else // creer ou change le sujet du cannal
		{
			std::string topic = *it;
			if (topic[0] == ':')
				topic.erase(0,1);
			std::cout << YELLOW <<  "I want to create or change the TOPIC of channel " << tmp->getName() <<  " to " << topic << RESET << std::endl;
			if (tmp->getStatusTopic() == false) // creation
			{
				std::string rpl;
				rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " TOPIC  " + tmp->getName() + " :" + topic + "\r\n";
				std::vector<Client*> tmp_client = tmp->getMemberList();
				std::vector<Client*>::iterator it = tmp_client.begin();
				std::vector<Client*>::iterator ite = tmp_client.end();
				while (it != ite)
				{
					send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
					it++;
				}
				//send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
			}
			tmp->setTopic(topic);
		}
	}
	else
	{
		std::cout << RED << "The Channel does not exists" << RESET << std::endl;
		// AJOUT pas sure que ce soit a faire
		ft_error(ERR_NOSUCHCHANNEL, params, client, NULL, client_list, *channel_list); //  ERR_NOSUCHCHANNEL
	}
}

// ******** PRIVMSG *************

void send_privmsg_user(Client *client, std::string message, Client* user)
{
	std::string rpl;

	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " PRIVMSG " + user->getNickname() + message;
  	send(user->getSocket(), (rpl.c_str()), rpl.size(), 0);;
}

void send_privmsg_channel(Client *client, std::string message, Channel* channel)
{
	std::vector<Client*> tmp = channel->getMemberList();
	std::vector<Client*>::iterator it = tmp.begin();	
	std::vector<Client*>::iterator ite = tmp.end();

	std::string rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " PRIVMSG " + channel->getName() + message;
	while (it != ite)
	{
		if (*it != client) // si je ne suis pas l'envoyeur
		{    
			send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
		}
		it++;
	}
}

void Commands::privmsg(std::vector<std::string> params, CMD_PARAM)
{
	std::cout << YELLOW << "Hello from PRIVMSG function!" << RESET << std::endl;

	if (params.size() <= 2)
	{
		std::cout << "Error : PRIVMSG lack of params" << std::endl;
		if (params.size() == 1)
		    ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list); //  ERR_NEEDMOREPARAMS
		else if (params.size() == 2)
		    ft_error(ERR_NOTEXTTOSEND, params, client, NULL, client_list, *channel_list); //  ERR_NOTEXTTOSEND
		return;
	}

	//On parse le 2e arg de params (destinataires)
	std::vector<std::string>::iterator it = params.begin();
	std::vector<std::string>::iterator ite = params.end();
	it++;
	std::vector<std::string> destinataires;
	commaSplit(*it, &destinataires);

	//On cherche un potentiel message de départ&
	it++;
	std::string message;
	message.clear();
	message += " ";
	while (it != ite)
	{
		std::cout << "str = " << *it << std::endl;
		if (message.size() == 1 && (*it).c_str()[0] !=  ':')
			message += ":";
		message += (*it);
		message += " ";
		it++;
	}
	message += "\r\n";
	//Juste pour imprimer
	print_vector(destinataires);
	std::cout << "Message: |" << message << "|" << std::endl;

	//On s'occupe des noms de chan un a un
	std::vector<std::string>::iterator itd = destinataires.begin();
	std::vector<std::string>::iterator itde = destinataires.end();
	while (itd != itde)
	{
		Client* tmp_client = ft_nickname_exist_return(client_list, *itd);
		if (tmp_client) // si j'ai trouvé un client 
		{
			std::cout << "I am a user" << std::endl;
		    if (client == tmp_client)
			{
				std::cout << RED << "Cannot send a message to yourself!" << RESET << std::endl;
				//err?
			}
			else
			{
				std::cout << GREEN << "Message from " << client->getNickname() << " to " << tmp_client->getNickname() <<  " : " << message << RESET << std::endl;
		   		send_privmsg_user(client, message, tmp_client);
			}
	   }
		else // si je suis un cannal
		{
			std::cout << "I am a channel" << std::endl;
			Channel* tmp_channel = ft_channel_exist(*channel_list, *itd);
			if (tmp_channel) // si je suis un cannal
			{
				if (tmp_channel->isUserMember(client) == 0) //si je ne fais pas parti de ce cannal
				{
					std::cout << RED << client->getNickname() << " is not a member of " << tmp_channel->getName() << RESET << std::endl;
					ft_error(ERR_CANNOTSENDTOCHAN, params, client, tmp_channel, client_list, *channel_list); //  ERR_CANNOTSENDTOCHAN
			        return;
        }
				else
				{
					std::cout << GREEN << "Message from " << client->getNickname() << " to " << tmp_channel->getName() <<  " : " << message << RESET << std::endl;
					send_privmsg_channel(client, message, tmp_channel);
				}
			}
			else
			{
				std::cout << RED << "Not a client or a channel" << RESET << std::endl;
			    ft_error(ERR_NOSUCHNICK, params, client, NULL, client_list, *channel_list); //ERR_NOSUCHNICK
			}
		}	
		itd++;
	}
}


/* ************************************WHO****************************************** */

//reste a definir : 
//l utilisation de "o"
//l exporation des variable du serveur pour enlever les parametres en dur de printWho
//comprendre les operator a send
//utilisation de real name

static int	matchChannel(std::vector<std::string> params, CMD_PARAM) {
	std::vector<Channel*>::iterator it = channel_list->begin();
	int i = 0;
	(void)client_list;
	(void)client;

	while (it != channel_list->end()) {
		std::cout << "params[1] : " << params[1] << std::endl;
		std::cout << "it : " << (*it)->getName() << std::endl;
		if (!params[1].compare((*it)->getName())) {
			return i;
		}
		i++;
		it++;
	}
	return (-1);
}

static void	printWho(std::vector<std::string> params, CMD_PARAM, Client* client_ref) {
	std::string rpl;
	(void)params;
	(void)client_list;
	(void)channel_list;
	rpl += "*" + client_ref->getUsername() + " ";
	rpl += "127.0.0.1";
	rpl += " irc_90.com " + client_ref->getNickname();
	// lettre change en fonction du mode away (" H")/(" G") - user operator ("*")/ member operator ("@") 
	rpl += " H  :0\r\n"; // + real name
	send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
}

static	int	matchOthers(std::vector<std::string> params, CMD_PARAM) {
	std::vector<Client*>::iterator it = client_list.begin();
	(void)channel_list;
	(void)client;
	int i = 0;

	while (it != client_list.end()) {
		if (!params[1].compare((*it)->getUsername())) {
			printWho(params, client, client_list, channel_list, *it);
			/* std::cout << "real name : " << (*it)->getUsername() << std::endl; */
			i++;
		}
		if (!params[1].compare((*it)->getNickname())) {
			printWho(params, client, client_list, channel_list, *it);
			/* std::cout << "nick name : " << (*it)->getNickname() << std::endl; */
			i++;
		}
		it++;
	}
	return i;
}

static void	printWhoAllClient(std::vector<std::string> params, CMD_PARAM) {
	std::vector<Client*>::iterator it = client_list.begin();
	while (it != client_list.end()) {
		printWho(params, client, client_list, channel_list, *it);
		std::cout << (*it)->getUsername() << std::endl;
		it++;
	}
}

void Commands::who(std::vector<std::string> params, CMD_PARAM) {
	std::cout << YELLOW << "Hello from Who function!" << RESET << std::endl;
	if (params.size() == 1) {
		printWhoAllClient(params, client, client_list, channel_list);
		return ;
	}
	// on cheche si un cannal correspond au second parametre de who 
	int num = matchChannel(params, client, client_list, channel_list);
	if (num < 0) {
		//on regarde si le parametre de who correspond a n importe quoi d autre
		if (matchOthers(params, client, client_list, channel_list) == 0)
			return ;
		// on trouve rien on print tout
		printWhoAllClient(params, client, client_list, channel_list);
		return ;
	}
	//on print tout les membres du cannal trouvé 
	(*channel_list)[num]->printMembers();
	std::cout << YELLOW << "GoodBye Who function!" << RESET << std::endl;
}

/* ************************************NAMES****************************************** */

static void	printNamesAllChan(std::vector<Channel*>* channel_list) {
	std::vector<Channel*>::iterator it = (*channel_list).begin();
	while (it != (*channel_list).end()) {
		(*it)->printMembersNick();
		it++;
	}
}

void Commands::names(std::vector<std::string> params, CMD_PARAM) {
	std::cout << YELLOW << "Hello from Names function!" << RESET << std::endl;
	(void)client;
	(void)client_list;
	(void)channel_list;
	if (!params[1].compare("IRC_90's")) { //a changer avec la ref du num du serveur
		//la commande NAMES seule est renvoyer par hexchat comme NAMES + IRC_90's 
		printNamesAllChan(channel_list);
		return ;
	}
	int num = matchChannel(params, client, client_list, channel_list);
	(*channel_list)[num]->printMembersNick();
}
