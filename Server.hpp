#ifndef Server_hpp
#define Server_hpp

#include "headers.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Grammar.hpp"
#include "Commands.hpp"
#include "ft_print_help.hpp"

//namespace ft
//
#define MAX_CLIENT 2
#define PORT_SERVER "6667"

class Commands;

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

        // GETTERS
        int const &				getSocket() const;
        std::string const &		getPort() const;
        std::string const &		getPassword() const;
        struct addrinfo*		getServInfo() const;

        //MAIN FUNCTIONS
        void init();
        void run();
        void poll_add_client(Client const& new_client);
       // void poll_remove_client(Client const& old_client);
        void poll_remove_client(int const& fd);
        void addClient();
        void removeClient(int const & fd);
        void receiveMessage(Client* client);
        void analyzeMessage(std::string message, Client* client);
        void manage_substr(std::string message, Client* client);
		void sendGreetings(Client *client);

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
        
       // Commands _command_list;

        std::vector<Client*> _all_clients;
        std::vector<Channel*> _all_channels;

};

#endif
