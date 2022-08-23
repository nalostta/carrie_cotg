#ifndef __INCLUDED_CARRIE_LOGGER__
#define __INCLUDED_CARRIE_LOGGER__

#include <iostream>
#include <ctime>
#include <iomanip>

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)    // only show filename and not it's path (less clutter)
#define INFO std::cout << std::put_time(std::localtime(&time_now), "%y-%m-%d %OH:%OM:%OS") << " [INFO] " << __FILENAME__ << "(" << __FUNCTION__ << ":" << __LINE__ << ") >> "
#define ERROR std::cout << std::put_time(std::localtime(&time_now), "%y-%m-%d %OH:%OM:%OS") << " [ERROR] " << __FILENAME__ << "(" << __FUNCTION__ << ":" << __LINE__ << ") >> "

#include <iostream>
#include <fstream>
#include "stopwatch.hpp"

class logger
{
    std::ofstream logfile;
    int omit;
    int isOpen;
    std::chrono::time_point<std::chrono::high_resolution_clock> log_start_time;
    Stopwatch<> sw;
    
public:
    logger(const char *filename,  int _omit=0, int trunc=1);
    logger(const std::string &filename, int _omit=0, int trunc=1);
    ~logger();
    void log(const char *msg);
    void log(const std::string &msg);
    void logSL(const char *msg);
    void logSL(const std::string &msg);
    void logNS(const char *msg);
    void logNS(const std::string &msg);
    void setOmit();
    void clearOmit();
    int isOmitSet(); 
    inline int isFileOpen()
    {
        return isOpen;
    }
};

#endif