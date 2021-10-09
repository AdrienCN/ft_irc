#ifndef ft_hpp
#define ft_hpp

#include "headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"

//print
class Client;
class Channel;

void print_command(std::string prefix, std::string command, std::vector<std::string> params);
void print_client_list(std::vector<Client*> client_list);
void print_channel_list(std::vector<Channel*> channel_list);
void print_vector(std::vector<std::string> inputs);

/*
//error
void ft_reply(int nb, std::vector<std::string> params, Client* client, std::vector<Client *> client_list, std::vector<Channel*> channel_list)


//reply
void ft_reply(int nb, std::vector<std::string> params, Client* client, Channel* channel, std::vector<Client *> client_list, std::vector<Channel*> channel_list)
*/

#endif