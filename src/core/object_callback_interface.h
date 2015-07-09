#ifndef CORE_OBJECT_CALLBACK_INTERFACE_H
#define CORE_OBJECT_CALLBACK_INTERFACE_H

class ObjectCallbackInterface
{
    protected:
        ObjectCallbackInterface() = default;

    public:
        ObjectCallbackInterface(const ObjectCallbackInterface &) = delete;
        ObjectCallbackInterface & operator=(const ObjectCallbackInterface &) = delete;

        virtual void Do(double frame_swap_time) = 0;

        virtual ~ObjectCallbackInterface(void) {}
};

#endif