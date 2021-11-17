#ifndef PARALLEL_SEARCH_ARGUMENTS_H
#define PARALLEL_SEARCH_ARGUMENTS_H

#include <string>
#include <filesystem>
#include <iostream>
#include "SearchEngine.h"


/*
 * Интерфейс для работы с аргументами командной строки
 */
class Arguments {
public:
    const int threads = 1;                                                            // количество потоков работы поиска
    mutable std::string root = std::filesystem::current_path().root_name();           // имя корневого каталога
    mutable std::string pattern;                                                      // строка, которую требуется найти
    const bool is_deeper = true;                                    // false - только текущей директория, true - поиск вглубь

    Arguments() = default;
    Arguments(int, std::string, std::string, bool);

    static Arguments parse_command(int, char**);                    // парсинг аргументов командной строки
};


#endif //PARALLEL_SEARCH_ARGUMENTS_H
