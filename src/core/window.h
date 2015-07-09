#ifndef CORE_WINDOW_H
#define CORE_WINDOW_H

#include "exception.h"
#include "object_callback_interface.h"

class WindowEventHandlerInterface;
class MouseEventHandlerInterface;
class KeyboardEventHandlerInterface;

class Window
{
    private:
        static std::unordered_map<GLFWwindow *, WindowEventHandlerInterface *> window_to_window_event_handler;
        static std::unordered_map<GLFWwindow *, MouseEventHandlerInterface *> window_to_mouse_event_handler;
        static std::unordered_map<GLFWwindow *, KeyboardEventHandlerInterface *> window_to_keyboard_event_handler;

        GLFWwindow *window = nullptr;

        bool has_window_event_handler = false;
        bool has_mouse_event_handler = false;
        bool has_keyboard_event_handler = false;

        std::vector<std::weak_ptr<ObjectCallbackInterface>> *object_callback_interfaces = nullptr;

        void Render(void);
        void Render2(std::vector<std::weak_ptr<ObjectCallbackInterface>>::const_iterator &object_callback_interface_iterator, std::vector<std::weak_ptr<ObjectCallbackInterface>>::const_iterator &object_callback_interface_end_iterator, std::weak_ptr<ObjectCallbackInterface> &object_callback_interface_weak_pointer, std::shared_ptr<ObjectCallbackInterface> &object_callback_interface_shared_pointer, double frame_swap_time);

        static void PrintError(int error, const char *description);
        static void GLFWframebuffersizefun(GLFWwindow *window, int width, int height);
        static void GLFWscrollfun(GLFWwindow *window, double xoffset, double yoffset);
        static void GLFWcursorposfun(GLFWwindow *window, double xpos, double ypos);
        static void GLFWkeyfun(GLFWwindow *window, int key, int scancode, int action, int mods);

    public:
        Window(const Window &) = delete;
        Window & operator=(const Window &) = delete;

        Window(int screen_x,
               int screen_y,
               unsigned int client_width,
               unsigned int client_height,
               const char *title,
               GLFWmonitor *monitor,
               WindowEventHandlerInterface *window_event_handler,
               MouseEventHandlerInterface *mouse_event_handler,
               KeyboardEventHandlerInterface *keyboard_event_handler,
               std::vector<std::weak_ptr<ObjectCallbackInterface>> *object_interfaces);

        void GetCursorPosition(double &x, double &y);
        static void GetScreenResolution(int &width, int &height);

        ~Window(void);
};

class WindowEventHandlerInterface
{
    protected:
        WindowEventHandlerInterface() = default;

    public:
        WindowEventHandlerInterface(const WindowEventHandlerInterface &) = delete;
        WindowEventHandlerInterface & operator=(const WindowEventHandlerInterface &) = delete;

        virtual void OnInit(Window *window) = 0;
        virtual void OnFrameBufferResize(int width, int height) = 0;

        virtual ~WindowEventHandlerInterface(void) {}
};

class MouseEventHandlerInterface
{
    protected:
        MouseEventHandlerInterface() = default;

    public:
        MouseEventHandlerInterface(const MouseEventHandlerInterface &) = delete;
        MouseEventHandlerInterface & operator=(const MouseEventHandlerInterface &) = delete;

        virtual void OnMove(double xpos, double ypos) = 0;
        virtual void OnScroll(double xoffset, double yoffset) = 0;

        virtual ~MouseEventHandlerInterface(void) {}
};

class KeyboardEventHandlerInterface
{
    protected:
        KeyboardEventHandlerInterface() = default;

    public:
        KeyboardEventHandlerInterface(const KeyboardEventHandlerInterface &) = delete;
        KeyboardEventHandlerInterface & operator=(const KeyboardEventHandlerInterface &) = delete;

        virtual void OnTrigger(int key, int scancode, int action, int mods) = 0;

        virtual ~KeyboardEventHandlerInterface(void) {}
};

#endif