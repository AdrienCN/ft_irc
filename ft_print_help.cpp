#include "headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void print_command(std::string prefix, std::string command, std::vector<std::string> params)
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

void print_client_lit(std::vector<Client*> client_list)
{
	std::cout << "**** Print Client List ******" << std::endl;
	std::vector<Client*>::iterator it =  client_list.begin();
	std::vector<Client*>::iterator ite =  client_list.end();
	int nb = 0;
	while (it != ite)
	{
		(*it)->present();
		it++;
		nb++;
	}
}
/*
void print_channel_lit(std::vector<Channel*> channel_list)
{
	std::cout << "**** Print Channel List ******" << std::endl;
	std::vector<Client*>::iterator it =  channel_list.begin();
	std::vector<Client*>::iterator ite =  channel_list.end();
	int nb = 0;
	while (it != ite)
	{
		std::cout << (*it)->present() << std::endl;
		it++;
		nb++;
	}
} */