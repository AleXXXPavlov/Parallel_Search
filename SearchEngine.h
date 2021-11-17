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
    static int start(int, char**);                                         // старт раоты поисковика
private:
    void thread_working(size_t, size_t);                                   // поиск в заданных файлах
    std::vector<std::pair<size_t, size_t>> take_bolds();                   // получение границ поиска файлов для каждого потока
    std::vector<FindPattern> SearchInFile();


public:
    friend class Arguments;
    friend class FindPattern;
};


#endif //PARALLEL_SEARCH_SEARCHENGINE_H
