// Class parent pout analyser un Command --> les class filles seront les differentes commandes
#ifndef Command_hpp
#define Command_hpp

#include "headers.hpp"

class ACommand
{

    public:
        ACommand(std::string command, std::vector<std::string> params); // on lira l'Command de std::getline puis on le tri 
        virtual ~ACommand(); 

        std::string getName() const;

        virtual void execute() = 0;
  
    protected:
        ACommand(Command const& src);
        ACommand& operator=(Command const& src); 

        //[{:}{prefix}{SP}]{COMMAND}[{SP}{PARAMS}]{CRLF}
        //[optionnel] {SP = espace normal} {CRLF = duo charriot fin de ligne}
        //===> au total 512 caracteres max
        //===> {COMMAND} = commande ou reponse en 3 chiffre du serveur
        // std::string _prefix; // optionnel dans notre cas sans la com server server
        std::string _command;
        std::vector<std::string> _params; // jusqu'a 15 max, chaines de caracteres sans espace SAUF: le dernier param peiut contenir des espaces si le premier commence par :
};

#endif
