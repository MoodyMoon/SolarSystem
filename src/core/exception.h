#ifndef CORE_EXCEPTION_H
#define CORE_EXCEPTION_H

class Exception : public std::exception
{
    public:
        enum ID
        {
            GLFW_EXCEPTION,
            GLEW_EXCEPTION,
            SEEK_EXCEPTION,
            READ_FILE_EXCEPTION
        };

    private:
        ID exception_id;
        std::string caller_name;
        bool has_code;
        int code;
        std::string message;
        std::string what_message;

        void CreateWhatMessage(void);

    protected:
        Exception(const char * const caller_name, bool has_code, int code, const char * const message, ID exception_id);

    public:
        const char * GetCallerName(void) const noexcept;
        void SetCallerName(std::string &caller_name);
        bool HasCode(void) const noexcept;
        int GetErrorCode(void) const noexcept;
        void SetErrorCode(int code);
        const char * GetErrorMessage(void) const noexcept;
        ID GetExceptionID(void) const noexcept;
        void SetErrorMessage(std::string &message);
        virtual const char * what() const noexcept;

        virtual ~Exception(void) {}
};

class GLFWException : public Exception
{
    public:
        GLFWException(const char * const caller_name, const char * const message);
        GLFWException(const char * const caller_name, int code, const char * const message);
};

class GLEWException : public Exception
{
    public:
        GLEWException(const char * const caller_name, const char * const message);
        GLEWException(const char * const caller_name, int code, const char * const message);
};

class SeekException : public Exception
{
    public:
        SeekException(const char * const caller_name, const char * const message);
        SeekException(const char * const caller_name, int code, const char * const message);
};

class ReadFileException : public Exception
{
    public:
        ReadFileException(const char * const caller_name, const char * const message);
        ReadFileException(const char * const caller_name, int code, const char * const message);
};

#endif