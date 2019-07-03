using size_t = unsigned int;
using ssize_t = int;

static constexpr int STDIN_FILENO = 0;    // standard input file descriptor
static constexpr int STDOUT_FILENO = 1;    // standard output file descriptor
static constexpr int STDERR_FILENO = 2;    // standard error file descriptor

extern "C"
{
    ssize_t write(int fildes, const void *buf, size_t nbytes);
}

enum Platform
{
    WINDOWS,
    MACOS,
    LINUX
};

template <int>
class Application
{
};

template <>
class Application<WINDOWS>
{
public:
    static void log(const char* s, size_t length)
    {
    }

    static void error(const char* s, size_t length)
    {
    }

    void run()
    {
    }
};

template <>
class Application<MACOS>
{
public:
    static void log(const char* s, size_t length)
    {
        write(STDOUT_FILENO, s, length);
    }

    static void error(const char* s, size_t length)
    {
        write(STDERR_FILENO, s, length);
    }

    void run()
    {
        log("macOS\n", 6);
    }
};

template <>
class Application<LINUX>
{
public:
    static void log(const char* s, size_t length)
    {
        write(STDOUT_FILENO, s, length);
    }

    static void error(const char* s, size_t length)
    {
        write(STDERR_FILENO, s, length);
    }

    void run()
    {
        log("Linux\n", 6);
    }
};

int main(int argc, const char * argv[])
{
    try
    {
        Application<PLATFORM> application;
        application.run();
    }
    catch (...)
    {
        Application<PLATFORM>::error("Error\n", 6);
    }

    return 0;
}
