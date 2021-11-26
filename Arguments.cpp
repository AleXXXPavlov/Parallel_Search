#include "Arguments.h"


// вывод аргументов для проверки правильности парсинга
std::ostream &operator<<(std::ostream &out, const Arguments& args) {
    out << "Директория: " << args.root << "\n";
    out << "Паттерн: " << args.pattern << "\n";
    out << "Количество потоков для поиска: " << args.threads << "\n";
    out << "Нужно ли искать вглубь? - " << (args.is_deeper ? "Да" : "Нет") << "\n";
    return out;
}

// парсинг командной строки
Arguments::Arguments(int argc, char **argv) {
    // проверка на количество
    if (argc > 6) {
        std::cerr << "Oooops...you have too many args. Seriously???\n";
    }

    // распределяем значения
    // не рассматриваем 1-й аргумент - ./psearch
    // -t# и -n произвольное расположение - проверка в первую очередь
    // порядок ./psearch FILE DIR
    for (size_t i = 2; i < argc; ++i) {
        std::string curr_arg = argv[i];

        if (curr_arg.substr(0, 2) == "-n") {
            is_deeper = false;
            continue;
        }
        if (curr_arg.substr(0, 2) == "-t") {
            // оборачиваем в try-except дабы избежать выброса исключений
            try {
                threads = std::stoi(curr_arg.substr(2));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument instead of # in -t#: " << e.what();
            }

            continue;
        }
        if (pattern.empty()) {
            pattern = curr_arg;
            continue;
        }

        // если дошли до этого места - остался один аргумент, каталог, в котором ведется поиск
        root = curr_arg;
    }

    // проверка на корректность
    // проверка на пустоту строки
    if (pattern.empty()) {
        std::cerr << "An empty search string\n";
    }

    // проверка директории на существование
    if (!root.empty()) {
        if (!std::filesystem::exists(root)) {
            std::cerr << "Directory not exists\n";
        }
    }
}
