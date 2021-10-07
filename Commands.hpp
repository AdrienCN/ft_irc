// Class  pour analyser les  Commands
#ifndef Commands_hpp
#define Commands_hpp

#include "headers.hpp"
#include "Grammar.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

# define CMD_PARAM std::vector<std::string> params, Client* client, std::vector<Client *> client_list, std::vector<Channel*> channel_list


class Commands
{

    public:
        Commands();
        virtual ~Commands(); 

        //General
        void initialize_list();
        void find_command(CMD_PARAM); 
        void print_command(std::string prefix, std::string command, std::vector<std::string> params); 

        //Commandes
		void	join(CMD_PARAM);
		void	nick(CMD_PARAM);
		void	user(CMD_PARAM);
		void	pass(CMD_PARAM);
    
    protected:
        Commands	(Commands const& src);
        Commands&	operator=(Commands const& src); 
		std::map	<std::string , void(Commands::*)(CMD_PARAM)> _cmd_list;
        //[{:}{prefix}{SP}]{Commands}[{SP}{PARAMS}]{CRLF}
        //[optionnel] {SP = espace normal} {CRLF = duo charriot fin de ligne}
        //===> au total 512 caracteres max
        //===> {Commands} = Commandse ou reponse en 3 chiffre du serveur
        // std::string _prefix; // optionnel dans notre cas sans la com server server
};


#endif
