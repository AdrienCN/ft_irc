#include "Server.hpp"
#include "headers.hpp"

int main()
{
	try
	{
		Server irc("6667", "password");
		irc.init();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	while (1)
	{
		int i = 1;
		i++;
	}
	return (0);
}
