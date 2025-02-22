#ifndef MTHREADLOGGER_HPP
#define MTHREADLOGGER_HPP

#include "ilog/ilog.hpp"

#include <queue>
#include <mutex>
#include <thread>
#include <atomic>


// Log node for LogChan
struct Log {
    Log(std::string_view m, int l, std::time_t t):
        msg(m), loglevel(l), time(t)
    {}

    std::string msg;
    char loglevel;
    std::time_t time;
};


// queue with logs to be wrote
using LogChan = std::queue<Log>;


// thread-safe Ilog wrapper
class MThreadLogger : private Ilog, private LogChan {
public:

    // init Ilog with the logfile @logfile and the minimum loglevel = @loglevel
    //    loglevel can be: Ilog::INFO, Ilog::WARNING, Ilog::ERROR
    //    mode is c++ filestream open mode
    // and run logger thread
    MThreadLogger(const char* logfile, int loglevel, std::ios_base::openmode mode=std::ios::out):
        Ilog::Ilog(logfile, loglevel, mode),    // Ilog-base init
        write_thread(&MThreadLogger::run, this) // logging thread start
    {}

    // checks if the file has been opened successfully
    bool good();

    // push log to write
    void push(std::string_view msg, int loglevel=Ilog::DEFAULT);

    // log writing loop
    void run();

    // set & get default loglevel
    void setLoglevel (int loglevel);
    int getLoglevel ();

    // time format (32bytes time-format c-string)
    void setTimeFormat (const char* fmt);
    const char* getTimeFormat ();

    ~MThreadLogger();

private:

    mutable std::mutex mtx;
    std::thread write_thread;
    std::atomic<bool> stop = false;

    static constexpr const int TIMEOUT = 10; // thread sleep timeout (as milliseconds)
};

#endif /* MTHREADLOGGER_HPP */
