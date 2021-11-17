#include "SearchEngine.h"


int SearchEngine::start(int argc, char **argv) {
    Arguments args = Arguments::parse_command(argc, argv);
    // проверка на получение непустого pattern-а
    if (args.pattern.empty()) {
        return INVALID_ARGUMENTS;
    }




    return OK;
}


void SearchEngine::thread_working(size_t begin, size_t end) {

}


std::vector<std::pair<size_t, size_t>> SearchEngine::take_bolds() {
    return std::vector<std::pair<size_t, size_t>>();
}


std::vector<FindPattern> SearchEngine::SearchInFile() {
    return std::vector<FindPattern>();
}



