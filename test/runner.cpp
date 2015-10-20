#include "../include/LRU.hpp"
#include "../include/ALRU.hpp"

#include <iostream>
#include <exception>

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cerr << "Not enough parameters!\nUSAGE: " << argv[0] << " request_file" << std::endl;
        return -1;
    }

    const std::string fname(argv[1]);

    try {
        std::cout << "INIT LRU" << std::endl;
        LRU lru_test;

        std::cout << "RUNNING LRU" << std::endl;
        uint32_t total_faults = lru_test(fname);

        std::cout << "LRU COMPLETE, " << total_faults << " TOTAL PAGE FAULTS." << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Something broke! " << e.what() << std::endl;
    }


    try {
        std::cout << "INIT ALRU" << std::endl;
        ALRU alru_test;

        std::cout << "RUNNING ALRU" << std::endl;
        uint32_t total_faults = alru_test(fname);

        std::cout << "ALRU COMPLETE, " << total_faults << " TOTAL PAGE FAULTS." << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Something broke! " << e.what() << std::endl;
    }

    return 0;
}
