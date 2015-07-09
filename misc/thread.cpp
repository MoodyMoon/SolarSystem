#include "stdafx.h"
#include "thread.h"

Thread::Thread(ThreadCallbackInterface *thread_callback)
{
    thread.reset(new std::thread(&ThreadCallbackInterface::RunAsync, thread_callback));
}

Thread::~Thread()
{
    thread->join();
}