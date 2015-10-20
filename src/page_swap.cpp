#include "../include/page_swap.hpp"

page_swap_algorithm::page_swap_algorithm() {
    init_backing_store();

    // TODO: initialize and fill frame/page tables
}

void page_swap_algorithm::init_backing_store() {
    block_store = block_store_create();
    if (block_store)
        return;
    throw std::bad_alloc("Could not create block_store because ".append(block_store_strerror(bs_errno))); // not sure I can mix "" and . directly
}

page_swap_algorithm::~page_swap_algorithm() {
    block_store_destroy(block_store, BS_NO_FLUSH);
}


void page_swap_algorithm::fault_printer(const uint32_t request, const uint32_t frame_replaced, const uint32_t page_replaced) {
    std::cout << "PAGE FAULT: PAGE REQUESTED: " << request << " FRAME REPLACED: " << frame_replaced << " PAGE REPLACED: " << page_replaced << std::endl;
}


bool page_swap_algorithm::read_from_back_store(/*?????? TODO: add your parameters*/) {
    return false;
}

bool page_swap_algorithm::write_to_back_store(/*???? TODO: add your parameters*/) {
    return false;
}


std::vector<uint32_t> page_swap_algorithm::read_page_requests(const std::string &fname) {
    return std::vector<uint32_t>();
}
