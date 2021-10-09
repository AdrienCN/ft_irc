#include "headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void ft_reply(int nb, std::vector<std::string> params, Client* client, Channel* channel, std::vector<Client *> client_list, std::vector<Channel*> channel_list)
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
		case 331: //RPL_NOTOPIC
		{
			rpl = channel->getName() + " :No topic is set\r\n";
		}
		
		/*case 332: //RPL_TOPIC --> pas de topic dans Channel pour le moment
		{			
			rpl = channel->getName() +":" + channel.getTopic() + "\r\n";
		}*/
		
		default:
		{
			rpl = "No RPL set yet \r\n";
		}

    }
	send(client->getSocket(), rpl.c_str(), rpl.size(), 0);
    return;

}


