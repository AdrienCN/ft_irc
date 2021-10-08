#include "headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

# define CMD_PARAM std::vector<std::string> params, Client* client, std::vector<Client *> client_list, std::vector<Channel*> channel_list

void ft_reply(int nb, CMD_PARAM)
{
    (void)nb;
    (void)params;
    (void)client;
    (void)client_list;
    (void)channel_list;
	std::string rpl;
    switch(nb)
    {
		case 1:
		{
			rpl = "Welcome to the Internet Relay Network\n" + client->getNickname() + "!" + client->getUsername() + "@" + "0" + "\r\n";
		}
		case 2:
		{

		}
		case 3:
		{
		}
		case 4:
		{
		}

    }
	send(client->getSocket(), rpl.c_str(), rpl.size(), 0);
    return;

}
