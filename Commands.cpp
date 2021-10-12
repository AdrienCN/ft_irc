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

Commands::Commands(std::string const & password) 
{
	_server_password = password;
	_cmd_list["CAP"] = &Commands::cap;
	_cmd_list["PASS"] = &Commands::pass;
	_cmd_list["NICK"] = &Commands::nick;
	_cmd_list["USER"] = &Commands::user;
	_cmd_list["JOIN"] = &Commands::join;
	_cmd_list["PART"] = &Commands::part;
	_cmd_list["TOPIC"] = &Commands::topic;
	_cmd_list["PRIVMSG"] = &Commands::privmsg;
    return;
}

Commands::~Commands()
{
    return;
}

//************ PARSING ET TRI DE LA STR COMMAND EN VECTOR ********************

void Commands::find_command(std::string input, Client* client, std::vector<Client*> client_list, std::vector<Channel*>* channel_list) 
{
	// A verifier si on doit gérer les prefixes ":" en premier message (mais je pense pas)
	(void)client;
    (void)client_list;
    (void)channel_list;
   
	this->analyzeCommand(input);
	std::string key(_parsed_cmd.front());
	//std::map<std::string , void(Commands::*)(std::vector<std::string>)>::iterator it = _cmd_list.begin();
	if (_cmd_list[key])
		(this->*_cmd_list[key])(_parsed_cmd, client, client_list, channel_list);
	else
		std::cout << "Command not found(Adrien)" << std::endl;
	this->_parsed_cmd.clear();
}

void Commands::analyzeCommand(std::string command) // Parsing des whitespaces
{	
		std::istringstream str(command);
		std::string tmp;

		/*
		if (command.length() > 512) // 512 en incluant \r\n
		{
			//Commande trop longue, quelle erreur a manager?
		}
		*/

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
				_parsed_cmd.push_back(tmp);
				tmp.clear(); // on enleve tout le contenu et size = 0
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
//	if (params[1] == this->_server_password)
//	{
	if (client->getRegNick() == true || client->getRegUser() == true)
	{
		std::cout << RED << "Error : Pass : /Pass must be register before /NICK or /USER\r\n" << RESET << std::endl;
		return;
	}
		std::cout << GREEN << "New password saved successfully" << RESET << std::endl;
		client->setPassword(params[1]);
		client->setRegPass(true);
//	}
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

bool	ft_strRespectCharset(std::string str)
{
	std::string::iterator itb = str.begin();
	while (itb != str.end())
	{
		if (std::isprint(*itb) == false)
			return (false);
		itb++;
	}
	return (true);
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
	//Nickname contient des non_printables ou ne respect pas le charset 
	if (ft_strRespectCharset(params[1]) == false)
		return ft_error(ERR_ERRONEUSNICKNAME, params, client, NULL, client_list, *channel_list);
			//Pseudo exist deja
	if (ft_nickname_exist(client_list, params[1]))
	{
		return ft_error(ERR_NICKNAMEINUSE, params, client, NULL, client_list, *channel_list);
	}
	std::string rpl;
	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " NICK " + params[1] + "\r\n";
	send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
	client->setNickname(params[1]);
	client->setRegNick(true);
}

// ******** USER *************

void Commands::user(std::vector<std::string> params, CMD_PARAM)
{
    (void)client;
    (void)client_list;
    (void)channel_list;
	(void)params;
	
	std::cout << YELLOW << "Hello from USER function!" << RESET << std::endl;
	if (client->isRegistered() == true)
		return ft_error(ERR_ALREADYREGISTERED, params, client, NULL, client_list, *channel_list);
	if (params.size() < 3)
		return ft_error(ERR_NEEDMOREPARAMS, params, client, NULL, client_list, *channel_list);

	
	if (client->getRegUser() == true)
	{
		std::cout << RED << "Error: USER : Username already set\r\n" << RESET << std::endl;
		return;
	}
//	if (client->getRegUSER() == true && client->getRegNick() == true)
//	{
//		
		client->setUsername(params[1]);
		std::cout << GREEN << "Username successfully registered\r\n" << RESET << std::endl;
		client->setRegUser(true);
//	}
}

// ******** Channel Functions *************

int isValidChanName(std::string channel_name)
{
	if (channel_name == "\0") // + ajouter els regles d'un nom de channel
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
				if (tmp[0] == '#' || tmp[0] == '&') // HEXCHAT envoie le nom de channel avec ces args
					tmp.erase(0, 1);
				vector->push_back(tmp);
			}
			tmp.clear(); // on enleve tout le contenu et size = 0
		}
}

Channel* ft_channel_exist(std::vector<Channel*> channel_list, std::string channel)
{
	std::vector<Channel*>::iterator it = channel_list.begin();

	if (channel[0] == '#' || channel[0] == '&')  // HEXCHAT envoie le nom de channel avec ces args
		channel.erase(0, 1);

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
	send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
	if (new_chan->getStatusTopic() == false)
		ft_reply(331, params, client, new_chan, client_list, *channel_list); // RPL_NOTOPIC
	else
		ft_reply(332, params, client, new_chan, client_list, *channel_list); // RPL_TOPIC
	ft_reply(353, params, client, new_chan, client_list, *channel_list); // RPL_NAMEREPLY
}

void leaveChannel(Channel* channel, Client *client, std::string part_message)
{
	client->remove_channel(channel);
	channel->removeMember(client);

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
	send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
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
		ft_error(461, params, client, NULL, client_list, *channel_list); //  ERR_NEEDMOREPARAMS
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
				ft_error(474, params, client, tmp, client_list, *channel_list); // ERR_BANNEDFROMCHANNEL
			}
			else if(tmp->isUserMember(client) == 1)
			{
				std::cout << RED << client->getNickname() << " is already a client of channel " <<  tmp->getName() << RESET << std::endl;
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
							ft_error(475, params, client, tmp, client_list, *channel_list); // ERR_BADCHANNELKEY
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
						ft_error(461, params, client, tmp, client_list, *channel_list); //  ERR_NEEDMOREPARAMS (ou bad channel key?)
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
				std::cout << GREEN << "Channel to create!" << RESET << std::endl;

				Channel* new_chan = new Channel(*itn, client);
				new_chan->present();

				if (itk != itke)
				{
					new_chan->setKey(*itk);
					itk++;
				}
				channel_list->push_back(new_chan);
				joinChannel(new_chan, params, client, client_list, channel_list);
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
		ft_error(461, params, client, NULL, client_list, *channel_list); //  ERR_NEEDMOREPARAMS
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
	print_vector(channels_name);
	std::cout << "Part message: " << part_message << std::endl;

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
				ft_error(442, params, client, NULL, client_list, *channel_list); //  ERR_NOTONCHANNEL
			}
			else
			{	
				std::cout << YELLOW << client->getNickname() << " is leaving channel " <<  tmp->getName() << RESET << std::endl;
				leaveChannel(tmp, client, part_message);
			}	
		}
		else // no such channel
		{
			ft_error(403, params, client, NULL, client_list, *channel_list); //  ERR_NOSUCHCHANNEL
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
		ft_error(461, params, client, NULL, client_list, *channel_list); //  ERR_NEEDMOREPARAMS
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
			ft_error(442, params, client, NULL, client_list, *channel_list); //  ERR_NOTONCHANNEL
			return;
		}
		it++;
		if (it == ite) // Voir le sujet du cannal
		{
			std::cout << YELLOW <<  "Just here to see the TOPIC of channel " << tmp->getName() << RESET << std::endl;
			if (tmp->getStatusTopic() == false)
			{
				std::cout << RED <<  "This Channel has no topic"  << RESET << std::endl;
				ft_reply(331, params, client, tmp, client_list, *channel_list); //  RPL_NOTOPIC
			}
			else
			{
				std::cout << YELLOW <<  "This Channel topic is "  << tmp->getTopic() << RESET << std::endl;
				ft_reply(332, params, client, tmp, client_list, *channel_list); //  RPL_TOPIC
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
				rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " TOPIC  #" + tmp->getName() + " :" + topic + "\r\n";
				send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
			}
			tmp->setTopic(topic);
		}
	}
	else
	{
		std::cout << RED << "The Channel does not exists" << RESET << std::endl;
		// AJOUT pas sure que ce soit a faire
		ft_error(403, params, client, NULL, client_list, *channel_list); //  ERR_NOSUCHCHANNEL
	}
}

// ******** PRIVMSG *************

void send_privmsg_user(Client *client, std::string message, Client* user)
{
	// Comment faire pour afficher le message???
	std::string rpl;
	//rpl = client->getNickname() + message;
	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " PRIVMSG " + user->getNickname() + message;
	//send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
//	send(user->getSocket(), (rpl.c_str()), rpl.size(), 0);
	//ou 
  	send(user->getSocket(), (rpl.c_str()), rpl.size(), 0);;
}

void send_privmsg_channel(Client *client, std::string message, Channel* channel)
{
	std::vector<Client*> tmp = channel->getMemberList();
	std::vector<Client*>::iterator it = tmp.begin();	
	std::vector<Client*>::iterator ite = tmp.end();

	std::string rpl;
//	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " PRIVMSG #" + channel->getName() + message;
//	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " " + message;
// envoie message sur mauvais socket
//	send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
			
    while (it != ite)
	{
		// Comment faire pour afficher le message???
		if (*it != client) // si je ne suis pas l'envoyeur
		{    
			std::cout << "Message sent to " << (*it)->getNickname() << std::endl;
			//message aux utilisateirs direct
			//rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " PRIVMSG " + (*it)->getNickname() + message;
		//	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " PRIVMSG " + message;
			
			//nothing happends
			rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " PRIVMSG #" + channel->getName() + message;
			//rpl = client->getNickname() + " " + message;
			
			//GARBAGE dans les deux quand 2 fenetres ouverts
			//rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " #" + channel->getName() + message;
			
			//GARBAGE mais dans la convers privée
			//rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0 " + (*it)->getNickname() + message;
			//rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0 :" + (*it)->getNickname() + message;


			//rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " #" + channel->getName() + (*it)->getNickname() + message;
			//rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " #" + channel->getName() + " PRIVMSG " +  (*it)->getNickname() + message;
			//	rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " #" + channel->getName() + " PRIVMSG " + message;
			//rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " " + message;
			//rpl = ":" + client->getNickname() + " " + message;
			

			//rpl = ":" + client->getNickname() + message;

			//rpl = "PRIVMSG #" + channel->getName() + message;
			std::cout << "rpl : |" << rpl << "|" << std::endl;
			send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
			
			//rpl = client->getNickname() + message;
			//send((*it)->getSocket(), (rpl.c_str()), rpl.size(), 0);
			rpl.clear();

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
		    ft_error(461, params, client, NULL, client_list, *channel_list); //  ERR_NEEDMOREPARAMS
		else if (params.size() == 2)
		    ft_error(412, params, client, NULL, client_list, *channel_list); //  ERR_NOTEXTTOSEND
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
		    std::cout << GREEN << "Message from " << client->getNickname() << " to " << tmp_client->getNickname() <<  " : " << message << RESET << std::endl;
		   	send_privmsg_user(client, message, tmp_client);
	    }
		else // si je suis un cannal
		{
			Channel* tmp_channel = ft_channel_exist(*channel_list, *itd);
			if (tmp_channel) // si je suis un cannal
			{
				if (tmp_channel->isUserMember(client) == 0) //si je ne fais pas parti de ce cannal
				{
					std::cout << RED << client->getNickname() << " is not a member of " << tmp_channel->getName() << RESET << std::endl;
					ft_error(404, params, client, tmp_channel, client_list, *channel_list); //  ERR_CANNOTSENDTOCHAN
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
			    ft_error(401, params, client, NULL, client_list, *channel_list); //ERR_NOSUCHNICK
			}
		}	
		itd++;
	}
}

void	Commands::cap(std::vector<std::string> params, CMD_PARAM)
{
	(void)client;
//	(void)channel;
	(void)client_list;
	(void)channel_list;
	(void)params;
	
	std::string cap("CAP REPLY TO DO\r\n");
	std::cout << cap  << std::endl;
//	send(client->getSocket(), cap.c_str(), cap.size(), 0);
}
