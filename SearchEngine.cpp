#include <deque>
#include <filesystem>
#include <dirent.h>
#include <thread>
#include <algorithm>
#include <fstream>
#include <utility>


#include "SearchEngine.h"


/*
 * Add Functions
 */

bool FilesCmp(std::pair<std::string, size_t>& file1, std::pair<std::string, size_t>& file2) {
    return file1.second > file2.second;
}


/*
 * Main Functions
 */


int SearchEngine::start(int argc, char **argv) {
    Arguments arguments = Arguments::parse_command(argc, argv);
    // проверка на получение непустого pattern-а
    if (arguments.pattern.empty()) {
        return INVALID_ARGUMENTS;
    }

    // создание текущего поисквого движка
    SearchEngine search_engine = SearchEngine(arguments);

    // вектор для хранения наших потоков
    std::vector<std::thread> our_threads(arguments.threads);

    // распределяем работу
    std::vector<std::pair<size_t, size_t>> threads_work = search_engine.take_bolds(); // границы каждого потока
    for (size_t i = 0; i < arguments.threads; ++i) {
        our_threads[i] = std::thread(thread_working, search_engine, threads_work[i].first, threads_work[i].second);
    }

    // убеждаемся, что потоки завершили работу
    for(auto& thread: our_threads) thread.join();

    return OK;
}


SearchEngine::SearchEngine(Arguments arguments) :
    arguments(std::move(arguments)) {
    all_files = getAllFiles();
}


std::vector<std::pair<size_t, size_t>> SearchEngine::take_bolds() {
    std::vector<std::pair<size_t, size_t>> threads_se(arguments.threads);             // старт - конец номеров считывания файлов потока

    size_t sum = 0;                                                         // подсчет размера всех файлов
    for (const auto& file_pair: all_files) sum += file_pair.second;
    size_t avg = sum / arguments.threads;                                             // узнаем среднее значение

    size_t file_i = 0;                                                      // индекс текущего файла
    size_t thread_i = 0;
    for (; thread_i < arguments.threads - 1; ++thread_i) {
        threads_se[thread_i].first = file_i;                                // начало рассмотрения текущего потока

        size_t curr_size = 0;                                               // размер файлов, обрабатываемых потоком
        for (; curr_size + all_files[file_i].second <= avg && file_i < all_files.size(); ++file_i) {
            curr_size += all_files[file_i].second;
        }

        threads_se[thread_i].second = file_i;                               // индекс конца, нерассматриваемый потоком
    }

    // последний поток обрабатываем отдельно -> отдаем на обработку оставшиееся файлы
    threads_se[arguments.threads - 1] = std::make_pair(file_i, all_files.size());

    return threads_se;
}


void SearchEngine::thread_working(const SearchEngine& se, size_t begin, size_t end) {
    std::vector<FindPattern> all_results;                                   // найденные объекты c файлов от begin до end

    std::vector<FindPattern> curr_results;                                  // найденные объекты в рассматриваемом файле
    for (size_t curr = begin; curr < end; ++curr) {
        curr_results = SearchInFile(se.all_files[curr].first, se.arguments.pattern);  // поиск в текущем файле
        all_results.insert(all_results.end(), curr_results.begin(), curr_results.end());
    }

    // выводим все найденные объекты FindPattern
    for(const auto& elem: all_results) {
        std::cout << elem;
    }
}


std::vector<FindPattern> SearchEngine::SearchInFile(const std::string& fileName, const std::string& patt) {
    std::vector<FindPattern> curr_result;                                   // найденный pattern-ы в текущем файле

    std::string curr_line;                                                  // текущая строка
    std::ifstream file(fileName);

    for (size_t line_num = 0; getline(file, curr_line); ++line_num) {
        std::string curr_prefix_line = patt + "#";
        curr_prefix_line.append(curr_line);
        std::vector<int> prefix_result = prefixFunction(curr_prefix_line);

        for (size_t i = patt.length() + 1; i < prefix_result.size(); ++i) {
            if (prefix_result[i] == patt.length()) {
                curr_result.emplace_back(fileName, line_num, curr_line);
            }
        }
    }
    file.close();

    return curr_result;
}

std::vector<std::pair<std::string, size_t>> SearchEngine::getAllFiles() {
    std::vector<std::pair<std::string, size_t>> all_files_;                 // результат
    std::deque<std::string> dirs;                                           // заводим очередь с папк-ой / -ами

    dirs.push_back(arguments.root);
    for (; !dirs.empty();) {
        std::string curr_dir_name = dirs.front();                           // имя текущей рассматриваемой папки
        dirs.pop_front();

        DIR* curr_dir = opendir(curr_dir_name.c_str());                     // открываем текущую папку
        if (!curr_dir) continue;                                            // не получилось открыть по каким-либо причинам

        // в случае рассмотрения всех вложенных папок
        if (arguments.is_deeper) {
            for (dirent* dir_elem = readdir(curr_dir); dir_elem; dir_elem = readdir(curr_dir)) {
                std::string dir_elem_name = curr_dir_name + "/" + dir_elem->d_name;         // имя текущего элемента директории
                if (dir_elem->d_type == DT_DIR) dirs.emplace_back(curr_dir_name);           // если элемент - папка
                else if (dir_elem->d_type == DT_REG) {                                      // если элемент - обычный файл
                    all_files_.emplace_back(curr_dir_name, std::filesystem::file_size(curr_dir_name));
                }
            }
        }
        else {
            for (dirent* dir_elem = readdir(curr_dir); dir_elem; dir_elem = readdir(curr_dir)) {
                std::string dir_elem_name = curr_dir_name + "/" + dir_elem->d_name;         // имя текущего элемента директории
                if (dir_elem->d_type == DT_REG) {                                           // если элемент - обычный файл
                    all_files_.emplace_back(curr_dir_name, std::filesystem::file_size(curr_dir_name));
                }
            }
        }

        closedir(curr_dir);
    }

    std::sort(all_files_.begin(), all_files_.end(), FilesCmp);                                // сортируем по-возрастанию значения
    return all_files_;
}

std::vector<int> SearchEngine::prefixFunction(const std::string &pr_line) {
    size_t len = pr_line.length();
    std::vector<int> pi(len);                                                                   // результат префикс-функции
    for (int i = 1; i < len; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pr_line[i] != pr_line[j]) j = pi[j - 1];
        if (pr_line[i] == pr_line[j]) ++j;
        pi[i] = j;
    }

    return pi;
}

