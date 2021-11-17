#include "Arguments.h"


// конструктор с параметрами
Arguments::Arguments(int threads, std::string root, std::string pattern, bool is_deeper) :
    threads(threads), root(std::move(root)), pattern(std::move(pattern)), is_deeper(is_deeper) {}


// парсинг командной строки
Arguments Arguments::parse_command(int argc, char **argv) {
    // проверка на количество
    if (argc > 5) {
        std::cerr << "Oooops...you have too many args. Seriously???\n";
        return {};
    }

    // хранение значений аргументов
    int threads = 1;                                        // по-умолчанию, если -t# не будет в аргументах
    bool is_deeper = true;                                  // по-умолчанию, если -n не будет в аргументах
    std::string root = std::filesystem::current_path().root_name();
    std::string pattern = std::string("");

    // распределяем значения
    // не рассматриваем 0-й аргумент - ./psearch
    // -t# и -n произвольное расположение - проверка в первую очередь
    // порядок ./psearch FILE DIR
    for (size_t i = 1; i < argc; ++i) {
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
        if (curr_arg.empty()) {
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
        return {};
    }

    // проверка директории на существование
    if (!root.empty()) {
        if (!std::filesystem::exists(root)) {
            std::cerr << "Directory not exists\n";
            return {};
        }
    }

    return {threads, root, pattern, is_deeper};
}
