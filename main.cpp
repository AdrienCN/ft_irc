#include "Server.hpp"
#include "headers.hpp"

int main()
{
	Server irc("6667", "password");
	try
	{
		irc.init();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
		return (1);
	}
	while (1)
	{
		struct sockaddr new_client;
		socklen_t		client_size = sizeof(new_client);
		int				fd_client;

		fd_client = accept(irc.getSocket(), &new_client, &client_size);

	}
	return (0);
}
