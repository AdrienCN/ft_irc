#include "ft.hpp"

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
			//rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " " + "001" + " " + ": ""Welcome to the Internet Relay Network " + client->getNickname() + "\r\n";
			rpl = ":127.0.0.1 001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "\r\n";
			break;
		}
		case 331: //RPL_NOTOPIC
		{
			rpl = channel->getName() + " :No topic is set\r\n";
			break;
		}
		
		case 332: //RPL_TOPIC 
		{			
			rpl = channel->getName() + " :" + channel->getTopic() + "\r\n";
			break;
		}
		
		case 353: // RPL_NAMEREPLY
		{
			//<canal>:[[@|+]<pseudo>[[@|+]<pseudo>[...]]]

			rpl = channel->getName() + " :";
			std::vector<Client*> tmp = channel->getMemberList();
			std::vector<Client*>::iterator it = tmp.begin();
			std::vector<Client*>::iterator ite = tmp.end();
			while (it != ite)
			{
				rpl += (*it)->getNickname() + " ";
				it++;
			}
			rpl +=  "\r\n";
			break;	
		}

		default:
		{
			rpl = "No RPL set yet \r\n";
			break;
		}

    }
	std::cout << YELLOW << "RPL : [" << rpl << "]" << RESET << std::endl;
	send(client->getSocket(), rpl.c_str(), rpl.size(), 0);
    return;

}


