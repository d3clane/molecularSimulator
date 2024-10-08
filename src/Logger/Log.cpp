#include <assert.h>
#include <execinfo.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

#include "Logger/Log.h"

static int LOG_FILE;

static inline void PrintSeparator();
static void LogClose();

static inline size_t Min(size_t a, size_t b);
static int TryOpenFile(const char* name);

void LogOpen(const char* argv0)
{
    assert(argv0);
    LOG_FILE = TryOpenFile(argv0);

    if (LOG_FILE == -1)
        return;

    time_t timeInSeconds = time(nullptr);

    Log("<pre>\n\n");

    Log(HTML_RED_HEAD_BEGIN "\n" 
        "Log file was opened by program %s, compiled %s at %s. "
        "Opening time: %s"
        HTML_HEAD_END "\n", 
        argv0, __DATE__, __TIME__, ctime(&timeInSeconds));

    atexit(LogClose);
}

static void LogClose()
{
    if (LOG_FILE == -1)
        return;
    time_t timeInSeconds = time(nullptr);

    Log("\n" HTML_RED_HEAD_BEGIN "\n"
        "Log file was closed by program compiled %s at %s. "
        "Closing time: %s"
        HTML_HEAD_END "\n",
        __DATE__, __TIME__, ctime(&timeInSeconds));

    PrintSeparator();

    Log("</pre>\n");

    close(LOG_FILE);
    LOG_FILE = -1;
}

void LogBegin(const char* fileName, const char* funcName, const int line)
{
    assert(fileName);
    assert(funcName);

    if (LOG_FILE == -1)
        return;

    time_t timeInSeconds = time(nullptr);     

    Log("\n-----------------------\n\n"                            
        HTML_GREEN_HEAD_BEGIN "\n"                                 
        "New log called %s"                                        
        "Called from file: %s, from function: %s, from line: %d\n" 
        HTML_HEAD_END "\n\n\n",                                    
        ctime(&timeInSeconds), fileName, funcName, line);                                 

    static const size_t buffSize = 128;
    static void* buffer[buffSize];
    int numb = backtrace(buffer, buffSize);
    Log("Functions calling stack on beginning:\n");
    backtrace_symbols_fd(buffer, numb, LOG_FILE);
}

ssize_t Log(const char* format, ...)
{
    assert(format);

    va_list args = {};

    va_start(args, format);

    static const size_t BufSize = 1024;
    static char buf[BufSize];

    size_t numberOfChars = (size_t) vsnprintf(buf, BufSize, format, args);

    va_end(args);

    numberOfChars = Min(numberOfChars, BufSize);
    
    return write(LOG_FILE, buf, numberOfChars * sizeof(char));
}

ssize_t LogError(const char* format, ...)
{
    assert(format);

    va_list args = {};

    va_start(args, format);

    static const size_t BufSize = 1024;
    static char buf[BufSize];

    size_t numberOfChars = (size_t) vsnprintf(buf, BufSize, format, args);

    va_end(args);

    numberOfChars = Min(numberOfChars, BufSize);
    
    ssize_t numberOfPrintedChars = 0;
    
    //TODO: check each print result
    numberOfPrintedChars += write(LOG_FILE, HTML_RED_HEAD_BEGIN, sizeof(HTML_RED_HEAD_BEGIN) - 1);
    numberOfPrintedChars += write(LOG_FILE, buf, numberOfChars * sizeof(char));
    numberOfPrintedChars += write(LOG_FILE, HTML_HEAD_END,       sizeof(HTML_HEAD_END) - 1);
    
    return numberOfPrintedChars;
}

void LogEnd(const char* fileName, const char* funcName, const int line)
{
    static const size_t buffSize = 128;
    static void* buffer[buffSize];
    int numb = backtrace(buffer, buffSize);
    
    Log("Functions calling stack on ending:\n");
    backtrace_symbols_fd(buffer, numb, LOG_FILE);

    time_t timeInSeconds = time(nullptr);  
    Log("\n" HTML_GREEN_HEAD_BEGIN "\n"                       
        "Logging ended %s"                                    
        "Ended in file: %s, function: %s, line: %d\n"         
        HTML_HEAD_END "\n\n"                                  
        "-----------------------\n\n\n",                      
        ctime(&timeInSeconds), fileName, funcName, line);                                 
}

static inline void PrintSeparator()
{
    Log("\n\n---------------------------------------------------------------------------\n\n");
}

static inline size_t Min(size_t a, size_t b)
{
    return a < b ? a : b;
}

static int TryOpenFile(const char* name)
{
    const char* fileSuffix = ".log.html";

    const size_t  fileNameSize  = 256;
    char fileName[fileNameSize] = "";
    
    assert(strlen(name) + sizeof(fileSuffix) <= fileNameSize);
    snprintf(fileName, fileNameSize, "%s%s", name, fileSuffix);

    LOG_FILE = open(fileName, O_WRONLY | O_APPEND);

    if (LOG_FILE == -1)
    {
        creat(fileName, 0666);
        LOG_FILE = open(fileName, O_WRONLY | O_APPEND);
    }
    
    return LOG_FILE;
}