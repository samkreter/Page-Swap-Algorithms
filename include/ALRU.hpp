#ifndef PAGE_SWAP_ALRU_HPP__
#define PAGE_SWAP_ALRU_HPP__

#include "page_swap.hpp"


class ALRU : private page_swap_algorithm {
public:
    size_t operator()(const std::string &fname);
};

#endif
