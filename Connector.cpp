#include "Connector.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

Connector::Connector(uint16_t port):port(port)
{
	ev_loop_flags |= EVBACKEND_EPOLL;
}

void Connector::init()
{
	int socket_d = socket(AF_INET, SOCK_STREAM, 0);
//	struct sockaddr_in address;
//
//	//address.sin_addr = htonl(INADDR_LOOPBACK);
//	address.sin_family = AF_INET;
//	address.sin_port = htons(this->port);

	if(socket_d < 0)
		std::cout << "Can't create socket";

//	if(bind(socket_d, ))
//	{
//		std::cout << "Can't bind to address";
//	}

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
}

