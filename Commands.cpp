#include "Commands.hpp"


Client* 	ft_nickname_exist_return(std::vector<Client*> client_list, std::string nickname);

Commands::Commands(std::string const & password, std::string server_name, std::string server_ipaddress, std::string server_creation_date): _server_password(password) , _server_name(server_name) , _server_ipaddress(server_ipaddress), _server_creation_date(server_creation_date), _oper_name("admin"), _oper_pass("password")
{

	_cmd_list["MODE"] = &Commands::mode;
	_cmd_list["KILL"] = &Commands::kill;
	_cmd_list["OPER"] = &Commands::oper;
	_cmd_list["CAP"] = &Commands::cap;
	_cmd_list["AWAY"] = &Commands::away;
	_cmd_list["PASS"] = &Commands::pass;
	_cmd_list["NICK"] = &Commands::nick;
	_cmd_list["USER"] = &Commands::user;
	_cmd_list["JOIN"] = &Commands::join;
	_cmd_list["PART"] = &Commands::part;
	_cmd_list["KICK"] = &Commands::kick;
	_cmd_list["TOPIC"] = &Commands::topic;
	_cmd_list["PRIVMSG"] = &Commands::privmsg;
	_cmd_list["NOTICE"] = &Commands::notice;
	_cmd_list["WHO"] = &Commands::who;
	_cmd_list["NAMES"] = &Commands::names;
	_cmd_list["LIST"] = &Commands::list;
	_cmd_list["QUIT"] = &Commands::quit;
	_cmd_list["MYHELP"] = &Commands::myhelp;
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

void  Commands::setServerName(std::string const & src)
{
	this->_server_name = src;
}

void  Commands::setServerIpaddress(std::string const & src)
{
	this->_server_ipaddress = src;
}

void  Commands::setServerCreationDate(std::string const & src)
{
	this->_server_creation_date = src;
}

void Commands::find_command(std::string input, Client* client, std::vector<Client*> client_list, std::vector<Channel*>* channel_list) 
{
	// A verifier si on doit gérer les prefixes ":" en premier message (mais je pense pas)
	(void)client;
	(void)client_list;
	(void)channel_list;

	if (input.size() > 510) // 510 car \r\n deja parsé == 512
	{
		std::string rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " : command too long (512)\r\n";
		send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
		return; 
	}

	this->analyzeCommand(input);
	std::string key(_parsed_cmd.front());
	std::cout << "key :" << key << std::endl;
	//std::map<std::string , void(Commands::*)(std::vector<std::string>)>::iterator it = _cmd_list.begin();
	if (_cmd_list[key])
		(this->*_cmd_list[key])(_parsed_cmd, client, client_list, channel_list);
	else
	{
		std::string arg("[");
		std::vector<std::string>::iterator itb = _parsed_cmd.begin();
		while (itb != _parsed_cmd.end())
		{
			arg += *itb;
			itb++;
			if (itb != _parsed_cmd.end())
				arg += " ";
		}
		arg += "]";
		ft_error(ERR_UNKNOWNCOMMAND, client, NULL, arg);
	}
	this->_parsed_cmd.clear();
}

void	ft_uppercase(std::string & str)
{
	std::string::iterator  itb = str.begin();
	while (itb != str.end())
	{
		*itb = std::toupper(*itb);
		itb++;
	}
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
				ft_uppercase(tmp);
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

// *********CAP******
void	Commands::cap(std::vector<std::string> params, CMD_PARAM)
{
	(void)params;
	(void)client_list;
	(void)channel_list;
	ft_reply(RPL_CUSTOMCAP, client, NULL, "");
}


void	Commands::oper(std::vector<std::string> params, CMD_PARAM)
{
	(void)params;
	(void)client_list;
	(void)channel_list;
	std::cout << YELLOW << "Hello from OPER function!" << RESET << std::endl;
	if (params.size() < 3)
		return (ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]));
	//return (ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list));

	//If the client is not connecting from a valid host for the given name,
	if (client->isRegistered() == false)
		return (ft_error(ERR_NOOPERHOST, client, NULL, ""));
	//return (ft_error(ERR_NOOPERHOST, params, client, NULL, client_list, *channel_list));

	//If the client does not send the correct password for the given name
	if (params[1].compare(this->_oper_name) || params[2].compare(this->_oper_pass))
		return (ft_error(ERR_PASSWDMISMATCH, client, NULL, ""));
	std::cout << YELLOW << "OPER success" << RESET << std::endl;
	client->setOper(true);
	ft_reply(RPL_YOUREOPER, client, NULL, "");
	ft_reply(RPL_UMODEIS, client, NULL, "");
}



// *********MODE**********(Ne peut s'utiliser que sur soit meme)


void	Commands::mode(std::vector<std::string> params, CMD_PARAM)
{
	std::cout << YELLOW << "Hello from MODE function!" << RESET << std::endl;

	if (params.size() < 2)
		return (ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]));
	//return (ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list));
	if (params[1] != client->getNickname())
		return (ft_error(ERR_USERSDONTMATCH, client, NULL, ""));
	//return (ft_error(ERR_USERSDONTMATCH, params, client, NULL, client_list, *channel_list));
	//MODE <nickname> --> renvoie info sur user
	if (params.size() == 2)
		return(ft_reply(RPL_UMODEIS, client, NULL, ""));
	//		return (ft_reply(RPL_UMODEIS, params, client, NULL, client_list, *channel_list));
	std::string mode = params[2];
	if ((mode[0] != '-' && mode[0] != '+') || (mode.size() != 2))
		return (ft_error(ERR_UMODEUNKNOWNFLAG, client, NULL, ""));
	//return (ft_error(ERR_UMODEUNKNOWNFLAG, params, client, NULL, client_list, *channel_list));
	if (mode[1] != 'a' && mode[1] != 'O')
		return (ft_error(ERR_UMODEUNKNOWNFLAG, client, NULL, ""));
	//return (ft_error(ERR_UMODEUNKNOWNFLAG, params, client, NULL, client_list, *channel_list));

	//User attemps to bypass OPER command, IGNORE command
	if (mode.compare("+O") == 0)
		return;
	else if (mode.compare("-O") == 0)
	{
		client->setOper(false);
		return (ft_reply(RPL_CUSTOMMODESUCCESS, client, NULL, mode));
	}
	else if (mode.compare("+a") == 0)
	{
		client->setAwayMessage("I'm busy");
		client->setAway(true);
		ft_reply(RPL_CUSTOMMODESUCCESS, client, NULL, mode);
		std::vector<std::string> tmp;
		tmp.push_back("AWAY");
		tmp.push_back(":I'm busy");
		this->away(tmp, client, client_list, channel_list);
	}
	else if (mode.compare("-a") == 0)
	{
		client->setAway(false);
		//	ft_reply(RPL_CUSTOMMODESUCCESS, params, client, NULL, client_list, *channel_list);
		ft_reply(RPL_CUSTOMMODESUCCESS, client, NULL, "");
		std::vector<std::string> tmp;
		tmp.push_back("AWAY");
		this->away(tmp, client, client_list, channel_list);
	}
	//quid de l'input MODE <nickname> +o qqchose qqchose || MODE <nickname> +oui
}


// ******** AWAY *************

void	Commands::away(std::vector<std::string> params, CMD_PARAM)
{
	std::cout << YELLOW << "Hello from AWAY function!" << RESET << std::endl;

	(void)client;
	(void)client_list;
	(void)channel_list;
	(void)params;


	//BACK instead
	if (params.size() == 1)
	{
		std::cout << GREEN << "MODE 1" << RESET << std::endl;

		client->setAway(false);
		client->getAwayMessage();
		return (ft_reply(RPL_UNAWAY, client, NULL,""));
	}
	else if (params.size() >= 2)
	{
		std::cout << GREEN << "MODE 2" << RESET << std::endl;

		client->setAway(true);
		//faux, message = concatene de params[1] -> params.end()
		std::string away_msg;
		for (std::vector<std::string>::iterator it = params.begin() + 1; it != params.end(); it++)
		{
			away_msg += *it + " ";
		}
		away_msg.erase(away_msg.end() - 1);
		//delete ":"
		away_msg.erase(away_msg.begin());
		client->setAwayMessage(away_msg);
		return (ft_reply(RPL_NOWAWAY, client, NULL,""));
	}
	//	*/

}
// ******** PASS *************

void Commands::pass(std::vector<std::string> params, CMD_PARAM)
{
	(void)client;
	(void)client_list;
	(void)channel_list;
	(void)params;

	std::cout << YELLOW << "Hello from PASS function!" << RESET << std::endl;
	if (client->isRegistered() == true)
		return ft_error(ERR_ALREADYREGISTERED, client, NULL, "");
	//return ft_error(ERR_ALREADYREGISTERED, params, client, NULL, client_list, *channel_list);
	if (params.size() < 2)
		return (ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]));
	//		return (ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list));
	if (client->getRegUser() == true || client->getRegNick() == true)
		return;
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

bool	ft_nick_syntax_error(std::string nick)
{
	std::string special("[]\\_^{|}");
	std::cout<<  YELLOW << "nickname = nick " << std::endl;
	
	if (!isalpha(nick[0]) && special.find(nick[0], 0) == std::string::npos )
		return (true);
	std::cout << "first ok" << std::endl;
	for (std::string::iterator it = nick.begin() + 1; it != nick.end(); it++)
	{
		if (!isalpha(*it) && special.find(*it, 0) == std::string::npos && *it != '-'  && !isdigit(*it))
			return (true);
	}
	std::cout << "nick syntax return false : nick ok" << RESET << std::endl;
	return false;
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
		return ft_error(ERR_NONICKNAMEGIVEN, client, NULL, "");
		//return ft_error(ERR_NONICKNAMEGIVEN, params, client, NULL, client_list, *channel_list);
	}

	if (ft_nick_syntax_error(params[1]) == true)
		return (ft_error(ERR_ERRONEUSNICKNAME, client, NULL, params[1]));

  if (ft_nickname_exist(client_list, params[1]))
	{
		return ft_error(ERR_NICKNAMEINUSE, client, NULL, params[1]);
		//return ft_error(ERR_NICKNAMEINUSE, params, client, NULL, client_list, *channel_list);
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
		return ft_error(ERR_ALREADYREGISTERED, client, NULL, "");
	//return ft_error(ERR_ALREADYREGISTERED, params, client, NULL, client_list, *channel_list);
	if (params.size() < 5)
		return (ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]));
	//return ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list);	
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
		vector->push_back(tmp);
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

	if (new_chan->getStatusTopic() == true)
		ft_reply(RPL_TOPIC, client, new_chan, "");
	//Mis en commentaire car libera chat n'envoie que si le topic existe
	//else
	//	ft_reply(RPL_NOTOPIC, client, new_chan, ""); 
	ft_reply(RPL_NAMEREPLY, client, new_chan, ""); 
	ft_reply(RPL_ENDOFNAMES, client, new_chan, ""); 
}

void leaveChannel(Channel* channel, Client *client, std::string message, std::vector<Channel*>* channel_list)
{
	client->remove_channel(channel);

	std::string rpl;
	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " PART " + channel->getName() + " " + message + "\r\n";
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

	if (channel->getNbMembers() == 0) // si jamais je suis le dernier membre
	{	
		std::cout << YELLOW << client->getNickname() << " is the last member of this channel" << RESET << std::endl;
		std::vector<Channel*>::iterator it = channel_list->begin();
		std::vector<Channel*>::iterator ite = channel_list->end();
		while (it != ite)
		{
			if ((*it)->getName() == channel->getName())
			{
				std::cout << RED << (*it)->getName() << " is deleted" << RESET << std::endl;
				delete (*it);
				channel_list->erase(it);
				return; // pas de message a envoyer
			}
			it++;
		}
	}	
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
		ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]);
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
			/* Pas utlisé car ban pas suicvi
			   if (tmp->isUserBanned(client) == 1)//user banned
			   {
			   std::cout << RED << "You are banned from  channel" <<  tmp->getName() << RESET <<  std::endl;
			   ft_error(ERR_BANNEDFROMCHAN, params, client, tmp, client_list, *channel_list); // ERR_BANNEDFROMCHANNEL
			   }
			   */
			if(tmp->isUserMember(client) == 1)
			{
				std::cout << RED << client->getNickname() << " is already a client of channel " <<  tmp->getName() << RESET << std::endl;
				//pas d'erreurs retournée
			}
			else if (client->getNbChannels() == USER_MAXCHAN)
				ft_error(ERR_TOOMANYCHANNELS, client, tmp, ""); //a faire une fois au debute ou a chaque fois?
			else if(tmp->getNbMembers() == CHAN_MAXCAPACITY)
			{
				ft_error(ERR_CHANNELISFULL, client, tmp, "");
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
							ft_error(ERR_BADCHANNELKEY, client, tmp, ""); 
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
						ft_error(ERR_BADCHANNELKEY, client, tmp, ""); 
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
						leaveChannel(*itl, client, client->getNickname(), channel_list);
					}
					itl++;
				}
			}
			else
			{
				if (isValidChanName(*itn) == 0) //not a channel name
				{
					std::cout << RED << *itn << " is not a valid Channel name" << RESET << std::endl;
					ft_error(ERR_BADCHANMASK, client, tmp, *itn); 			
				}
				else
				{
					std::cout << GREEN << "Channel to create!" << RESET << std::endl;

					Channel* new_chan = new Channel(*itn, client);
					new_chan->present();
					if (client->getNbChannels() == USER_MAXCHAN)
					{
						ft_error(ERR_TOOMANYCHANNELS, client, new_chan, ""); //a faire une fois au debute ou a c
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
		ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]);
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
	std::string message;
	if (it != ite)
	{
		while (it != ite)
		{
			message += ((*it) + " ");
			it++;
		}
		message.erase(message.size() - 1);
	}
	else
		message = client->getNickname();

	//Juste pour imprimer

	print_vector(channels_name);
	std::cout << "Part message: " << message << std::endl;

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
				ft_error(ERR_NOTONCHANNEL, client, tmp, ""); 
			}
			else
			{	
				std::cout << YELLOW << client->getNickname() << " is leaving channel " <<  tmp->getName() << RESET << std::endl;
				leaveChannel(tmp, client, message, channel_list);
			}	
		}
		else // no such channel
		{
			ft_error(ERR_NOSUCHCHANNEL, client, NULL, *itn); 
		}
		itn++;
	}
}

// ******** KICK *************

//preque pareil que leave maus le message ets != --> KICK au lieu de PART
void kickChannel(Channel* channel, Client *client, Client* to_kick, std::string message, std::vector<Channel*>* channel_list)
{
	to_kick->remove_channel(channel);
	std::string rpl;
	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " KICK " + channel->getName() + " " + to_kick->getNickname() + " " + message + "\r\n";
	std::vector<Client*> tmp = channel->getMemberList();
	std::vector<Client*>::iterator it = tmp.begin();
	std::vector<Client*>::iterator ite = tmp.end();
	while (it != ite)
	{
		send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
		it++;
	}
	channel->removeMember(to_kick);
	channel->removeOp(to_kick); // si jamais le client est Operateur

	if (channel->getNbMembers() == 0) // si jamais je suis le dernier membre et que je m'autoexpulse
	{	
		std::cout << YELLOW << to_kick->getNickname() << " is the last member of this channel" << RESET << std::endl;
		std::vector<Channel*>::iterator it = channel_list->begin();
		std::vector<Channel*>::iterator ite = channel_list->end();
		while (it != ite)
		{
			if ((*it)->getName() == channel->getName())
			{
				std::cout << RED << (*it)->getName() << " is deleted" << RESET << std::endl;
				delete (*it);
				channel_list->erase(it);
				return; // pas de message a envoyer
			}
			it++;
		}
	}	
	return;
}
void Commands::kick(std::vector<std::string> params, CMD_PARAM)
{
	std::cout << YELLOW << "Hello from KICK function!" << RESET << std::endl;
	(void)client;
	(void)client_list;
	(void)channel_list;
	(void)params;

	if (params.size() <= 2)
	{
		std::cout << "Error : KICK lack of params" << std::endl;
		ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]);
		//		ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list); 
		return;
	}

	//On parse le 2e arg de params (channel names)
	std::vector<std::string>::iterator it = params.begin();
	std::vector<std::string>::iterator ite = params.end();
	it++;
	std::vector<std::string> channels_name;
	commaSplit(*it, &channels_name);

	//On parse le 3e arg de params (user names)
	it++;
	std::vector<std::string> users_name;
	commaSplit(*it, &users_name);

	//On cherche un potentiel commentaire
	it++;
	std::string kick_comment;
	if(it == ite)
		std::string kick_comment = "";
	else
	{
		while (it != ite)
		{
			kick_comment += ((*it) + " ");
			it++;
		}
		kick_comment.erase(kick_comment.size() - 1);
	}

	//Juste pour imprimer
	print_vector(channels_name);
	print_vector(users_name);
	std::cout << "Kick comment: " << kick_comment << std::endl;

	//On s'occupe des noms de chan un a un
	std::vector<std::string>::iterator itc = channels_name.begin();
	std::vector<std::string>::iterator itce = channels_name.end();
	std::vector<std::string>::iterator itu = users_name.begin();
	std::vector<std::string>::iterator itue = users_name.end();

	while (itc != itce)
	{
		Channel* tmp_channel = ft_channel_exist(*channel_list, *itc);
		if (tmp_channel) 
		{			
			std::cout << GREEN << "The Channel to leave \"" << tmp_channel->getName() << "\" exists" << RESET << std::endl;
			if(tmp_channel->isUserMember(client) == 0)
			{
				std::cout << RED << client->getNickname() << " is not a client of channel " <<  tmp_channel->getName() << RESET << std::endl;
				ft_error(ERR_NOTONCHANNEL, client, tmp_channel, ""); 
			}
			else if(tmp_channel->isUserOp(client) == 0)// si le client faisant kick nest pas op du cannal
			{
				std::cout << RED << client->getNickname() << " is not an operator of channel " <<  tmp_channel->getName() << RESET << std::endl;
				ft_error(ERR_CHANOPRIVNEEDED, client, tmp_channel, ""); //  ERR_NOTONCHANNEL
			}
			else
			{	
				std::cout << YELLOW << client->getNickname() << " is an operator of channel " <<  tmp_channel->getName() << RESET << std::endl;
				if (itu != itue)
				{
					Client* tmp_client = ft_nickname_exist_return(client_list, *itu);
					if (tmp_client)
					{
						if (tmp_channel->isUserMember(tmp_client) == 0) //
						{
							std::cout << RED << tmp_client->getNickname() << " is not a member  of channel " <<  tmp_channel->getName() << RESET << std::endl;
							ft_error(ERR_USERNOTINCHANNEL, client, tmp_channel, *itu); 
						}
						else
						{
							std::cout << GREEN << client->getNickname() << " is forcing " << tmp_client->getNickname() << " to leave channel " <<  tmp_channel->getName() << RESET << std::endl;
							if (kick_comment == "")
								kick_comment = tmp_client->getNickname();
							kickChannel(tmp_channel, client, tmp_client, kick_comment, channel_list);
						}
					}
					else
					{
						std::cout << RED << " The  client does not exist" << RESET << std::endl;
						ft_error(ERR_USERNOTINCHANNEL, client, tmp_channel, *itu);
					}
					itu++;
				}		
			}	
		}
		else // no such channel
		{
			ft_error(ERR_NOSUCHCHANNEL, client, NULL, *itc); 
		}
		itc++;
	}
}


// ******** TOPIC *************

void Commands::topic(std::vector<std::string> params, CMD_PARAM)
{
	(void)channel_list;
	(void)client_list;

	std::cout << YELLOW << "Hello from TOPIC function!" << RESET << std::endl;

	if (params.size() == 1)
	{
		std::cout << "Error : TOPIC lack of params" << std::endl;
		ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]);
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
			ft_error(ERR_NOTONCHANNEL, client, NULL, ""); 
			return;
		}
		it++;
		if (it == ite) // Voir le sujet du cannal
		{
			std::cout << YELLOW <<  "Just here to see the TOPIC of channel " << tmp->getName() << RESET << std::endl;
			if (tmp->getStatusTopic() == false)
			{
				std::cout << RED <<  "This Channel has no topic"  << RESET << std::endl;
				ft_reply(RPL_NOTOPIC, client, tmp, "");
			}
			else
			{
				std::cout << YELLOW <<  "This Channel topic is "  << tmp->getTopic() << RESET << std::endl;
				ft_reply(RPL_TOPIC, client, tmp, "");
			}
		}
		else // creer ou change le sujet du cannal
		{
			std::string topic;
			while (it != ite)			
			{
				topic += (*it + " ");
				it++;
			}
			topic.erase(topic.end() -1);
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
			}
			tmp->setTopic(topic);
		}
	}
	else
	{
		std::cout << RED << "The Channel does not exists" << RESET << std::endl;
		// AJOUT pas sure que ce soit a faire
		ft_error(ERR_NOSUCHCHANNEL, client, NULL, *it);
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

			ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]);
		else if (params.size() == 2)
			ft_error(ERR_NOTEXTTOSEND, client, NULL, "");
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
				if (tmp_client->getAway() == true)
				{
					std::cout << YELLOW << "Dest is away" << RESET << std::endl;
					std::string arg = tmp_client->getNickname() + " :" + tmp_client->getAwayMessage();
					ft_reply(RPL_AWAY, client, NULL, arg); //tmp_params, client, NULL, client_list, *channel_list);
				}
			}
		}
		else // si je suis un cannal
		{
			Channel* tmp_channel = ft_channel_exist(*channel_list, *itd);
			if (tmp_channel) // si je suis un cannal
			{
				std::cout << "I am a channel" << std::endl;
				if (tmp_channel->isUserMember(client) == 0) //si je ne fais pas parti de ce cannal
				{
					std::cout << RED << client->getNickname() << " is not a member of " << tmp_channel->getName() << RESET << std::endl;
					ft_error(ERR_CANNOTSENDTOCHAN, client, tmp_channel, "");
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
				//std::vector<std::string> tmp_params;
				//tmp_params.push_back(*itd);
				ft_error(ERR_NOSUCHNICK, client, NULL, *itd);
				// ft_error(ERR_NOSUCHNICK, tmp_params, client, NULL, client_list, *channel_list);
			}
		}	
		itd++;
	}
}

// ******** NOTICE *************
void send_notice_user(Client *client, std::string message, Client* user)
{
	std::string rpl;

	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " NOTICE " + user->getNickname() + message;
	send(user->getSocket(), (rpl.c_str()), rpl.size(), 0);;
}
// Meme chose que privmsg mais on n'envoie aucune erreur

void send_notice_channel(Client *client, std::string message, Channel* channel)
{
	std::vector<Client*> tmp = channel->getMemberList();
	std::vector<Client*>::iterator it = tmp.begin();	
	std::vector<Client*>::iterator ite = tmp.end();

	std::string rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " NOTICE " + channel->getName() + message;
	while (it != ite)
	{
		if (*it != client) // si je ne suis pas l'envoyeur
		{    
			send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
		}
		it++;
	}
}

void Commands::notice(std::vector<std::string> params, CMD_PARAM)
{
	std::cout << YELLOW << "Hello from NOTICE function!" << RESET << std::endl;

	if (params.size() <= 2)
	{
		std::cout << "Error : NOTICE lack of params" << std::endl;
		/*
		   if (params.size() == 1)
		   ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list); 
		   else if (params.size() == 2)
		   ft_error(ERR_NOTEXTTOSEND, params, client, NULL, client_list, *channel_list); 
		   */
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
				send_notice_user(client, message, tmp_client);
			}
		}
		else // si je suis un cannal
		{
			Channel* tmp_channel = ft_channel_exist(*channel_list, *itd);
			if (tmp_channel) // si je suis un cannal
			{
				std::cout << "I am a channel" << std::endl;
				if (tmp_channel->isUserMember(client) == 0) //si je ne fais pas parti de ce cannal
				{
					std::cout << RED << client->getNickname() << " is not a member of " << tmp_channel->getName() << RESET << std::endl;
					return;
				}
				else
				{
					std::cout << GREEN << "Message from " << client->getNickname() << " to " << tmp_channel->getName() <<  " : " << message << RESET << std::endl;
					send_notice_channel(client, message, tmp_channel);
				}
			}
			else
			{
				std::cout << RED << "Not a client or a channel" << RESET << std::endl;
				/*
				   std::vector<std::string> tmp;
				   tmp.push_back(*itd);
				   ft_error(ERR_NOSUCHNICK, params, client, NULL, client_list, *channel_list); 
				   */
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

static Channel*	matchChannel(std::vector<std::string> params, CMD_PARAM) {
	std::vector<Channel*>::iterator it = channel_list->begin();
	int i = 0;
	(void)client_list;
	(void)client;

	while (it != channel_list->end()) {
		if (!params[1].compare((*it)->getName())) {
			return (*it);
		}
		i++;
		it++;
	}
	return (NULL);
}

static void	printWho(std::vector<std::string> params, Channel *channel, Client *client, Client* client_ref, int user) {
	std::string rpl;
	//if params[1] == nom -> * 
	//if params[1] == channel -> #nom de channel
	std::cout << "print WHO" << std::endl;
	if (user)
		rpl = "*";
	else 
		rpl = params[1];
	rpl += " " + client_ref->getUsername() + " ";
	rpl += client_ref->getServerIpAddress() + " ";
	rpl += client_ref->getServerName() + " " + client_ref->getNickname();
	// lettre change en fonction du mode away (" H")/(" G") - user operator ("*")/ member operator ("@")
	if (client_ref->getAway())
		rpl += " G ";
	else
		rpl += " H";
	if (channel->isUserOp(client_ref))
		rpl += "@";
	// Adrien -> fait la fonction away qui va me donner les infos
	rpl += " :0 " + client_ref->getRealname() + "\r\n"; // + real name
	ft_reply(RPL_WHOREPLY, client, NULL, rpl);
	/* send(client->getSocket(), (rpl.c_str()), rpl.size(), 0); */
}

static	int	matchOthers(std::vector<std::string> params, CMD_PARAM) {
	std::vector<Client*>::iterator it = client_list.begin();
	(void)channel_list;
	(void)client;

	while (it != client_list.end()) {
		if (!params[1].compare((*it)->getUsername())) {
			printWho(params, NULL, client,  *it, 1);
		}
		else if (!params[1].compare((*it)->getRealname())) {
			printWho(params, NULL, client, *it, 1);
		}
		else if (!params[1].compare((*it)->getNickname())) {
			printWho(params, NULL, client, *it, 1);
		}
		it++;
	}
	ft_reply(RPL_ENDOFWHO, client, NULL, "");
	return false;
}

static void	printChan(std::vector<std::string> params, Client* client, Channel* channel, int isOp) {
	std::vector<Client*>::const_iterator it = channel->getMemberList().begin();
	while (it != channel->getMemberList().end()) {
		if (isOp) {
			printWho(params, channel, client, *it, 1);
		}
		else 
			printWho(params, channel, client, *it, 1);
		it++;
	}
	ft_reply(RPL_ENDOFWHO, client, NULL, "");
}

static void	printWhoAllClient(std::vector<std::string> params, Client* client, std::vector<Client *> client_list) {
	std::vector<Client*>::iterator it = client_list.begin();
	while (it != client_list.end()) {
		printWho(params, NULL, client, *it, 1);
		it++;
	}
	ft_reply(RPL_ENDOFWHO, client, NULL, "");
}

void Commands::who(std::vector<std::string> params, CMD_PARAM) {
	std::cout << YELLOW << "Hello from Who function!" << RESET << std::endl;

	if (params.size() == 1) {
		printWhoAllClient(params, client, client_list);
		return ;
	}
	// on cheche si un cannal correspond au second parametre de who 
	Channel *tmp = matchChannel(params, client, client_list, channel_list);
	if (!tmp) {
		//on regarde si le parametre de who correspond a n importe quoi d autre
		if (matchOthers(params, client, client_list, channel_list) == 0) {
			printWhoAllClient(params, client, client_list);
			return ;
		}
		return ;
	}
	//on print tout les membres du cannal trouvé
	if (params.size() == 3 && !params[2].compare("O")) {
		printChan(params, client, tmp, 1);
	}
	else 
		printChan(params, client, tmp, 0);
	//si o imprimer les _operator sinon _members
}

/* ************************************NAMES****************************************** */

std::vector<std::string>	split(const std::string &s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss (s);
	std::string item;

	while (getline(ss, item, delim)) { 
		result.push_back(item);
	}
	return result;
}

static int	matchNamesChannel(std::string params, CMD_PARAM) {
	std::vector<Channel*>::iterator it = channel_list->begin();
	int i = 0;
	(void)client_list;
	(void)client;

	while (it != channel_list->end()) {
		if (!params.compare((*it)->getName())) {
			return i;
		}
		i++;
		it++;
	}
	return (-1);
}

void Commands::names(std::vector<std::string> params, CMD_PARAM) {
	std::cout << YELLOW << "Hello from Names function!" << RESET << std::endl;
	(void)client;
	(void)client_list;
	(void)channel_list;
	//la commande NAMES seule est renvoyer par hexchat comme NAMES + IRC_90's
	//split "," pour avoir tout les chans
	std::vector<std::string> v = split(params[1], ',');
	std::vector<std::string>::iterator vit = v.begin();

	while (vit != v.end()) {
		int num = matchNamesChannel(*vit, client, client_list, channel_list);
		if (num >= 0) {
			(*channel_list)[num]->printMembersNick(client);
			ft_reply(RPL_ENDOFNAMES, client, (*channel_list)[num], "");
		}
		else { 
			ft_reply(RPL_ENDOFNAMES, client, NULL , "");
		}
		vit++;
	}
}

/* ************************************LIST****************************************** */

void Commands::list(std::vector<std::string> params, CMD_PARAM) {
	std::cout << YELLOW << "Hello from list function!" << RESET << std::endl;
	(void)client;
	(void)params;
	(void)client_list;
	(void)channel_list;
	std::vector<Channel*>::iterator it = channel_list->begin();

	while (it != channel_list->end()) {
		ft_reply(RPL_LIST, client, *it, "");
		it++;
	}
	ft_reply(RPL_LISTEND, client, NULL, "");
}


/* ************************************HELP****************************************** */
void Commands::myhelp(std::vector<std::string> params, CMD_PARAM) 
{
	(void)channel_list;
	(void)client_list;
	std::cout << YELLOW << "Hello from MYHELP function!" << RESET << std::endl;
	if (params.size() != 1) 
	{
		ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]);
		return ;
	}
	ft_reply(RPL_CUSTOMHELP, client, NULL, "");
}

/* ************************************QUIT****************************************** */
void Commands::quit(std::vector<std::string> params, CMD_PARAM)
{
	(void)client_list;

	std::cout << YELLOW << "Hello from QUIT function!" << RESET << std::endl;
	std::string message; 
	if (params.size() == 1) 
		message = ""; 
	else
	{	
		std::vector<std::string>::iterator it = params.begin();
		std::vector<std::string>::iterator ite = params.end();
		it++;
		while(it != ite)
		{
			message += (*it + " ");
			it++;
		}
		message.erase(message.end() - 1);
	}	

	//leave all channels
	std::cout << RED << client->getNickname() << " must leave all channels!" << RESET << std::endl;
	client->leaveAllChannels();
	std::vector<Channel*>::iterator itl = channel_list->begin();
	std::vector<Channel*>::iterator itle = channel_list->end();
	while (itl != itle)
	{
		if ((*itl)->isUserMember(client) == 1)
		{
			leaveChannel(*itl, client, message, channel_list);
			//le quit message du channel se fait la
		}
		itl++;
	}

	client->setMessageStatus(DISCONNECT);
	//disconnect server
	//close la socket? remove client?
}

//*****************KILL******* (Available for OPER only)

void	Commands::kill(std::vector<std::string> params, CMD_PARAM)
{
	(void)client_list;
	(void)channel_list;

	std::cout << YELLOW << "Hello from KILL function!" << RESET << std::endl;
	if (params.size() < 3)
		return (ft_error(ERR_NEEDMOREPARAMS, client, NULL, params[0]));
	std::cout << YELLOW << "Hello from KILL function!" << RESET << std::endl;
	if (client->getOper() == false)
		return (ft_error(ERR_NOPRIVILEGES, client, NULL, ""));
	std::cout << YELLOW << "Hello from KILL function!" << RESET << std::endl;

	Client *client_to_kill = ft_nickname_exist_return(client_list, params[1]);
	std::cout << YELLOW << "Hello from KILL function!" << RESET << std::endl;
	if (client_to_kill == NULL)
		return (ft_error(ERR_NOSUCHNICK, client, NULL, params[1]));
	std::cout << YELLOW << "Hello from KILL SUCCESS!" << RESET << std::endl;
	std::string reason;
	for (std::vector<std::string>::iterator it = params.begin() + 2; it != params.end(); it++)
	{
		reason += *it + " ";
	}
	std::string rpl;


	// AJOUT JOANN

	client_to_kill->leaveAllChannels();
	std::vector<Channel*>::iterator itl = channel_list->begin();
	std::vector<Channel*>::iterator itle = channel_list->end();
	while (itl != itle)
	{
		if ((*itl)->isUserMember(client_to_kill) == 1)
			leaveChannel(*itl, client_to_kill, reason, channel_list);
		itl++;
	}

	// FIN AJOUT JOANN

	rpl = "Operator(" + client->getNickname() + ") KILLED you for " + reason + "\r\n";
	send(client_to_kill->getSocket(), rpl.c_str(), rpl.size(), 0);
	client_to_kill->setMessageStatus(DISCONNECT);
}
