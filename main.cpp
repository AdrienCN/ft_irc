#include "headers.hpp"
#include "common_macro.hpp"
#include "Server.hpp"

class ExceptSignal : public std::exception
{
   virtual const char* what() const throw()
   {
		return ("\nServer interruption --> leaving");
   }
};

void signal_handler(int signum) 
{
	(void)signum;
	throw ExceptSignal();
}

int main(int argc, char **argv)
{
	std::string port;
	std::string password;

	if (argc != 3)
	{
		std::cout << "Error : Arg : ./Irc_90's <port> <password>" << std::endl;
		return (1);
	}
	else
	{
		port = argv[1];
		password = argv[2];
	}
	Server irc(port, password);
	try
	{
		irc.init();
		irc.run();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
