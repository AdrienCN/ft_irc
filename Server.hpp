#ifndef Server_hpp
#define Server_hpp

#include "common_macro.hpp"
#include "headers.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Commands.hpp"
#include "ft.hpp"

//namespace ft
//
#define MAX_CLIENT 10
#define PORT_SERVER "6667"

//class Commands;

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
		std::string const & getServerName() const;
		std::string const & getServerIpaddress() const;
		std::string const & getServerCreationDate() const;

		//SETTERS
		void  setServerName(std::string const & src);
		void  setServerIpaddress(std::string const & src);
		void  setServerCreationDate(std::string const & src);


		//Adrien_degub
		void	pollInfo(std::vector<struct pollfd> const & src);
		void	refuseClient();

        // CONNECTIONS MANAGEMENT
        void    init();
        void    run();
        void    poll_add_client(Client const& new_client);
        void    poll_remove_client(int const& fd);
		void	find_to_kill();
       // void    addClient();
		struct pollfd const &    addClient();
        void    removeClient(int const & fd);
        Client* find_client_from_fd(int fd);

        //MESSAGE MANAGEMENT
        void    receiveMessage(Client* client);
		void    sendGreetings(Client *client);
		void	welcomeClient(Client* client);

        
    private:
        Server();
        Server(Server const& src);
        Server & operator=(Server const& src); 
        

        struct pollfd               _poll;
        std::vector<struct pollfd>  _fds;
        std::string                 _domain; // char* add IP ou nom de domain ou NULL si propre IP (!! flag AI8PASSIVE hint en plus)
        std::string                 _port; // char* port or http? Dans notre 6667
        struct addrinfo*            _serv_info;        
        struct addrinfo             _hints; // to initialize the server
        std::string                 _password; // const?
		std::string					_server_name;
		std::string					_server_ipaddress;
		std::string					_server_creation_date;
        int                         _server_socket;
        int                         _nbClients;
        Commands                    _command_book;
        std::vector<Client*>        _all_clients;
        std::vector<Channel*>       _all_channels;
};

#endif
