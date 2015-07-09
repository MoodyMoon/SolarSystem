#include "stdafx.h"
#include "slr_time.h"

const std::chrono::milliseconds Time::time_advance_speed(1);

Time::Time(std::vector<std::weak_ptr<TimeCallbackInterface>> *time_callback_interfaces) : time_callback_interfaces(time_callback_interfaces), started(false) {}

void Time::Start()
{
    if(!started.load())
    {
        started.store(true);
        timer.reset(new Thread(this));
    }
}

void Time::Stop()
{
    if(started.load())
    {
        started.store(false);
        timer.reset(new Thread(this));
    }
}

void Time::RunAsync()
{
    std::vector<std::weak_ptr<TimeCallbackInterface>>::iterator time_callback_interface_iterator;
    std::vector<std::weak_ptr<TimeCallbackInterface>>::iterator time_callback_interface_end_iterator;

    std::weak_ptr<TimeCallbackInterface> time_callback_interface_weak_pointer;
    std::shared_ptr<TimeCallbackInterface> time_callback_interface_shared_pointer;

    while(started.load())
    {
        if(time_callback_interfaces != nullptr)
        {
            time_callback_interface_iterator = time_callback_interfaces->begin();
            time_callback_interface_end_iterator = time_callback_interfaces->end();

            while(time_callback_interface_iterator < time_callback_interface_end_iterator)
            {
                time_callback_interface_weak_pointer = *time_callback_interface_iterator;

                if(time_callback_interface_weak_pointer.expired())
                {
                    time_callback_interfaces->erase(time_callback_interface_iterator);
                }
                else
                {
                    time_callback_interface_shared_pointer = time_callback_interface_weak_pointer.lock();
                    time_callback_interface_shared_pointer->Advance();
                    time_callback_interface_shared_pointer.reset();
                }

                ++time_callback_interface_iterator;
            }
        }

        std::this_thread::sleep_for(time_advance_speed);
    }
}