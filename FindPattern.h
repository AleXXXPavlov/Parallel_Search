#ifndef PARALLEL_SEARCH_FINDPATTERN_H
#define PARALLEL_SEARCH_FINDPATTERN_H

#include <string>

/*
 * Полученный результат - имя файла, номер строки, в которой был найден pattern, сама искомая строка
 */
class FindPattern {
public:
    const std::string fileName;                                              // имя файла
    const int lineNum = -1;                                                  // номер искомой строки
    const std::string line;                                                  // искомая строка

    FindPattern() = default;
    FindPattern(std::string, size_t, std::string);

    friend std::ostream& operator<< (std::ostream&, const FindPattern&);   // переопределенный оператор вывода
};



#endif //PARALLEL_SEARCH_FINDPATTERN_H
