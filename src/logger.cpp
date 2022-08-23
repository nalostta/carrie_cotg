
#include "logger.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std::chrono;

static std::time_t time_now = std::time(nullptr);
std::string time_in_HH_MM_SS_MMM();
//
logger::logger(const char *filename,  int _omit, int trunc)
{
    if (trunc) logfile.open(filename);
    else logfile.open(filename,std::ofstream::trunc);
    if(logfile){
        isOpen=1;
        sw.restart();
    } 
    omit = _omit;
}

logger::logger(const std::string &filename, int _omit, int trunc)
{
    if (trunc) logfile.open(filename);
    else logfile.open(filename,std::ofstream::trunc);
    if(logfile){
        isOpen=1;
        sw.restart();
    }
    omit = _omit;
}

logger::~logger()
{
    logfile.close();
}

void logger::log(const char *msg)
{
    if ((not omit) & isOpen)
    {
        logfile << time_in_HH_MM_SS_MMM() << "\t: " << msg << std::endl;
    }
}

void logger::log(const std::string &msg)
{
    if ((not omit) & isOpen)
    {
        logfile << time_in_HH_MM_SS_MMM() << "\t: " << msg << std::endl; //
    }
}

void logger::logSL(const char *msg)
{
    if ((not omit) & isOpen)
    {
        logfile << std::endl << time_in_HH_MM_SS_MMM() << "\t: " << msg;  //
    }
}

void logger::logSL(const std::string &msg)
{
    if ((not omit) & isOpen)
    {
        logfile << std::endl << time_in_HH_MM_SS_MMM() << "\t: " << msg; //
    }
}

void logger::logNS(const char *msg)
{
    if ((not omit) & isOpen)
    {
        logfile << "\t" << msg;
    }
}

void logger::logNS(const std::string &msg)
{
    if ((not omit) & isOpen)
    {
        logfile << "\t" << msg;
    }
}

void logger::setOmit()
{
    omit = 1;
}

void logger::clearOmit()
{
    omit = 0;
}

int logger::isOmitSet()
{
    return omit;
}

std::string time_in_HH_MM_SS_MMM()
{
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    auto us = duration_cast<milliseconds>(now.time_since_epoch()) % 1000000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    oss << ':' << std::setfill('0') << std::setw(3) << ms.count() << ':' << std::setfill('0') << std::setw(3) << us.count();

    return oss.str();
}