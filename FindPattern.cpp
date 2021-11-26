#include "FindPattern.h"
#include <utility>
#include <ostream>


//! Переопределенный вывод результата работы поиского движка
//! \param out поток вывода
//! \param result объект-результат поиска
//! \return поток вывода
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
