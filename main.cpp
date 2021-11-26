#include "SearchEngine.h"


int main(int argc, char **argv) {
    int result = SearchEngine::start(argc, argv);                       // старт работы поисковго движка
    if (result != OK) return result;                                    // если что-то пошло не так
}
