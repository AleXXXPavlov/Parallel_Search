#ifndef PARALLEL_SEARCH_ARGUMENTS_H
#define PARALLEL_SEARCH_ARGUMENTS_H

#include <string>
#include <filesystem>
#include <iostream>


//! Коды ошибок, используемых в программе
enum {
    OK,                             // исправная работа программы
    INVALID_ARGUMENTS,              // неправильные аргументов командной строки
};


//! Интерфейс для работы с аргументами командной строки
class Arguments {
public:
    int threads = 1;                                                          // количество потоков работы поиска
    std::string root = std::filesystem::current_path().root_name();           // имя корневого каталога
    std::string pattern;                                                      // строка, которую требуется найти
    bool is_deeper = true;                              // false - только текущей директория, true - поиск вглубь

    explicit Arguments(int, char**);                    // парсинг аргументов командной строки
    friend std::ostream& operator<< (std::ostream& out, const Arguments&);
};


#endif //PARALLEL_SEARCH_ARGUMENTS_H
