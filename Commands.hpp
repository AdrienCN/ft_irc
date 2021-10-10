// Class  pour analyser les  Commands
//[{:}{prefix}{SP}]{Commands}[{SP}{PARAMS}]{CRLF}
//[optionnel] {SP = espace normal} {CRLF = duo charriot fin de ligne}
//===> au total 512 caracteres max
//===> {Commands} = Commandse ou reponse en 3 chiffre du serveur
// std::string _prefix; // optionnel dans notre cas sans la com server server

#ifndef Commands_hpp
#define Commands_hpp

#include "headers.hpp"
#include "Grammar.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "ft.hpp"

class Client;
class Channel;

# define CMD_PARAM Client* client, std::vector<Client *> client_list, std::vector<Channel*>* channel_list

class Commands
{

    public:
        Commands();
        virtual ~Commands(); 

        //General
        void initialize_list();
        void print_command(std::string prefix, std::string command, std::vector<std::string> params); 

		// Parsing et tri de la str command
        void find_command(std::string params, CMD_PARAM); 
		void	analyzeCommand(std::string command);

        //Commandes à executer
		void	join(std::vector<std::string> params, CMD_PARAM);
		void	part(std::vector<std::string> params, CMD_PARAM);
		void	topic(std::vector<std::string> params, CMD_PARAM);
		void	nick(std::vector<std::string> params, CMD_PARAM);
		void	user(std::vector<std::string> params, CMD_PARAM);
		void	pass(std::vector<std::string> params, CMD_PARAM);
    
    protected:
        Commands	(Commands const& src);
        Commands&	operator=(Commands const& src); 
		std::map	<std::string , void(Commands::*)(std::vector<std::string> params, CMD_PARAM)> _cmd_list;
		std::vector<std::string>	_parsed_cmd;
 
};


#endif
