#include "ft.hpp"

void ft_error(int nb, std::vector<std::string> params, Client* client, Channel* channel, std::vector<Client *> client_list, std::vector<Channel*> channel_list)
{
    (void)nb;
    (void)client;  
    (void)params;
    (void)client_list;
    (void)channel_list;

    std::string error;
    
    switch (nb)
    {
        case 401: //ERR_NOSUCHNICK
        {
            //comment faire pour afficher le bon nickname? quell param?
            error = "<nickname> :No such nick/channel\r\n";
            break;
        }
        case 403: // ERR_NOSUCHCHANNEL
        {
            //comment faire pour afficher le bon channel name? quell param?
            error = params[1] + " :No such channel\r\n";
            break;
        }
        case 404: //ERR_CANNOTSENDTOCHAN
        {
            error = channel->getName() + " :Cannot send to channel\r\n";
            break;
        }
        case 411: // ERR_NORECIPIENT
        {
            std::string command = params[0];
            error = ":No recipient given (" + command + ")\r\n";
            break;
        }
        case 412: // ERR_NOTEXTTOSEND
        {
            error = ":No text to send\r\n";
            break;
        }
        case 442: // ERR_NOTONCHANNEL
        {
            error = channel->getName() + " :You're not on that channel\r\n";
            break;
        }
        case 461: // ERR_NEEDMOREPARAMS
        {
            std::string command = *(params.begin());
            error = command + " :Not enough parameters\r\n";
            break;
        }
        case 474: // ERR_BANNEDFROMCHANNEL
        {
            error = channel->getName() + " :Cannot join channel (+b)\r\n";
            break;
        }

        case 475: // ERR_BADCHANNELKEY
        {
            error = channel->getName() + " :Cannot join channel (+k)\r\n";
            break;
        }
        default:
		{
			error = "No Error set yet \r\n";
		}

    }
    send(client->getSocket(), error.c_str(), error.size(), 0);
    
    return;
}
