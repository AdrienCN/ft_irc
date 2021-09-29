#include "Server.hpp"
#include "headers.hpp"

int main()
{
	Server irc("6667", "password");
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
