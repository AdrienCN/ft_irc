#ifndef Client_hpp
#define Client_hpp

#include "headers.hpp"
#include "Channel.hpp"

class Channel;

//namespace ft

class Client
{

    public:
        Client(); 
        virtual ~Client(); 

        // GETTERS
        std::string const &		getPassword() const;
        std::string const &		getNickname() const;
        std::string const &		getUsername() const;

        //SETTERS
        void setPassword(std::string const& src) const;
        void setNickname(std::string const& src) const;
        void setUsername(std::string const& src) const;

        //Others
        void joinChannel(Channel & src); // JOIN
        void quitChannel(Channel & src); // QUIT


    private:
        Client(Client const& src);
        Client & operator=(Client const& src); 

        std::string _password;
        std::string _nickname;
        std::string _username;
        //std::string _host;
        //std::string _servername;
        //std::string _realname;
        // pas de roles car peut avoir des roles != dans != channels

        std::vector<Channel *> _channels; // liste des channels dans lesquels il est     
        int _socket;
        //Input _input: // template pour analyser un message


};

#endif
