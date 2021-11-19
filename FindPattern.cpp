#include "FindPattern.h"
#include <utility>
#include <ostream>

// конструктор с параметрами с использованием move-семантики
FindPattern::FindPattern(std::string fileName, size_t lineNum, std::string line) :
    fileName(std::move(fileName)), lineNum(static_cast<int>(lineNum)), line(std::move(line)) {}

// переопределяем оператор вывода
std::ostream &operator<<(std::ostream & out, const FindPattern &result) {
   if (result.lineNum == -1) {
       out << "Sorry, but this pattern doesn't exist in such directory.\n";
       return out;
   }

   out << "File's name is " << result.fileName << "\n";
   out << "Line's number is " << result.lineNum << "\n";
   out << "Line is " << result.line << "\n";
   return out;
}
