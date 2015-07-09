#include "stdafx.h"
#include "window.h"

std::unordered_map<GLFWwindow *, WindowEventHandlerInterface *> Window::window_to_window_event_handler;
std::unordered_map<GLFWwindow *, MouseEventHandlerInterface *> Window::window_to_mouse_event_handler;
std::unordered_map<GLFWwindow *, KeyboardEventHandlerInterface *> Window::window_to_keyboard_event_handler;

Window::Window(int screen_x,
               int screen_y,
               unsigned int client_width,
               unsigned int client_height,
               const char *title,
               GLFWmonitor *monitor,
               WindowEventHandlerInterface *window_event_handler,
               MouseEventHandlerInterface *mouse_event_handler,
               KeyboardEventHandlerInterface *keyboard_event_handler,
               std::vector<std::weak_ptr<ObjectCallbackInterface>> *object_callback_interfaces) : object_callback_interfaces(object_callback_interfaces)
{
    glfwSetErrorCallback(&Window::PrintError);

    if(glfwInit() != GL_TRUE)
        throw GLFWException("MainForm", "Unable to initialize GLFW.");
    else
        std::cout << "GLFW initialized\n";

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(client_width, client_height, title, monitor, nullptr);

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowPos(window, screen_x, screen_y);
    glfwShowWindow(window);

    if(window == nullptr)
    {
        glfwTerminate();
        throw GLFWException("MainForm", "Unable to create window.");
    }

    assert(window != nullptr);

    GLenum error_code = glewInit();

    if(error_code != GLEW_OK)
    {
        throw GLEWException("MainForm", error_code, reinterpret_cast<const char *>(glewGetErrorString(error_code)));
    }
    else
    {
        std::cout << "GLEW initialized\n";
    }

    assert(error_code == GLEW_OK);

    window_event_handler->OnInit(this);

    if(window_event_handler != nullptr)
    {
        glfwSetFramebufferSizeCallback(window, &Window::GLFWframebuffersizefun);

        window_to_window_event_handler.insert({{window, window_event_handler}});

        has_window_event_handler = true;
    }

    if(mouse_event_handler != nullptr)
    {
        glfwSetScrollCallback(window, &Window::GLFWscrollfun);
        glfwSetCursorPosCallback(window, &Window::GLFWcursorposfun);

        window_to_mouse_event_handler.insert({{window, mouse_event_handler}});

        has_mouse_event_handler = true;
    }

    if(keyboard_event_handler != nullptr)
    {
        glfwSetKeyCallback(window, &Window::GLFWkeyfun);

        window_to_keyboard_event_handler.insert({{window, keyboard_event_handler}});

        has_keyboard_event_handler = true;
    }

    Render();
}

void Window::GetCursorPosition(double &x, double &y)
{
    glfwGetCursorPos(window, &x, &y);
}

void Window::GetScreenResolution(int &width, int &height)
{
    if(glfwInit() != GL_TRUE)
        throw GLFWException("MainForm", "Unable to initialize GLFW.");
    else
        std::cout << "GLFW initialized\n";

    GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
    assert(primary_monitor != nullptr);
    const GLFWvidmode *video_mode = glfwGetVideoMode(primary_monitor);
    assert(video_mode != nullptr);
    width = video_mode->width;
    height = video_mode->height;
}

void Window::Render()
{
    std::vector<std::weak_ptr<ObjectCallbackInterface>>::const_iterator object_callback_interface_iterator;
    std::vector<std::weak_ptr<ObjectCallbackInterface>>::const_iterator object_callback_interface_end_iterator;

    std::weak_ptr<ObjectCallbackInterface> object_callback_interface_weak_pointer;
    std::shared_ptr<ObjectCallbackInterface> object_callback_interface_shared_pointer;

    double previous_time = 0.0;
    double current_time = 0.0;
    double frame_swap_time = 0.0;

    Render2(object_callback_interface_iterator, object_callback_interface_end_iterator, object_callback_interface_weak_pointer, object_callback_interface_shared_pointer, frame_swap_time);

    while(glfwWindowShouldClose(window) == 0)
    {
        previous_time = glfwGetTime();
        glfwSwapBuffers(window);
        current_time = glfwGetTime();

        frame_swap_time = current_time - previous_time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Render2(object_callback_interface_iterator, object_callback_interface_end_iterator, object_callback_interface_weak_pointer, object_callback_interface_shared_pointer, frame_swap_time);

        glfwPollEvents();
    }
}

void Window::Render2(std::vector<std::weak_ptr<ObjectCallbackInterface>>::const_iterator &object_callback_interface_iterator, std::vector<std::weak_ptr<ObjectCallbackInterface>>::const_iterator &object_callback_interface_end_iterator, std::weak_ptr<ObjectCallbackInterface> &object_callback_interface_weak_pointer, std::shared_ptr<ObjectCallbackInterface> &object_callback_interface_shared_pointer, double frame_swap_time)
{
    if(object_callback_interfaces != nullptr)
    {
        object_callback_interface_iterator = object_callback_interfaces->begin();
        object_callback_interface_end_iterator = object_callback_interfaces->end();

        while(object_callback_interface_iterator < object_callback_interface_end_iterator)
        {
            object_callback_interface_weak_pointer = *object_callback_interface_iterator;

            if(object_callback_interface_weak_pointer.expired())
            {
                object_callback_interfaces->erase(object_callback_interface_iterator);
            }
            else
            {
                object_callback_interface_shared_pointer = object_callback_interface_weak_pointer.lock();
                object_callback_interface_shared_pointer->Do(frame_swap_time);
                object_callback_interface_shared_pointer.reset();
            }

            ++object_callback_interface_iterator;
        }
    }
}

Window::~Window()
{
    if(has_window_event_handler == true)
    {
        glfwSetFramebufferSizeCallback(window, nullptr);
        glfwSetWindowRefreshCallback(window, nullptr);

        window_to_window_event_handler.erase(window);

        has_window_event_handler = false;
    }

    if(has_mouse_event_handler == true)
    {
        glfwSetScrollCallback(window, nullptr);
        glfwSetCursorPosCallback(window, nullptr);

        window_to_mouse_event_handler.erase(window);

        has_mouse_event_handler = false;
    }

    if(has_keyboard_event_handler == true)
    {
        glfwSetKeyCallback(window, nullptr);

        window_to_keyboard_event_handler.erase(window);

        has_keyboard_event_handler = false;
    }

    glfwTerminate();

    std::cout << "GLFW de-initialized\n";
}

void Window::PrintError(int error, const char *description)
{
    std::string error_message(std::string("GLFW internal error: ") + std::to_string(error) + description + '\n');
    std::cerr << error_message;
}

void Window::GLFWframebuffersizefun(GLFWwindow *window, int width, int height)
{
    (window_to_window_event_handler.at(window))->OnFrameBufferResize(width, height);
}

void Window::GLFWscrollfun(GLFWwindow *window, double xoffset, double yoffset)
{
    (window_to_mouse_event_handler.at(window))->OnScroll(xoffset, yoffset);
}

void Window::GLFWcursorposfun(GLFWwindow *window, double xpos, double ypos)
{
    (window_to_mouse_event_handler.at(window))->OnMove(xpos, ypos);
}

void Window::GLFWkeyfun(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (window_to_keyboard_event_handler.at(window))->OnTrigger(key, scancode, action, mods);
}