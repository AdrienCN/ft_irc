  #include "Server.hpp"
  
Server::Server(std::string port, std::string password) : _domain(NULL), _port(port), _serv_info(NULL), _password(password)
{
    // Initialize la structure hints

    try
    {
        //getaddrinfo

        
        //init
        this->init();
    }

    catch
    {
        //write exception
    }

}

Server::~Server()
{
    return;
} 

int Server::getSocket() const
{
    // afaire
}

struct addrinfo* Server::getServInfo() const
std::string Server::getPortconst() const
std::string Server::getPassword() const

void Server::init()
{

}