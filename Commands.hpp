// Class  pour analyser les  Commands
#ifndef Commands_hpp
#define Commands_hpp

#include "headers.hpp"
#include "Grammar.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

# define CMD_PARAM_CMD std::string params, Client* client, std::vector<Client *> client_list, std::vector<Channel*> channel_list

# define CMD_PARAM_FT std::vector<std::string> params, Client* client, std::vector<Client *> client_list, std::vector<Channel*> channel_list

class Commands
{

    public:
        Commands();
        virtual ~Commands(); 

        //General
        void initialize_list();
        void find_command(CMD_PARAM_CMD); 
        void print_command(std::string prefix, std::string command, std::vector<std::string> params); 

		//Other
		void	analyzeCommand(std::string command);

        //Commandes
		void	join(CMD_PARAM_FT);
		void	nick(CMD_PARAM_FT);
		void	user(CMD_PARAM_FT);
		void	pass(CMD_PARAM_FT);
    
    protected:
        Commands	(Commands const& src);
        Commands&	operator=(Commands const& src); 
		std::map	<std::string , void(Commands::*)(CMD_PARAM_FT)> _cmd_list;
		std::vector<std::string>	_parsed_cmd;
        //[{:}{prefix}{SP}]{Commands}[{SP}{PARAMS}]{CRLF}
        //[optionnel] {SP = espace normal} {CRLF = duo charriot fin de ligne}
        //===> au total 512 caracteres max
        //===> {Commands} = Commandse ou reponse en 3 chiffre du serveur
        // std::string _prefix; // optionnel dans notre cas sans la com server server
};


#endif
