#include "SearchEngine.h"


int main(int argc, char **argv) {
    int code = SearchEngine::start(argc, argv);
    if (code != OK) return code;
}