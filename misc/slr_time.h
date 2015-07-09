#ifndef SLR_TIME_H
#define SLR_TIME_H

#include "thread.h"

class TimeCallbackInterface;

class Time : public ThreadCallbackInterface
{
    private:
        static const std::chrono::milliseconds time_advance_speed;

        std::unique_ptr<Thread> timer;
        std::vector<std::weak_ptr<TimeCallbackInterface>> *time_callback_interfaces = nullptr;

        std::atomic<bool> started;

        virtual void RunAsync(void);

    public:
        Time(const Time &) = delete;
        Time & operator=(const Time &) = delete;

        void Start(void);
        void Stop(void);

        Time(std::vector<std::weak_ptr<TimeCallbackInterface>> *time_callback_interfaces);

};

class TimeCallbackInterface
{
    public:
        TimeCallbackInterface() = default;
        TimeCallbackInterface(const TimeCallbackInterface &) = delete;
        TimeCallbackInterface & operator=(const TimeCallbackInterface &) = delete;

        virtual void Advance(void) = 0;

        virtual ~TimeCallbackInterface(void) {}
};

#endif