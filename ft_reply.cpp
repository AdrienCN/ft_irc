#include "ft.hpp"
#include "common_macro.hpp"

std::string ft_id_type1(Client* client, std::string nb_str)
{
	std::string id = ":" + client->getServerName() + " " + nb_str + " " + client->getNickname() + " :";
	return (id);
}

std::string ft_id_type2(Client* client, std::string nb_str)
{
	std::string id = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " " + nb_str + " : ";
	return (id);
}

void ft_reply(std::string nb_str, Client* client, Channel* channel, std::string arg)
{
	std::string rpl;
	int nb = atoi(nb_str.c_str());
	rpl.clear();
	if (nb <= 5 || nb == 353)
		rpl = ft_id_type1(client, nb_str);
	else
		rpl = ft_id_type2(client, nb_str);

	switch(nb)
	{
		case 1:
		{			
			rpl += "Welcome to the Internet Relay Network " + client->getNickname() + "\r\n";
			break;
		}
		case 2:
		{
			rpl += "Your host is [" + client->getServerIpAddress() + "], running on version [42.42]\r\n";
			break;
		}
		case 3:
		{
			rpl += "This server was created " + client->getServerCreationDate() + "\r\n";
			break;
		}
		case 4:
		{
			rpl += client->getServerName() + " version [42.42]. Available user MODE : +Oa . Avalaible channel MODE : none. \r\n";
			break;
		}
		case 5:
		{
			rpl +="Sorry IRC_90's capacity is full. Please retry connection later\r\n";
			break;
		}	
		case 221: //RPL_UMODEIS
		{
			if (client->getAway() == false && client->getOper() == false)
				rpl += client->getNickname() + " :no mode set"; 
			else
				rpl += client->getNickname() + " :active mode +";
			if (client->getAway() == true)
				rpl += "a";
			if (client->getOper() == true)
				rpl += "O";
			rpl +="\r\n";
			break;
		}
		case 301: //RPL_AWAY
		{
			rpl += (arg + "\r\n");
			break;
		}	
		case 305: //RPL_UNAWAY
		{
			rpl += "You are no longer marked as being away\r\n";
			break;
		}
		case 306: //RPL_NOWAWAY
		{
			rpl += "You have been marked as being away\r\n" ;
			break;
		}
		case 315: //RPL_ENDWHO
		{
			if (arg.empty())
				rpl += client->getServerName();
			else
				rpl += arg;
			rpl += " :End of WHO list\r\n" ;
			break ;
		}
		case 322: //RPL_LIST
		{
			rpl += (client->getServerName() + " :" + channel->getTopic() + "\r\n");
			break;
		}
		case 323: //RPL_LISTEND
		{
			rpl += " :End of LIST\r\n";
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
		case 352: //RPL_WHO
		{
			rpl = ":127.0.0.1 " + nb_str + " " + client->getNickname() + " "; // != id1 car pas double 00 ==> a checker
			rpl += arg;
			break;
		}
		case 353: // RPL_NAMEREPLY
		{
			rpl = ":127.0.0.1 " + nb_str + " " +  client->getNickname(); // != id1 car pas double 00 ==> a checker
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
			//std::cout << "rpl: |" << rpl << "|" << std::endl;
			break;	
		}
		case 366: // ENDOFNAMES
		{
			rpl = ":127.0.0.1 " + nb_str + " " +  client->getNickname() + " ";  // != id1 car pas double 00 ==> a checker
			if (channel)
				rpl += channel->getName();
			rpl += " :End of NAMES list\r\n";
			break;	
		}
		case 375:
		{
			rpl += ":- " + client->getServerName() + " Message of the day - \r\n";
			break;
		}
		case 376:
		{
			rpl += ":End of MOTD command\r\n";
			break;
		}
		case 381: // YOUREOPER
		{
			rpl += "You are now an IRC operator\r\n";
			break;	
		}
		case 4242:
		{
			rpl += ":" + client->getServerName() + " " + nb_str + " " + client->getNickname() + " :";
			rpl = "[CAP] : IRC_90'S does not handle capabilities\r\n";
			break;
		}
		case 4243:
		{
			rpl += client->getNickname() + ": sets mode " + arg + " on " + client->getNickname() + "\r\n";
			break;
		}
		case 4244: // customhelp
		{
			// Liste des commandes dispo sur notre IRC
			rpl += "Available commands:\n";
			rpl += "---> Globales: NICK | OPER | MODE | WHO | AWAY | HELP | KILL | QUIT \n";
			rpl += "---> On Channels: JOIN | PART | TOPIC | NAMES | LIST | KICK\n";
			rpl += "---> To send messages: PRIVMSG | NOTICE";
			rpl += "\r\n";
			break;
		}
		case 4245:
		{
			rpl += arg + "\r\n";
			break;
		}
		default:
		{
			rpl = "No RPL set \r\n";
			break;
		}
	}
	send(client->getSocket(), rpl.c_str(), rpl.size(), 0);
	return;

}


