#pragma once

#include <ev++.h>
#include <unordered_set>
#include <vector>
#include <memory>

#include "Connector.h"

class Server {
public:
	typedef std::vector<std::shared_ptr<Connector>> connectors_t;
	typedef std::unordered_set<uint16_t> ports_t;

	void run();
	void set_ports(ports_t);
	void set_text(const std::string& text);
	const std::string* get_text();
protected:
	ev::default_loop loop;
	ev::sig sigint;
	ev::sig sigkill;
	ev::sig sigterm;
	connectors_t connectors;
	ports_t ports;
	std::string text;

	static void on_terminate_signal(ev::sig& w, int revents);
};
