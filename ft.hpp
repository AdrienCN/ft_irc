#ifndef ft_hpp
#define ft_hpp

#include "headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "common_macro.hpp"

//print
class Client;
class Channel;

void print_command(std::string prefix, std::string command, std::vector<std::string> params);
void print_client_list(std::vector<Client*> client_list);
void print_channel_list(std::vector<Channel*> channel_list);
void print_vector(std::vector<std::string> inputs);

//error
//void ft_error(std::string nb, std::vector<std::string> params, Client* client, Channel* channel, std::vector<Client *> client_list, std::vector<Channel*> channel_list);
void ft_error(std::string nb, Client* client, Channel* channel, std::string arg);


//reply
//void ft_reply(std::string nb, std::vector<std::string> params, Client* client, Channel* channel, std::vector<Client *> client_list, std::vector<Channel*> channel_list);
void ft_reply(std::string nb, Client* client, Channel* channel, std::string arg);

std::string ft_id_type1(Client* client, std::string nb_str);
std::string ft_id_type2(Client* client, std::string nb_str);



#endif