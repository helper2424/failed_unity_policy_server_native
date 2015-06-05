#include "Connector.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>

#include <iostream>
#include <netinet/tcp.h>

Connector::Connector(uint16_t port):port(port)
{
	ev_loop_flags |= EVBACKEND_EPOLL;
}

void Connector::init()
{
	this->socket_d = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address;
	int buf;

	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	address.sin_family = AF_INET;
	address.sin_port = htons(this->port);

	if(this->socket_d < 0)
		std::cout << "Can't create socket";

	if(setsockopt(this->socket_d, IPPROTO_TCP, TCP_NODELAY, (const char*)&buf, sizeof(this->socket_d)) == -1)
		std::cout << "Can't set socket option TCP_NODELAY";

	if(setsockopt(this->socket_d, SOL_SOCKET, SO_REUSEADDR, (const char*)&buf, sizeof(this->socket_d)) == -1)
		std::cout << "Can't set socket option SO_RESUDEADDR";

	if(bind(socket_d, (struct sockaddr *)&address, sizeof(address)))
	{
		std::cout << "Can't bind to address";
	}

	listen(socket_d, 5);

	this->connect_watcher.set(*this->loop);
	this->connect_watcher.set<Connector, &Connector::connnect>(this);
	this->connect_watcher.set(socket_d, EV_READ);
	this->connect_watcher.start();
}

void Connector::finalize()
{
	this->connect_watcher.stop();
}

void Connector::connnect(ev::io& , int )
{
	std::cout << this->port << " new connection" << std::endl;
}