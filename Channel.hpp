// Class pour representer un cannal de communication du IRC

#ifndef Channel_hpp
#define Channel_hpp

#include "headers.hpp"
#include "Client.hpp"

class Client;

//namespace ft

class Channel
{

    public:
        Channel(std::string name); 
        virtual ~Channel(); 

        void present();
        
        // GETTERS
        std::string const & getName() const;

        //en public pour pouvoir utiliser les iterateurs facilement?
        std::vector<Client *> _operators; // liste des operateurs du cannal
        std::vector<Client *> _members;

    private:
        Channel();
        Channel(Channel const& src);
        Channel & operator=(Channel const& src); 

        std::string _name;


        //Modes
        // Des booleans a définir
        // o donne/retire ops rights
        // p cannal priviliégé
        // s cannal secret
        // i cannal accessible sur invite d'un ops
        // t sujet du cannal modifiale pour ops uniquement
        // n pas de msg clients exterieurs 
        // b masque de bannissement
        // v donne/retire possibilité de parler dans un cannal modéré
        // m cannal modéré
        // l max user cannal        
};

#endif
