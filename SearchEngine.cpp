#include <deque>
#include <filesystem>
#include <dirent.h>
#include <thread>
#include <algorithm>

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
    std::vector<std::pair<size_t, size_t>> threads_work = search_engine.take_bolds(arguments.threads); // границы каждого потока
    for (size_t i = 0; i < arguments.threads; ++i) {
        our_threads[i] = std::thread(thread_working, search_engine, threads_work[i].first, threads_work[i].second);
    }

    // убеждаемся, что потоки завершили работу
    for(auto& thread: our_threads) thread.join();

    return OK;
}


SearchEngine::SearchEngine(const Arguments &arguments) {
    all_files = getAllFiles(arguments.root, arguments.is_deeper);
}


std::vector<std::pair<size_t, size_t>> SearchEngine::take_bolds(int threads) {
    std::vector<std::pair<size_t, size_t>> threads_se(threads);             // старт - конец номеров считывания файлов потока

    size_t sum = 0;                                                         // подсчет размера всех файлов
    for (const auto& file_pair: all_files) sum += file_pair.second;
    size_t avg = sum / threads;                                             // узнаем среднее значение

    size_t file_i = 0;                                                      // индекс текущего файла
    for (size_t thread_i = 0; ; thread_i < threads - 1; ++thread_i) {
        threads_se[thread_i].first = file_i;                                // начало рассмотрения текущего потока

        size_t curr_size = 0;                                               // размер файлов, обрабатываемых потоком
        for (; curr_size + all_files[file_i].second <= avg && file_i < all_files.size(); ++file_i) {
            curr_size += all_files[file_i].second;
        }

        threads_se[thread_i].second = file_i;                               // индекс конца, нерассматриваемый потоком
    }

    // последний поток обрабатываем отдельно -> отдаем на обработку оставшиееся файлы
    threads_se[threads - 1] = std::make_pair(file_i, all_files.size());

    return threads_se;
}


void SearchEngine::thread_working(const SearchEngine& se, size_t begin, size_t end) {
    std::vector<FindPattern> all_results;                                   // найденные объекты c файлов от begin до end

    std::vector<FindPattern> curr_results;                                  // найденные объекты в рассматриваемом файле
    for (size_t curr = begin; curr < end; ++curr) {
        curr_results = SearchInFile(se.all_files[curr].first);              // поиск в текущем файле
        all_results.insert(all_results.end(), curr_results.begin(), curr_results.end());
    }

    // выводим все найденные объекты FindPattern
    for(const auto& elem: all_results) {
        std::cout << elem;
    }
}


std::vector<FindPattern> SearchEngine::SearchInFile(const std::string& fileName) {
    return std::vector<FindPattern>();
}

std::vector<std::pair<std::string, size_t>> SearchEngine::getAllFiles(const std::string & root, bool is_deeper) {
    std::vector<std::pair<std::string, size_t>> all_files;                  // результат
    std::deque<std::string> dirs;                                           // заводим очередь с папк-ой / -ами

    dirs.push_back(root);
    for (; !dirs.empty();) {
        std::string curr_dir_name = dirs.front();                           // имя текущей рассматриваемой папки
        dirs.pop_front();

        DIR* curr_dir = opendir(curr_dir_name.c_str());                     // открываем текущую папку
        if (!curr_dir) continue;                                            // не получилось открыть по каким-либо причинам

        // в случае рассмотрения всех вложенных папок
        if (is_deeper) {
            for (dirent* dir_elem = readdir(curr_dir); dir_elem; dir_elem = readdir(curr_dir)) {
                std::string dir_elem_name = curr_dir_name + "/" + dir_elem->d_name;         // имя текущего элемента директории
                if (dir_elem->d_type == DT_DIR) dirs.emplace_back(curr_dir_name);           // если элемент - папка
                else if (dir_elem->d_type == DT_REG) {                                      // если элемент - обычный файл
                    all_files.emplace_back(curr_dir_name, std::filesystem::file_size(curr_dir_name));
                }
            }
        }
        else {
            for (dirent* dir_elem = readdir(curr_dir); dir_elem; dir_elem = readdir(curr_dir)) {
                std::string dir_elem_name = curr_dir_name + "/" + dir_elem->d_name;         // имя текущего элемента директории
                if (dir_elem->d_type == DT_REG) {                                           // если элемент - обычный файл
                    all_files.emplace_back(curr_dir_name, std::filesystem::file_size(curr_dir_name));
                }
            }
        }

        closedir(curr_dir);
    }

    std::sort(all_files.begin(), all_files.end(), FilesCmp);                                // сортируем по-возрастанию значения
    return all_files;
}

