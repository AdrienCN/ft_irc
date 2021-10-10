#ifndef Client_hpp
#define Client_hpp

#include "headers.hpp"
#include "Channel.hpp"
#include "Commands.hpp"

class Channel;

# define MAX_CHAR 512
# define COMPLETE 1
# define INCOMPLETE 0
# define DISCONNECT -1
# define END_CHAR "\r\n"

//namespace ft

class Client
{

    public:
        Client(); 
        virtual ~Client(); 

        // Presentation
        void present();
        
        // GETTERS
        std::string const &		getPassword() const;
        std::string const &		getNickname() const;
        std::string const &		getUsername() const;
        std::string const &		getMessage() const;
        int         const &		getMessageStatus() const;
        int         const &     getSocket() const;
		int			const &		getGreetings();
        struct pollfd   const & getPoll() const;
		bool		const &		isRegistered() const;
		bool		const &		getRegPass() const;
		bool		const &		getRegNick() const;
		bool		const &		getRegUser() const;
		std::vector<std::string> const & getCommand() const;

        //SETTERS
 		void		setRegPass(bool const &src);
		void		setRegNick(bool const &src);
		void		setRegUser(bool const &src);       
		void setPassword(std::string const& src);
        void setNickname(std::string const& src);
        void setUsername(std::string const& src);
        void setMessage(std::string const& src);
        void setMessageStatus(bool const& src);
		void setRegistration(bool const& src);
        void init(int const & socket);

        //Others
		void incrGreetings(); // to remove
        void clearMessage();
        void clearStr(std::string str);
		void clearCommand();
		void recvMessage();
		void analyzeMessage();
        void add_channel(Channel* channel);
        void remove_channel(Channel* channel);
        void leaveAllChannels();


    private:
        Client(Client const& src);
        Client & operator=(Client const& src); 

        struct pollfd _poll;
        std::string _password;
        std::string _nickname;
        std::string _username;
        std::string _message;
        int	        _message_status; // true finished , false not finished
		int			_greetings;
		std::string _hostname;

        
        //std::string _host;
        //std::string _servername;
        //std::string _realname;
        // pas de roles car peut avoir des roles != dans != channels

        std::vector<Channel *> _channels; // liste des channels dans lesquels il est     
        int _socket;
		std::vector<std::string> _command;
		bool _registration_status;
		std::vector<std::string> _cap;
		std::vector<std::string> _nick;
		std::vector<std::string> _user;
		bool _reg_pass;
		bool _reg_nick;
		bool _reg_user;
        //Input _input: // template pour analyser un message


};

#endif
