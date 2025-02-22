#include "mthreadlogger.hpp"

#include <iostream>
#include <cstring>

// из ТЗ:
// c. Передавать принятые данные от пользователя в отдельный поток для записи в журнал. Передача данных должна быть потокобезопасной.
// d. Ожидать нового ввода от пользователя после передачи данных.
// Как я понял: не создавать новый поток, а передавать в уже существующий,
//   который пишет принимаемые данные в журнал, причем потокобезопасен должен быть именно сам процесс передачи данных
// ... то есть, как я понимаю, нужно, чтобы все данные (из разных потоков) передавались в одну очередь,
//        и потом из этой очереди, одним потоком записывались в файл.

void help_end_exit(const char* prog);
int str_to_loglevel(std::string_view);
std::string_view loglevel_to_str(MThreadLogger& logger);

int main(int argc, const char ** argv) {

    const char* logfile;
    int loglevel;

    // needs to 2 args: logfile and default loglevel
    if (argc < 3) help_end_exit(argv[0]);

    // validate loglevel
    if (strcmp(argv[2], "info") == 0) loglevel = Ilog::INFO; else
    if (strcmp(argv[2], "warning") == 0) loglevel = Ilog::WARNING; else
    if (strcmp(argv[2], "error") == 0) loglevel = Ilog::ERROR;
    else
        help_end_exit(argv[0]);

    logfile = argv[1];

    // thread-safe application demo

    // create logger and run logger thread
    // можно изменить режим логгирования  ---------|
    // на добавление в файл вместо перезаписывания V
    MThreadLogger logger(logfile, loglevel /*, std::ios::app  */);

    // check logfile stream
    if (logger.good())
        std::cout << "[The logger filestream has opened successfully]" << std::endl;
    else
        std::cout << "[Couldn't to open the log file]" << std::endl;

    std::cout << "Приветствую в программе для проверки логгера! Сначала введите сообщение, нажмитие ввод, а затем введите уровень важности: info, warning или error." << std::endl
              << "Чтобы выйти из программы введите Ctrl+D (конец ввода)"
              << std::endl;

    // main loop
    for (std::string str, loglevel_str;;)
    {
        std::cout << "Log message: ";
        if (!getline(std::cin, str)) break;

        std::cout << "Log level (default " << loglevel_to_str(logger) << "): ";
        if (!getline(std::cin, loglevel_str)) break;

        // передача данных
        logger.push(str, str_to_loglevel(loglevel_str));
    }

    // features

    //~ logger.push("It works");
    //~ logger.push("It's warning msg", Ilog::WARNING);

    //~ logger.setLoglevel(Ilog::ERROR); // change default loglevel
    //~ logger.setTimeFormat("[%d/%m/%Y] %H:%M:%S");   // change time format

    //~ logger.push("It's time-formated error message");


/*
    // thread_UN_safe lib demo

    Ilog logger("log.txt", Ilog::INFO);

    if (logger.good()) puts("ok"); else puts("nok");

    logger.write("Hello world!");

    logger.setLoglevel(Ilog::ERROR); // change default loglevel
    logger.setTimeFormat("[%d/%m/%Y] %H:%M:%S");   // change time format

    logger.stream() << "I found " << 12 << " apples" << std::endl;
*/

    return 0;
}

void help_end_exit(const char* prog) {
    std::cout << std::endl
              <<"Using: " << prog << " [logfile] [loglevel]" << std::endl
              << "\twhere loglevel can be: info, warning or error" << std::endl
              << std::endl;
    exit(EXIT_FAILURE);
}

int str_to_loglevel(std::string_view str) {
    if (str == "info") return Ilog::INFO;
    if (str == "warning") return Ilog::WARNING;
    if (str == "error") return Ilog::ERROR;
    else return Ilog::DEFAULT;
}

std::string_view loglevel_to_str(MThreadLogger& logger) {

    static constexpr const char* loglevel_text[Ilog::DEFAULT] = {
        "info", "waring", "error"
    };

    return loglevel_text[logger.getLoglevel()];
}
