#ifndef PAGE_SWAP_LRU_HPP__
#define PAGE_SWAP_LRU_HPP__

#include "page_swap.hpp"


class LRU : private page_swap_algorithm {
private: 
	vector< pageRequests;
public:
    size_t operator()(const std::string &fname);
};

#endif
