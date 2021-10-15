// Class  pour analyser les  Commands
//[{:}{prefix}{SP}]{Commands}[{SP}{PARAMS}]{CRLF}
//[optionnel] {SP = espace normal} {CRLF = duo charriot fin de ligne}
//===> au total 512 caracteres max
//===> {Commands} = Commandse ou reponse en 3 chiffre du serveur
// std::string _prefix; // optionnel dans notre cas sans la com server server

#ifndef Commands_hpp
#define Commands_hpp

#include "headers.hpp"
#include "common_macro.hpp"
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
		Commands(std::string const & password, std::string server_name, std::string server_ipaddress, std::string server_creation_date); 
        virtual ~Commands(); 

        //General
        void initialize_list();
        void print_command(std::string prefix, std::string command, std::vector<std::string> params); 

		// Parsing et tri de la str command
        void find_command(std::string params, CMD_PARAM); 
		void	analyzeCommand(std::string command);

        //Commandes à executer
		void	join(std::vector<std::string> params, CMD_PARAM);
		void	oper(std::vector<std::string> params, CMD_PARAM);
		void	cap(std::vector<std::string> params, CMD_PARAM);
		void	part(std::vector<std::string> params, CMD_PARAM);
		void	kick(std::vector<std::string> params, CMD_PARAM);
		void	topic(std::vector<std::string> params, CMD_PARAM);
		void	nick(std::vector<std::string> params, CMD_PARAM);
		void	user(std::vector<std::string> params, CMD_PARAM);
		void	pass(std::vector<std::string> params, CMD_PARAM);
		void	privmsg(std::vector<std::string> params, CMD_PARAM);
		void	notice(std::vector<std::string> params, CMD_PARAM);
		void	who(std::vector<std::string> params, CMD_PARAM);
		void	names(std::vector<std::string> params, CMD_PARAM);
		void	away(std::vector<std::string> params, CMD_PARAM);
		void	mode(std::vector<std::string> params, CMD_PARAM);
		std::string const & getServerName() const;
		std::string const & getServerIpaddress() const;
		std::string const & getServerCreationDate() const;

		void  setServerName(std::string const & src);
		void  setServerIpaddress(std::string const & src);
		void  setServerCreationDate(std::string const & src);


    
    protected:
        Commands();
        Commands	(Commands const& src);
        Commands&	operator=(Commands const& src); 
		std::map	<std::string , void(Commands::*)(std::vector<std::string> params, CMD_PARAM)> _cmd_list;
		std::vector<std::string>	_parsed_cmd;

		std::string					_server_password;
		std::string _server_name;
		std::string _server_ipaddress;
		std::string _server_creation_date;
        
 
};


#endif
