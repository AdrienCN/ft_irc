#include "common_macro.hpp"
#include "headers.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
	std::string port;
	std::string password;
	//condition a retravailler
	if (argc == 3)
	{
		port = argv[1];
		password = argv[2];
	}
	else
	{
		port = "6667";
		password = "password";
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
