#include "Server.h"


Server::Server()
{
}

void Server::run()
{
	for(auto &port: this->ports)
	{
		auto connector = std::make_shared<Connector>(port);
		this->connectors.push_back(connector);
		connector->start();
	}

	std::cout << "Connectors started" << std::endl;

	this->signal.set<Server, &Server::on_terminate_signal>(this);
	this->signal.start(SIGINT);

	this->loop.run(0);

	this->signal.stop();

	for(auto &iter: this->connectors)
		iter->stop();

	std::cout << "Connectors stopped" << std::endl;

	this->connectors.clear();

	std::cout << "Connectors removed" << std::endl;

	std::cout << "Gracefull exit" << std::endl;
}

void Server::set_ports(ports_t ports)
{
	this->ports = ports;
}

void Server::on_terminate_signal()
{
	std::cout << "Terminate";
}


