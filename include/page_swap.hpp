#ifndef PAGE_SWAP_HPP__
#define PAGE_SWAP_HPP__

#include <cstdint>
#include <vector>
//#include <list> // ?
//#include <unistd.h>
//#include <fcntl.h>
#include <fstream>
#include <array> // This is actually the first time I've every used this
// Missing things?
#include <exception>
#include <stdexcept>
#include <iostream>

extern "C" { // C library, no name mangling
#include <block_store.h>
}


class page_swap_algorithm {
private:
    void init_backing_store();
protected:

    static const uint32_t page_entry_total = 2048;
    static const uint32_t frame_entry_total = 512;
    static const uint32_t frame_size = 1024;

    using page_entry = struct {
       uint32_t idx;
       bool valid;
    };

    using frame_entry = struct {
        uint32_t page_table_idx;
        uint8_t data[frame_size];
        uint8_t tracking_byte;
        uint8_t access_bit;
    };

    using page_algorithm_results = struct {
        uint32_t page_requested;
        uint32_t frame_replaced;
        uint32_t page_replaced;
    };

    std::array<page_entry, page_entry_total> page_table;
    std::array<frame_entry, frame_entry_total> frame_table;
    block_store_t *backing_store;

    void fault_printer(const uint32_t request,const uint32_t frame_replaced,const uint32_t page_replaced);

    bool read_from_back_store(uint8_t* buffer, uint32_t pageId);
    bool write_to_back_store(/* ??? */);
    std::vector<uint32_t> read_page_requests(const std::string &fname);

public:
    page_swap_algorithm(); // do your frame and page table init here
    ~page_swap_algorithm();
    virtual size_t operator()(const std::string &fname) = 0;
};

#endif
