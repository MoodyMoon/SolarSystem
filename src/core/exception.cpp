#include "stdafx.h"
#include "exception.h"

Exception::Exception(const char * const caller_name, bool has_code, int code, const char * const message, ID exception_id) : exception_id(exception_id),
                                                                                                                             caller_name(caller_name),
                                                                                                                             has_code(has_code),
                                                                                                                             code(code)
{
    assert(caller_name != nullptr);
    assert(message != nullptr);

    switch(exception_id)
    {
        case Exception::ID::GLFW_EXCEPTION:
            this->message = "GLFW Error - ";
            break;
        case Exception::ID::GLEW_EXCEPTION:
            this->message = "GLEW Error - ";
            break;
        default:
            break;
    }

    this->message.append(message);

    CreateWhatMessage();
}

void Exception::CreateWhatMessage(void)
{
    what_message = caller_name;

    if(has_code)
    {
        what_message.append(" error ");
        what_message.append(std::to_string(code));
    }

    what_message.append(": ");
    what_message.append(message);
}

const char * Exception::GetCallerName(void) const noexcept
{
    return caller_name.c_str();
}

void Exception::SetCallerName(std::string &caller_name)
{
    this->caller_name = caller_name;
    CreateWhatMessage();
}

bool Exception::HasCode(void) const noexcept
{
    return has_code;
}

int Exception::GetErrorCode(void) const noexcept
{
    return code;
}
void Exception::SetErrorCode(int code)
{
    this->code = code;
    CreateWhatMessage();
}

const char * Exception::GetErrorMessage(void) const noexcept
{
    return message.c_str();
}

Exception::ID Exception::GetExceptionID(void) const noexcept
{
    return exception_id;
}

void Exception::SetErrorMessage(std::string &message)
{
    this->message = message;
    CreateWhatMessage();
}

const char * Exception::what() const noexcept
{
    return what_message.c_str();
}

GLFWException::GLFWException(const char * const caller_name, const char * const message) : Exception(caller_name, false, 0, message, Exception::ID::GLFW_EXCEPTION) {}
GLFWException::GLFWException(const char * const caller_name, int code, const char * const message) : Exception(caller_name, true, code, message, Exception::ID::GLFW_EXCEPTION) {}

GLEWException::GLEWException(const char * const caller_name, const char * const message) : Exception(caller_name, false, 0, message, Exception::ID::GLEW_EXCEPTION) {}
GLEWException::GLEWException(const char * const caller_name, int code, const char * const message) : Exception(caller_name, true, code, message, Exception::ID::GLEW_EXCEPTION) {}

SeekException::SeekException(const char * const caller_name, const char * const message) : Exception(caller_name, false, 0, message, Exception::ID::SEEK_EXCEPTION) {}
SeekException::SeekException(const char * const caller_name, int code, const char * const message) : Exception(caller_name, true, code, message, Exception::ID::SEEK_EXCEPTION) {}

ReadFileException::ReadFileException(const char * const caller_name, const char * const message) : Exception(caller_name, false, 0, message, Exception::ID::READ_FILE_EXCEPTION) {}
ReadFileException::ReadFileException(const char * const caller_name, int code, const char * const message) : Exception(caller_name, true, code, message, Exception::ID::READ_FILE_EXCEPTION) {}