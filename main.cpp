#include "SearchEngine.h"


int main(int argc, char **argv) {
    int result = SearchEngine::start(argc, argv);
    if (result != OK) return result;
}
