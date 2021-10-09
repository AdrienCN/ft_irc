#include "Commands.hpp"

Commands::Commands() 
{
	_cmd_list["PASS"] = &Commands::pass;
	_cmd_list["NICK"] = &Commands::nick;
	_cmd_list["USER"] = &Commands::user;
	_cmd_list["JOIN"] = &Commands::join;
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
	// do the action
		//if success
			//RPL_XXX
		
		//else
			//ERR_XXX
    std::cout << YELLOW << "Hello from PASS function!" << RESET << std::endl;
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


void Commands::nick(std::vector<std::string> params, CMD_PARAM)
{
	(void)client;
    (void)client_list;
    (void)channel_list;
	(void)params;

    std::cout << YELLOW << "Hello from NICK function!"<< RESET  << std::endl;
	//Pas de pseudo donne
	if (params.size() == 0 || params.size() == 1)
	{
		//ft_error(431);
		std::cout << "Error : nick : 431" << std::endl;
		return;
	}
		//Pseudo contient des chars non autorise 
		//	ft_error(432)a coder ??
		//Pseudo exist deja
	else if (ft_nickname_exist(client_list, params[1]))
	{
		//ft_error(433);
		std::cout << "Error : nick : 433" << std::endl;
		return;
	}
	else
	{
		std::string rpl;
		rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " NICK " + params[1] + "\r\n";
		send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
		client->incrGreetings();

		//send reply ?
	}
	client->setNickname(params[1]);
}

// ******** USER *************

int		ft_username_exist(std::vector<Client*> client_list, std::string username)
{
	std::vector<Client*>::iterator it = client_list.begin();

	while (it != client_list.end())
	{
		if ((*it)->getUsername() == username)
			return (1);
		it++;
	}
	return (0);
}

void Commands::user(std::vector<std::string> params, CMD_PARAM)
{
    (void)client;
    (void)client_list;
    (void)channel_list;
	(void)params;
	
	if (params.size() == 1)
		std::cout << "Error : username lack of params" << std::endl;
	else if (ft_username_exist(client_list, params[0]))
		std::cout << "Error : username exist already" << std::endl;
	else
	{
		client->setUsername(params[1]);
		if (client->getGreetings() > 2)
		{
			std::string tmp("Your new USERNAME is ");
			tmp += client->getUsername() + "\n";
			send(client->getSocket(), (tmp.c_str()), tmp.size(), 0);
		}
		else
			client->incrGreetings();
	}

	std::cout << YELLOW << "Hello from USER function!" << RESET << std::endl;
}




// ******** JOIN *************

int isValidChanName(std::string channel_name)
{
	if (channel_name == "\0") // + ajouter els regles d'un nom de channel
		return (0);
	else
		return (1); 
}

void  joinSplit(std::string str, std::vector<std::string>* vector)
{	
		std::istringstream str2(str);
		std::string tmp;

		while (std::getline(str2, tmp,','))
		{
			if (isValidChanName(tmp))
				vector->push_back(tmp);
			tmp.clear(); // on enleve tout le contenu et size = 0
		}
}

Channel* ft_channel_exist(std::vector<Channel*> channel_list, std::string channel)
{
	std::vector<Channel*>::iterator it = channel_list.begin();

	while (it != channel_list.end())
	{
		if ((*it)->getName() == channel)
			return (*it);
		it++;
	}
	return (NULL);
}

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
		//ERR NEED MORE PARAMS
		return;
	}
	
	//On parse le 2e arg de params (channel name) et le 3e (les keys potentiellement)
	std::vector<std::string>::iterator it = params.begin();
	it++;
	std::vector<std::string> channels_name;
	joinSplit(*it, &channels_name);
	it++;
	std::vector<std::string> channels_key;
	if (it != params.end())
		joinSplit(*it, &channels_key);
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
			if (tmp->isUserBanned(client) == 1)
			{
				std::cout << RED << "You are banned from  channel" <<  tmp->getName() << RESET <<  std::endl;
				//user banned
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
						}
						else
						{
							std::cout << GREEN << *itk << " was the right key! Welcome to Channel " << tmp->getName() << RESET << std::endl;
							client->add_channel(tmp);
							tmp->addMember(client);
						}	
						itk++;
					}
					else
					{
						std::cout << RED <<  "The channel " << tmp->getName() << " needs a key"  << RESET << std::endl;
					}
				}
				else
				{
					std::cout << GREEN << "Welcome to Channel " << tmp->getName() << RESET<< std::endl;
					client->add_channel(tmp);
					tmp->addMember(client);
					std::string rpl;
					rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " JOIN " + tmp->getName() + "\r\n";
					send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
					//message to all members
				}	
			}	
		}
		else // create channel
		{
			if (*itn == "0") // must be like a part command --> a revoir
			{
				std::cout << RED << client->getNickname() << " must leave all channels!" << RESET << std::endl;
				client->leaveAllChannels();
				std::vector<Channel*>::iterator itl = channel_list->begin();
				std::vector<Channel*>::iterator itle = channel_list->end();
				while (itl != itle)
				{
					if ((*itl)->isUserMember(client) == 1)
						(*itl)->removeMember(client);
						/*
						if (*itl)->getNbMembers() == 0)
						{
							channel_list->removeChannel((*itl->getName());
							delete *itl;
						}
						*/
					itl++;
				}
				// Que faire dans le cas ou le client est l'operateur ?? a voir 
				// Dans le cas ou le client est tout seul sur son cannal?
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
				client->add_channel(new_chan);

				std::string rpl;
				rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " JOIN " + new_chan->getName() + "\r\n";
				send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
			}

			//send(client->getSocket(), (tmp.c_str()), tmp.size(), 0);
			//send RPL topic
		}
		itn++;
	}
}



/*
void Commands::quit(std::vector<std::string> params)
{
    (void)params;
    std::cout << "Hello from quit function!" << std::endl;
}
*/
