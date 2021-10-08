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


void Commands::find_command(std::vector<std::string> input, Client* client, std::vector<Client*> client_list, std::vector<Channel*> channel_list) 
{

	(void)client;
    (void)client_list;
    (void)channel_list;
   
	std::string key(input[0]);	

	//std::map<std::string , void(Commands::*)(std::vector<std::string>)>::iterator it = _cmd_list.begin();
	if (_cmd_list[key])
		(this->*_cmd_list[key])(input, client, client_list, channel_list);
	else
		std::cout << "Command not found(Adrien)" << std::endl;

	/* OPTION JOANN
    (void)client;
    (void)client_list;
    (void)channel_list;
    
    std::vector<std::string>::iterator it = input.begin();
    std::vector<std::string>::iterator ite = input.end();

    std::string prefix; // je pense pas qu'on en ai besoin
    std::string command;

    if ((*it)[0] == ':') // si je suis un prefix
    {
        prefix.assign(*it);
        input.erase(it);
        it = input.begin(); // on remet les iterateurs à jour
        ite = input.end();
    } 
    command.assign(*it);
    it++;
    std::vector<std::string> params(it, ite); // on construit les params

    print_command(prefix, command, params);

    // Je voulais faire un switch mais j'ai pas trouvé avec de strings, a voir
    if (command == "JOIN")
        join(params);
    else
        std::cout << "Command not found" << std::endl;
	*/

}


void Commands::join(std::vector<std::string> params, Client *client, std::vector<Client*> client_list, std::vector<Channel*> channel_list)
{
	(void)client;
    (void)client_list;
    (void)channel_list;
	(void)params;

    std::cout << YELLOW << "Hello from join function!" << RESET << std::endl;
}

void Commands::pass(CMD_PARAM)
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

void Commands::nick(CMD_PARAM)
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
	}
		//Pseudo contient des chars non autorise 
		//	ft_error(432)a coder ??
		//Pseudo exist deja
	else if (ft_nickname_exist(client_list, params[1]))
	{
		//ft_error(433);
		std::cout << "Error : nick : 433" << std::endl;
	}
	else
	{
		if (client->getGreetings() > 2)
		{
		/*	std::string tmp;
			tmp += client->getNickname() + "\n";
			send(client->getSocket(), (tmp.c_str()), tmp.size(), 0);
		*/
			std::string rpl;
			rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + " NICK " + params[1] + "\r\n";
			send(client->getSocket(), (rpl.c_str()), rpl.size(), 0);
		}
		else
			client->incrGreetings();
		//send reply ?
	}
	client->setNickname(params[1]);
}

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

void Commands::user(CMD_PARAM)
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



/*
void Commands::quit(std::vector<std::string> params)
{
    (void)params;
    std::cout << "Hello from quit function!" << std::endl;
}
*/
