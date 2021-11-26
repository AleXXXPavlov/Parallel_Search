#include "FindPattern.h"
#include <utility>
#include <ostream>

// переопределяем оператор вывода
std::ostream &operator<<(std::ostream & out, const FindPattern &result) {
    if (result.lineNum == -1) {
        out << "Sorry, but this pattern doesn't exist in such directory.\n";
        return out;
    }

    out << "Имя файла: " << result.fileName << "\n";
    out << "Номер строки: " << result.lineNum << "\n";
    out << "Строка: " << result.line << "\n";
    return out;
}
