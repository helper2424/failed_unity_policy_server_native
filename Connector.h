#pragma once

#include <stdint.h>
#include "Thread.h"
#include <ev++.h>

class Connector : public Thread
{
public:
	Connector(uint16_t port);
protected:
	uint16_t port;
	ev::io connect_watcher;

	virtual void init();
	virtual void finalize();

	void connnect(ev::io &w, int revents);
};
