using size_t = unsigned int;
using ssize_t = int;

static constexpr int STDIN_FILENO = 0;    // standard input file descriptor
static constexpr int STDOUT_FILENO = 1;    // standard output file descriptor
static constexpr int STDERR_FILENO = 2;    // standard error file descriptor

using BOOL = int;
using VOID = void;
using PVOID = void*;
using LPVOID = void*;
using HANDLE = PVOID;
using DWORD = unsigned long;
using LPDWORD = DWORD*;

static constexpr DWORD STD_INPUT_HANDLE = static_cast<DWORD>(-10);
static constexpr DWORD STD_OUTPUT_HANDLE = static_cast<DWORD>(-11);
static constexpr DWORD STD_ERROR_HANDLE = static_cast<DWORD>(-12);

extern "C"
{
    HANDLE __stdcall GetStdHandle(DWORD nStdHandle);
    BOOL __stdcall WriteConsoleA(HANDLE  hConsoleOutput, const VOID *lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved);
    ssize_t write(int fildes, const void *buf, size_t nbytes);
}

enum Platform
{
    WINDOWS,
    MACOS,
    LINUX
};

enum class Output
{
    STDOUT,
    STDERR
};

static size_t getStringLength(const char* str)
{
    size_t result = 0;
    while (str[result] != 0)
        ++result;
    return result;
}


template <int>
class Application;

template <>
class Application<WINDOWS>
{
public:
    static constexpr DWORD getOutput(Output output)
    {
        return (output == Output::STDOUT) ? STD_OUTPUT_HANDLE :
            (output == Output::STDERR) ? STD_ERROR_HANDLE :
            throw "Invalid output";
    }

    static void log(const char* s, Output output)
    {
        size_t length = getStringLength(s);

        while (length)
        {
            DWORD result;
            if (!WriteConsoleA(GetStdHandle(getOutput(output)), reinterpret_cast<const VOID*>(s), length, &result, nullptr))
                throw "Failed to write to console";

            s += result;
            length -= static_cast<size_t>(result);
        }
    }

    void run()
    {
        log("Windows\n", Output::STDOUT);
    }
};

template <>
class Application<MACOS>
{
public:
    static constexpr int getOutput(Output output)
    {
        return (output == Output::STDOUT) ? STDOUT_FILENO :
            (output == Output::STDERR) ? STDERR_FILENO :
        throw "Invalid output";
    }

    static void log(const char* s, Output output)
    {
        size_t length = getStringLength(s);

        while (length)
        {
            auto result = write(getOutput(output), s, length);
            if (result == -1)
                throw "Failed to write to console";

            s += result;
            length -= static_cast<size_t>(result);
        }
    }

    void run()
    {
        log("macOS\n", Output::STDOUT);
    }
};

template <>
class Application<LINUX>
{
public:
    static constexpr int getOutput(Output output)
    {
        return (output == Output::STDOUT) ? STDOUT_FILENO :
            (output == Output::STDERR) ? STDERR_FILENO :
            throw "Invalid output";
    }

    static void log(const char* s, Output output)
    {
        size_t length = getStringLength(s);

        while (length)
        {
            auto result = write(getOutput(output), s, length);
            if (result == -1)
                throw "Failed to write to console";
            
            s += result;
            length -= static_cast<size_t>(result);
        }
    }

    void run()
    {
        log("Linux\n", Output::STDOUT);
    }
};

int main()
{
    try
    {
        Application<PLATFORM> application;
        application.run();
    }
    catch (const char* e)
    {
        Application<PLATFORM>::log(e, Output::STDERR);
    }
    catch (...)
    {
        Application<PLATFORM>::log("Error\n", Output::STDERR);
    }

    return 0;
}
