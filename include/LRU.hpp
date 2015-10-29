#ifndef PAGE_SWAP_LRU_HPP__
#define PAGE_SWAP_LRU_HPP__

#include "page_swap.hpp"


class LRU : private page_swap_algorithm {
private:
    //stores the least recently used frames
	vector<uint32_t> lsuTable;
public:
    size_t operator()(const std::string &fname);
};

#endif
