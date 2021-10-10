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
        case 403: // ERR_NOSUCHCHANNEL
        {
            error = channel->getName() + " :No such channel\r\n";
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
