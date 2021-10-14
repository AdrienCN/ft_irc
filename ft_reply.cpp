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
		rpl = ":127.0.0.1 00" + nb_str + " " + client->getNickname() + " :";
	else
	{
		rpl = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " " + nb_str + " : ";
	}
	switch(nb)
    {
		case 1:
		{			
			rpl += "Welcome to the Internet Relay Network " + client->getNickname() + "\r\n";
			break;
		}
		case 2:
		{
			rpl += "Your host is " + client->getServerName() + ", running version 42.42\r\n";
			break;
		}
		case 3:
		{
			rpl += "This server was created " + client->getServerCreationDate() + "\r\n";
			break;
		}
		case 4:
		{
			rpl += client->getServerName() + " 42.42 <available user modes> <available channel modes>\r\n";
			break;
		}
		case 5:
		{
			//Remplacer cette reponse par "Sorry IRC90's user capacity is full. Please retry later\r\n" ?
			rpl += "Try server <server name>, port <port number>\r\n";
			break;
		}
		//RPL_AWAY
		case 301:
		{
			rpl += client->getNickname() + " :" + client->getAwayMessage() + "\r\n";
			break;
		}
		//RPL_UNAWAY
		case 305:
		{
			rpl += "You are no longer marked as being away\r\n";
			break;
		}
		//RPL_NOWAWAY
		case 306:
		{
			rpl += "You have been marked as being away\r\n" ;
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
				if (channel->isUserOp(*it) == 1)
					rpl += "@";
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
		case 4242:
		{
			rpl ="[CAP] : IRC_90'S does not handle capabilities\r\n";
			std::cout << GREEN << rpl << RESET <<std::endl;
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


