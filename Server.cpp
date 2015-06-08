#include <vector>
#include <sstream>
#include <iosfwd>
#include <streambuf>
#include "Server.h"
#include "easylogging++.h"

void Server::run()
{
	std::stringstream ports_string;
	bool comma = false;
	for(auto &port: this->ports)
	{
		auto connector = std::make_shared<Connector>(port);
		this->connectors.push_back(connector);
		connector->start();

		if(comma)
			ports_string << ',';
		ports_string << port;
		comma = true;
	}

	LOG(INFO) << "Connectors started " << this->connectors.size() << ": " << ports_string.str();

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

	LOG(INFO) << "Connectors stop pending ";

	for(auto &iter: this->connectors)
		iter->stop();

	LOG(INFO) << "Connectors stopped";

	this->connectors.clear();

	LOG(INFO) << "Connectors removed";
	LOG(INFO) << "Gracefull exit";
}

void Server::set_ports(ports_t ports)
{
	this->ports = ports;
}

void Server::on_terminate_signal(ev::sig& signal, int)
{
	signal.loop.break_loop(ev::ALL);
	signal.stop();
	LOG(INFO) << "Terminate signal";
	//abort();
}


