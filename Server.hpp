#ifndef Server_hpp
#define Server_hpp

#include "headers.hpp"
#include "Channel.hpp"
#include "Client.hpp"

//namespace ft
//
#define MAX_CLIENT 10

class Server
{

	class ExceptGetaddr : public std::exception
	{
		virtual const char* what() const throw()
		{
			return ("Error Server: GetAddressInfo() failed");
		}
	};

    class ExceptInit : public std::exception
	{
	   virtual const char* what() const throw()
	   {
		   return (strerror(errno));
		   //std::cout << "Error : Socket or Bind() or Listen() failed" << std::endl;
	   }
	};
    
    public:
        Server(std::string port, std::string password); 
        virtual ~Server(); 

        int const &				getSocket() const;
        std::string const &		getPort() const;
        std::string const &		getPassword() const;
        struct addrinfo*		getServInfo() const;

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

        // Ajout
        std::vector<Client*> _all_users;
        std::vector<Channel*> _all_channels;




};

#endif
