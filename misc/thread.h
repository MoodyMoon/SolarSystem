#ifndef CORE_THREAD_H
#define CORE_THREAD_H

class ThreadCallbackInterface;

class Thread
{
    private:
        std::unique_ptr<std::thread> thread;

    public:
        Thread(const Thread &) = delete;
        Thread & operator=(const Thread &) = delete;

        Thread(ThreadCallbackInterface *thread_callback);

        ~Thread(void);
};

class ThreadCallbackInterface
{
    public:
        ThreadCallbackInterface() = default;
        ThreadCallbackInterface(const ThreadCallbackInterface &) = delete;
        ThreadCallbackInterface & operator=(const ThreadCallbackInterface &) = delete;

        virtual void RunAsync(void) = 0;

        virtual ~ThreadCallbackInterface() {}
};

#endif