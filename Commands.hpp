// Class  pour analyser les  Commands
#ifndef Commands_hpp
#define Commands_hpp

#include "headers.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Server;

class Commands
{

    public:
        Commands();
        virtual ~Commands(); 

        //General
        void initialize_list();
        void find_command(std::vector<std::string> input, Client* client, Server & server); 
        void print_command(std::string prefix, std::string command, std::vector<std::string> params); 

        //Commandes
        void join(std::vector<std::string> params);

    
    protected:
        Commands(Commands const& src);
        Commands& operator=(Commands const& src); 

        //[{:}{prefix}{SP}]{Commands}[{SP}{PARAMS}]{CRLF}
        //[optionnel] {SP = espace normal} {CRLF = duo charriot fin de ligne}
        //===> au total 512 caracteres max
        //===> {Commands} = Commandse ou reponse en 3 chiffre du serveur
        // std::string _prefix; // optionnel dans notre cas sans la com server server
};

#endif
