#ifndef print_help_hpp
#define print_help_hpp
#include "headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void print_command(std::string prefix, std::string command, std::vector<std::string> params);

void print_client_list(std::vector<Client*> client_list);

/*
void print_channel_lit(std::vector<Channel*> channel_list)
} */

void print_vector(std::vector<std::string> inputs);
#endif