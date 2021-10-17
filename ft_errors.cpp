#include "ft.hpp"
#include "common_macro.hpp"

void ft_error(std::string nb_str, Client* client, Channel* channel, std::string arg)
{
	int nb = atoi(nb_str.c_str());
    std::string error;
	//Revoir syntax systematique des erreurs	
	error = ft_id_type2(client, nb_str); //":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " " + nb_str + " : ";

    switch (nb)
    {
        case 401: //ERR_NOSUCHNICK
        {
            error += (arg + " :No such nick\r\n");
            break;
        }
        case 403: // ERR_NOSUCHCHANNEL
        {
            error += (arg + " :No such channel\r\n");
            break;
        }
        case 404: //ERR_CANNOTSENDTOCHAN
        {
            error += (channel->getName() + " :Cannot send to channel\r\n");
            break;
        }
		case 405: //ERR_TOOMANYCHANNELS
		{
			error += (channel->getName() + " :You have joined too many channels\r\n"); 
			break;
		}
		case 407: //ERR_TOOMANYTARGETS
		{
			//A voir
			error += "<target> :<error code> recipients. <abort message>\r\n";
			break;
		}
		/* Not used
        case 411: // ERR_NORECIPIENT
        {
            std::string command = params[0];
            error += (":No recipient given (" + command + ")\r\n");
            break;
        }
		*/
        case 412: // ERR_NOTEXTTOSEND
        {
            error += ":No text to send\r\n";
            break;
        }
		case 421: //ERR_UNKNOWNCOMMAND
		{
			error += (arg + " :Unknown command\r\n");
			break;
		}
		case 431: //ERR_NONICKNAMEGIVEN (deja gerer par hexchat?)
		{
			error += ":No nickname given\r\n";
			break;
		}
		case 432: //ERR_ERRONEOUSNICKNAME
		{
			error += (arg/*<nickname>*/ + " :Erroneous nicknamei\r\n");
			break;
		}
		case 433: //ERR_NICKNAMEINUSE
		{
			error += (arg + " :Nickname is already in use\r\n");
			break;
		}
		case 436: //ERR_NICKCOLLISION
		{
			error += (arg + " :Nickname collision KILL from" + client->getUsername() + "@" + client->getHostname() + "\r\n");
			break;
		}
		case 441: // ERR_USERNOTINCHANNEL
        {
            error += (channel->getName() + " :" + arg + " is not that channel\r\n");
            break;
        }
        case 442: // ERR_NOTONCHANNEL
        {
            error += (channel->getName() + " :You're not on that channel\r\n");
            break;
        }
        case 461: // ERR_NEEDMOREPARAMS
        {
            error += (arg + " :Not enough parameters\r\n");
            break;
        }
		case 462: //ERR_ALREADYREGISTERED
		{
			error += " :Unauthorized command (already registered)\r\n";
			break;
		}
		case 464: //ERR_PASSWDMISMATCH
		{
			error += " :Password incorrect\r\n";
			break;
		}
		case 471: //ERR_CHANNELISFULL
		{
			error += (channel->getName() + " :Cannot join channel (+1)\r\n");
			break;
		}
		case 473: //ERR_INVITEONLYCHAN
		{
			error += (channel->getName() + " :Cannot join channel (+i)\r\n");
			break;
		}
        case 474: // ERR_BANNEDFROMCHANNEL
        {
            error += (channel->getName() + " :Cannot join channel (+b)\r\n");
            break;
        }
        case 475: // ERR_BADCHANNELKEY
        {
            error += (channel->getName() + " :Cannot join channel (+k)\r\n");
            break;
        }
		case 476: // ERR_BADCHANMASK
        {
            error += (arg + " :Bad Channel Mask (+k)\r\n"); 
            break;
        }
		case 481: //ERR_NOPRIVILEGES
		{
			error += " :Permission Denied- You're not an IRC operator\r\n";
			break;
		}
		case 482: //ERR_CHANOPRIVNEEDED
		{
			error += (channel->getName() + " :Your are not channel operator\r\n");
			break;
		}
		case 484: //ERR_RESTRICTED
		{
			error += " :Your connection is restricted\r\n";
			break;
		}
		case 491: //ERR_NOOPERHOST
		{
			error += " :No O-lines for your host\r\n";
			break;
		}
		case 501: //ERR_UMODEUNKNOWNFLAG
		{
			error += " :Unknown MODE flag\r\n";
			break;
		}
		case 502: //ERR_USERDONTMATCH
		{
			error += " :Cant change mode for other users\r\n";
			break;
		}
        default:
		{
			error += "No Error set yet \r\n";
		}
    }
    send(client->getSocket(), error.c_str(), error.size(), 0);
    return;
}
