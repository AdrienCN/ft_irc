#include "Commands.hpp"

Commands::Commands() 
{
    return;
}

Commands::~Commands()
{
    return;
}


void Commands::find_command(std::vector<std::string> input, Client *client)
{
    (void)client;
    std::vector<std::string>::iterator it = input.begin();
    std::vector<std::string>::iterator ite = input.end();

    std::string prefix; // je pense pas qu'on en ai besoin
    std::string command;

   /* if ((*it)[0] == ':') // si je suis un prefix
    {
        prefix.assign(*it);
        input.erase(it);
        it = input.begin(); // on remet les iterateurs à jour
        ite = input.end();
    }*/
    command.assign(*it);
    it++;
    std::vector<std::string> params(it, ite); // on construit les params

    print_command(prefix, command, params);

    // Je voulais faire un switch mais j'ai pas trouvé avec de strings, a voir
    if (command == "JOIN")
        join(params);
    else
        std::cout << "Command not found" << std::endl;

}

void Commands::print_command(std::string prefix, std::string command, std::vector<std::string> params)
{
	std::cout << "**** Print Params ******" << std::endl;
	std::cout << " - Prefix : |" << prefix << "|" << std::endl;
	std::cout << " - Command : |" << command << "|" << std::endl;
	std::cout << " - Params : " << std::endl;
	std::vector<std::string>::iterator it =  params.begin();
	std::vector<std::string>::iterator ite =  params.end();
	int nb = 0;
	while (it != ite)
	{
		std::cout << "    " << nb << "|" << *it << "|" << std::endl;
		it++;
		nb++;
	}
}

void Commands::join(std::vector<std::string> params)
{
   (void)params;
    std::cout << "Hello from join function!" << std::endl;
}
/*
void Commands::quit(std::vector<std::string> params)
{
    (void)params;
    std::cout << "Hello from quit function!" << std::endl;
}

void Commands::nick(std::vector<std::string> params)
{
    (void)params;
    std::cout << "Hello from nick function!" << std::endl;
}*/