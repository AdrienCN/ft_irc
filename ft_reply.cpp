#include "ft.hpp"
#include "common_macro.hpp"


void ft_reply(std::string nb_str, std::vector<std::string> params, Client* client, Channel* channel, std::vector<Client *> client_list, std::vector<Channel*> channel_list)
{
    (void)params;
    (void)client;
    (void)client_list;
    (void)channel_list;

	std::string rpl;
	int nb = atoi(nb_str.c_str());
	rpl.clear();
	if (nb <= 5)
		rpl = "quelque chose";
	else
	{
		rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " " + nb_str + " : ";
	}
	switch(nb)
    {
		case 1:
		{
			rpl.clear();
			rpl = ":127.0.0.1 " + nb_str + " " +  client->getNickname() + " :";
			//rpl = "Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + "0" + "\r\n";
			rpl += "Welcome to the Internet Relay Network " + client->getNickname() + "\r\n";
			break;
		}
		case 331: //RPL_NOTOPIC
		{
			rpl += (channel->getName() + " :No topic is set\r\n");
			break;
		}
		
		case 332: //RPL_TOPIC 
		{			
			rpl +=(channel->getName() + " :" + channel->getTopic() + "\r\n");

			break;
		}
		
		case 353: // RPL_NAMEREPLY
		{
			//<canal>:[[@|+]<pseudo>[[@|+]<pseudo>[...]]]
			rpl = ":127.0.0.1 " + nb_str + " " +  client->getNickname();
			rpl += (" = " + channel->getName() + " :");
			std::vector<Client*> tmp = channel->getMemberList();
			std::vector<Client*>::iterator it = tmp.begin();
			std::vector<Client*>::iterator ite = tmp.end();
			while (it != ite)
			{
				rpl += ((*it)->getNickname() + " ");
				it++;
			}
			rpl +=  "\r\n";
			std::cout << "rpl: |" << rpl << "|" << std::endl;
			break;	
		}
		case 366: // ENDOFNAMES
		{
			rpl = ":127.0.0.1 " + nb_str + " " +  client->getNickname() + " ";
			rpl += (channel->getName() + " :End of NAMES list\r\n");
			break;	
		}

		default:
		{
			rpl = "No RPL set yet \r\n";
			break;
		}

    }
	send(client->getSocket(), rpl.c_str(), rpl.size(), 0);
    return;

}


