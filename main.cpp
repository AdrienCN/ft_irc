#include "common_macro.hpp"
#include "headers.hpp"
#include "Server.hpp"

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
