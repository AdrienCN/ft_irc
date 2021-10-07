#include "headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

# define CMD_PARAM std::vector<std::string> params, Client* client, std::vector<Client *> client_list, std::vector<Channel*> channel_list

void ft_error(int nb, CMD_PARAM)
{
    (void)nb;
(void)client;
(void)params;
    (void)client_list;
    (void)channel_list;
    /*switch nb
    {
        case :

    }
    */
    return;
}
