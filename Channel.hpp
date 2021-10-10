// Class pour representer un cannal de communication du IRC

#ifndef Channel_hpp
#define Channel_hpp

#include "headers.hpp"
#include "Client.hpp"

class Client;

class Channel
{

    public:
        Channel(std::string name, Client* client); 
        virtual ~Channel(); 

        void present();
        
        // GETTERS
        std::string const & getName() const;
        std::vector<char> const & getMode() const;
        std::string const & getKey() const;
        bool const & getStatusKey() const;
        unsigned int const & getNbMembers() const;
        std::string const & getTopic() const;
        bool const & getStatusTopic() const;
        std::vector<Client*> const & getMemberList() const;
        std::vector<Client*> const & getOperatorList() const;
        std::vector<Client*> const & getBannedList() const;

        //SETTERS
        void setName(std::string const & src);
        void setKey(std::string const & src);
        void setTopic(std::string const & src);

        //Others
        int isUserBanned(Client* client);
        int isUserOp(Client* client);
        int isUserMember(Client* client);
        int isKeyRight(std::string key);
        void addMember(Client* client);
        void removeMember(Client* client);
        void addOp(Client* client);
        void removeOp(Client* client);
        void addBanned(Client* client);
        void removeBanned(Client* client);

    private:
        Channel();
        Channel(Channel const& src);
        Channel & operator=(Channel const& src); 

        std::string _name;
        std::vector<char> _mode; // d = default
        std::string _key;
        bool _has_key;
        unsigned int _nb_members;
        std::string _topic;
        bool _has_topic;
        std::vector<Client*> _members;
        std::vector<Client*> _operators; // liste des operateurs du cannal
        std::vector<Client*> _banned;

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
