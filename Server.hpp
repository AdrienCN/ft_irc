#ifndef Server_hpp
#define Server_hpp

#include "headers.hpp"

//namespace ft

class Server
{

    //class Exeception pour failed getaddrinfo
    //class Exeception pour failed ERrno pour socket bind listen
    
    public:
        Server(std::string port, std::string password); 
        virtual ~Server(); 

        int getSocket() const;
        struct addrinfo* getServInfo() const;
        std::string getPortconst() const;
        std::string getPassword() const;

        void init();

    private:
        Server();
        Server(Server const& src);
        Server & operator=(Server const& src); 

        std::string _domain; // char* add IP ou nom de domain ou NULL si propre IP (!! flag AI8PASSIVE hint en plus)
        std::string _port; // char* port or http? Dans notre 6667
        struct addrinfo* _serv_info;        
        struct addrinfo _hints; // to initialize the server
        std::string _password; // const?
        int _socket;




};

#endif