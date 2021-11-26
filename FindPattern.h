#ifndef PARALLEL_SEARCH_FINDPATTERN_H
#define PARALLEL_SEARCH_FINDPATTERN_H

#include <string>

class FindPattern {
public:
    std::string fileName;                                              // имя файла
    int lineNum;                                                  // номер искомой строки
    std::string line;                                                  // искомая строка

    friend std::ostream& operator<< (std::ostream&, const FindPattern&);   // переопределенный оператор вывода
};


#endif //PARALLEL_SEARCH_FINDPATTERN_H
