#include "Thread.h"

#include <iostream>

Thread::Thread():ev_loop_flags(0)
{
	std::cout << "Thread created" << std::endl;
}

Thread::~Thread()
{
	std::cout << "Thread destructed" << std::endl;
}

void Thread::run()
{
	this->loop = new ev::dynamic_loop(this->ev_loop_flags);
	this->init();

	std::cout << "Thread " << this->thread.get_id() << " started" << std::endl;

	this->loop->run(0);

	std::cout << "Thread " << this->thread.get_id() << " fstart inalized" << std::endl;

	this->finalize();

	std::cout << "Thread " << this->thread.get_id() << " finalized" << std::endl;
}

void Thread::start()
{
	this->thread = thread_t(&Thread::run, this);
}

void Thread::stop()
{
	std::cout << "unloop" << std::endl;
	std::cout << this->thread.get_id() << std::endl;

	if(this->thread.joinable())
		std::cout << " it's joinable" << std::endl;

	this->loop->break_loop(ev::ALL);

	this->thread.join();
	std::cout << "Thread " << this->thread.get_id() << " stopped" << std::endl;
}


