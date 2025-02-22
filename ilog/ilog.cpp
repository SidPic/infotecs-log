#include "ilog.hpp"
#include <iomanip>
#include <cstring>

// Constructor & Destructor

Ilog::Ilog (const char* __logfile, int __loglevel, std::ios_base::openmode mode):
    logfile(__logfile, mode), loglevel(__loglevel)
{}

Ilog::~Ilog() {
    logfile.close();
}

// Write
void Ilog::write(std::string_view msg, int __loglevel, std::time_t time) {
    stream(__loglevel, time) << msg << std::endl; // write msg to log stream
}

// Stream

constexpr const char* msg_type[Ilog::DEFAULT] = {
    " [INFO] ", " [WARNING] ", " [ERROR] "
};

std::ofstream& Ilog::stream(int __loglevel, std::time_t time) {
    if (__loglevel == DEFAULT) __loglevel = loglevel;

    std::time_t t = (time) ?: std::time(nullptr); // get current time

    // puts a string like this: "16-02-2025 23:30:23 [INFO] "
    logfile << std::put_time(std::localtime(&t), time_fmt) << msg_type[__loglevel];

    return logfile;
}

// good

bool Ilog::good() const { return !!logfile; }

// get/set Loglevel

int Ilog::getLoglevel() const { return loglevel; }

void Ilog::setLoglevel(int __loglevel) { loglevel = __loglevel; }

// time format

const char* Ilog::getTimeFormat() const { return time_fmt; }

void Ilog::setTimeFormat(const char* fmt) { std::strncpy(time_fmt, fmt, TIME_FMT_LEN); }
