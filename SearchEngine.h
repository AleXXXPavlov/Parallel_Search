#ifndef PARALLEL_SEARCH_SEARCHENGINE_H
#define PARALLEL_SEARCH_SEARCHENGINE_H

#include <vector>

#include "Arguments.h"
#include "FindPattern.h"


enum {
    OK,                             // исправная работа программы
    INVALID_ARGUMENTS,              // неправильные аргументов командной строки
};


/*
 * Поисковый движок, осуществляющий управление всеми процессами, используемыми в решении задачи
 */
class SearchEngine {
public:
    static int start(int, char**);                                                // старт раоты поисковика
    std::vector<std::pair<std::string, size_t>> all_files;                        // список пар вида: имя файла, размер
    Arguments arguments;
private:
    explicit SearchEngine(Arguments );

    std::vector<std::pair<std::string, size_t>> getAllFiles();                  // получение всех файлов
    static void thread_working(const SearchEngine&, size_t, size_t);              // поиск в заданных файлах для одного потока
    std::vector<std::pair<size_t, size_t>> take_bolds();                       // распределение потоков
    static std::vector<FindPattern> SearchInFile(const string&, const string&);             // поиск в конкретном файле

    static std::vector<int> prefixFunction(const std::string&) ;                  // префикс-функция к строкам из файла

public:
    friend class Arguments;
    friend class FindPattern;
};


#endif //PARALLEL_SEARCH_SEARCHENGINE_H
