#include <vector>
#include <memory>

#include "Server.h"

int main(int, char**)
{
	Server::ports_t ports = {{ 843, 81, 90 }};

	Server server;
	server.set_ports(std::move(ports));
	server.run();

	return 0;
}

