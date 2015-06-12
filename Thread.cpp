#include "Thread.h"
#include "easylogging++.h"

Thread::Thread():ev_loop_flags(0)
{
	LOG(INFO) << "Thread " << this->thread.get_id() << " created";
}

Thread::~Thread()
{
	LOG(INFO) << "Thread destroyed";
}

void Thread::run()
{
	this->loop = new ev::dynamic_loop(this->ev_loop_flags);
	this->init();

	this->stopper.set(*this->loop);
	this->stopper.set<Thread, &Thread::stop_handler>(this);
	this->stopper.start();

	LOG(INFO) << "Thread " << this->thread.get_id() << " started";

	this->loop->run(0);

	LOG(INFO) << "Thread " << this->thread.get_id() << " start finalized";

	this->finalize();

	LOG(INFO) << "Thread " << this->thread.get_id() << " finalized";
}

void Thread::start()
{
	this->thread = thread_t(&Thread::run, this);
}

void Thread::stop()
{
	this->stopper.send();
	this->thread.join();
}

void Thread::stop_handler()
{
	this->on_stop();

	this->stopper.stop();
	this->loop->break_loop(ev::ALL);
}



