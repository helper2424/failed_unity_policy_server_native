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

	this->sigint.set<&Server::on_terminate_signal>();
	this->sigint.start(SIGINT);

	this->sigterm.set<&Server::on_terminate_signal>();
	this->sigterm.start(SIGTERM);

	this->sigkill.set<Server::on_terminate_signal>();
	this->sigkill.start(SIGKILL);

	this->loop.run(0);

	this->sigint.stop();
	this->sigterm.stop();
	this->sigkill.stop();

	std::cout << "Start stop connectors" << std::endl;

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

void Server::on_terminate_signal(ev::sig& signal, int)
{
	signal.loop.break_loop(ev::ALL);
	signal.stop();
	std::cout << "Signal handled";
	//abort();
}


