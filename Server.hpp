#ifndef Server_hpp
#define Server_hpp

#include "headers.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Grammar.hpp"

//namespace ft
//
#define MAX_CLIENT 1

class Server
{

	class ExceptGetaddr : public std::exception
	{
		virtual const char* what() const throw()
		{
			return ("Error Server: GetAddressInfo() failed");
		}
	};

    class ExceptErrno : public std::exception
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
        void run();
        void poll_add_client(Client const& new_client);
       // void poll_remove_client(Client const& old_client);
        void poll_remove_client(int const& fd);
        void addClient();
        void removeClient();
        void receiveMessage(Client* client);
        Client* find_client_from_fd(int fd);
        
        
        std::vector<struct pollfd > _fds;

    private:
        Server();
        Server(Server const& src);
        Server & operator=(Server const& src); 
        
        struct pollfd _poll;
        std::string _domain; // char* add IP ou nom de domain ou NULL si propre IP (!! flag AI8PASSIVE hint en plus)
        std::string _port; // char* port or http? Dans notre 6667
        struct addrinfo* _serv_info;        
        struct addrinfo _hints; // to initialize the server
        std::string _password; // const?
        int _server_socket;
        int _nbClients;

        std::vector<Client*> _all_clients;
        std::vector<Channel*> _all_channels;





};

#endif
