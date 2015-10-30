#ifndef PAGE_SWAP_LRU_HPP__
#define PAGE_SWAP_LRU_HPP__

#include "page_swap.hpp"


class LRU : private page_swap_algorithm {
private:
    //stores the least recently used frame's page_table_idx
    //lists have same search for element at vector but o(1) insert so I choose it
	std::list<uint32_t> lsuTable;
public:
    size_t operator()(const std::string &fname);
    void initlruTable();
};

#endif
