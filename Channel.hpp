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
        
        // GETTERS
        std::string const &             getName() const;
        std::vector<char> const &       getMode() const;
        std::string const &             getKey() const;
        bool const &                    getStatusKey() const;
        unsigned int const &            getNbMembers() const;
        std::string const &             getTopic() const;
        bool const &                    getStatusTopic() const;
        std::vector<Client*> const &    getMemberList() ;
        Client*                         getOperator() ;

        //SETTERS
        void    setName(std::string const & src);
        void    setKey(std::string const & src);
        void    setTopic(std::string const & src);
        void    unsetTopic();

        //Others
        int     isUserBanned(Client* client);
        int     isUserOp(Client* client);
        int     isUserMember(Client* client);
        int     isKeyRight(std::string key);
        void    addMember(Client* client);
        void    removeMember(Client* client);
        void    removeOp(Client* client);
		void    printMembersNick(Client *client);
        void    present(); //Util pour debug

    private:
        Channel();
        Channel(Channel const& src);
        Channel & operator=(Channel const& src); 

        std::string             _name;
        std::string             _key;
        bool                    _has_key;
        unsigned int            _nb_members;
        std::string             _topic;
        bool                    _has_topic;
        std::vector<Client*>    _members;
        Client*                 _channel_operator;
};

#endif
