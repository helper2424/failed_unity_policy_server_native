#pragma once

#include <ev++.h>
#include <thread>

class Thread {
public:
	typedef std::thread thread_t;
	Thread();
	~Thread();
	virtual void start();
	virtual void stop();
	void run();
protected:
	thread_t thread;
	unsigned int ev_loop_flags;
	ev::dynamic_loop *loop;

	virtual void init() = 0;
	virtual void finalize() = 0;
private:
	Thread(const Thread &) = delete;
	void operator=(const Thread &) = delete;
};
