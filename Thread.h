#pragma once

#include <ev++.h>
#include <thread>

class Thread {
public:
	typedef std::thread thread_t;
	Thread();
	~Thread();
	virtual void start();
	void stop();
protected:
	thread_t thread;
	unsigned int ev_loop_flags;
	ev::dynamic_loop *loop;
	ev::async stopper;

	virtual void init() = 0;
	virtual void on_stop() = 0;
	virtual void finalize() = 0;
	void stop_handler();
	void run();
private:
	Thread(const Thread &) = delete;
	void operator=(const Thread &) = delete;
};
