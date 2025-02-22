#ifndef ILOG_HPP
#define ILOG_HPP

#include <string_view>
#include <fstream>
#include <ctime>

// basic thread_UN_unsafe logger
class Ilog {
public:

    // loglevels
    enum { INFO, WARNING, ERROR, DEFAULT };

    // init ilog with the logfile @logfile and the minimum loglevel = @loglevel
    // loglevel can be: Ilog::INFO, Ilog::WARNING, Ilog::ERROR
    // mode is c++ filestream open mode
    Ilog(const char* logfile, int loglevel, std::ios_base::openmode mode=std::ios::out);

    // write log msg
    void write(std::string_view msg, int loglevel=DEFAULT, std::time_t time=0);

    // returns prepared ofstream for write with operator <<
    // and/or for check file-open errors [ if (!ilog.stream()) { handle_error(); } ]
    // ! it doesn't put a new line
    std::ofstream& stream(int loglevel=DEFAULT, std::time_t time=0);

    // checks if the file has been opened successfully
    bool good() const;

    // set & get gefault loglevel
    void setLoglevel(int loglevel);
    int  getLoglevel() const;

    ~Ilog();

    // time format (32bytes time-format c-string)
    void setTimeFormat(const char*);
    const char* getTimeFormat() const;

    static constexpr const size_t TIME_FMT_LEN = 32;
private:

    std::ofstream logfile;
    int loglevel;
    char time_fmt[TIME_FMT_LEN] {"%d-%m-%Y %H:%M:%S"};

}; /* class Ilog */


#endif /* ILOG_HPP */
