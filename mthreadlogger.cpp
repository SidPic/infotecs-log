#include "mthreadlogger.hpp"

bool MThreadLogger::good() {
    std::lock_guard lock(mtx);
    return Ilog::good();
}

// push

void MThreadLogger::push(std::string_view msg, int loglevel) {
    std::time_t time = std::time(nullptr);  // get current time

    std::lock_guard lock(mtx);

    emplace(msg, loglevel, time);   // push log
}

// run

void MThreadLogger::run() {
    while (!stop) {
        {
            std::lock_guard lock(mtx);

            if (!empty()) {
                Log& log = front();
                write(log.msg, log.loglevel, log.time); // write log
                pop(); // pop log from LogChan
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(TIMEOUT));
    }
}

// loglevel

void MThreadLogger::setLoglevel (int loglevel) {
    std::lock_guard lock(mtx);

    // before this change we need to write the logs in previous format
    while (!empty()) {
        Log& log = front();
        write(log.msg, log.loglevel, log.time);
        pop();
    }

    // change loglevel
    Ilog::setLoglevel(loglevel);
}

int MThreadLogger::getLoglevel () {
    std::lock_guard lock(mtx);
    return Ilog::getLoglevel();
}

// time format

void MThreadLogger::setTimeFormat (const char* fmt) {
    std::lock_guard lock(mtx);

    // before this change we need to write the logs in previous format
    while (!empty()) {
        Log& log = front();
        write(log.msg, log.loglevel, log.time);
        pop();
    }

    // change time format
    Ilog::setTimeFormat(fmt);
}

const char* MThreadLogger::getTimeFormat () {
    std::lock_guard lock(mtx);
    return Ilog::getTimeFormat();
}

// destructor

MThreadLogger::~MThreadLogger() {
    stop = true; // stop the logger thread
    if (write_thread.joinable()) {
        write_thread.join();
    }

    // write a remaining logs
    while (!empty()) {
        Log& log = front();
        write(log.msg, log.loglevel, log.time);
        pop();
    }
}
