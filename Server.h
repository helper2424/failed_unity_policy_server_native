#pragma once

#include <ev++.h>
#include <vector>
#include <memory>

#include "Connector.h"

//delete it
#include <iostream>

class Server {
public:
	typedef std::vector<std::shared_ptr<Connector>> connectors_t;
	typedef std::vector<uint16_t> ports_t;

	Server();
	void run();
	void set_ports(ports_t);
protected:
	ev::default_loop loop;
	ev::sig sigint;
	ev::sig sigkill;
	ev::sig sigterm;
	connectors_t connectors;
	ports_t ports;

	static void on_terminate_signal(ev::sig& w, int revents);
};
