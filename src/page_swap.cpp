#include "../include/page_swap.hpp"

page_swap_algorithm::page_swap_algorithm() {
    init_backing_store();
    
    // TODO: initialize and fill frame/page tables
    throw std::runtime_error("constructor INCOMEPLETE");
}

void page_swap_algorithm::init_backing_store() {
    backing_store = block_store_create();
    if (backing_store)
        return;
    throw std::runtime_error(std::string("Could not create block_store because ").append(block_store_strerror(bs_errno)));
}

page_swap_algorithm::~page_swap_algorithm() {
    block_store_destroy(backing_store, BS_NO_FLUSH);
}


void page_swap_algorithm::fault_printer(const uint32_t request, const uint32_t frame_replaced, const uint32_t page_replaced) {
    std::cout << "PAGE FAULT: PAGE REQUESTED: " << request << " FRAME REPLACED: " << frame_replaced << " PAGE REPLACED: " << page_replaced << std::endl;
}


bool page_swap_algorithm::read_from_back_store(/*?????? TODO: add your parameters*/) {
    throw std::runtime_error("read_from_back_store INCOMEPLETE");
}

bool page_swap_algorithm::write_to_back_store(/*???? TODO: add your parameters*/) {
    throw std::runtime_error("write_to_back_store INCOMEPLETE");
}


std::vector<uint32_t> page_swap_algorithm::read_page_requests(const std::string &fname) {
    if(fname){

        const int fd = open(fname,O_RDONLY);

        if (fd >= 0) {
            
            std::vector<uint32_t> pRequests;
            uint32_t numBlocks;
            
            if(read(fd,&numBlocks,sizeof(uint32_t)) == sizeof(uint32_t)){
                
                for(int i = 0;i < numBlocks; i++){
                    
                    uint32_t* buffer = (uint32_t*)malloc(sizeof(uint32_t));
                    
                    if(read(fd,buffer,sizeof(uint32_t)) != sizeof(uint32_t)){
                        throw std::runtime_error(std::string("failed to read request ").append(i))
                    }
                    pRequests.push_back(buffer);
                }
                return pRequests;
                
            }
            throw std::runtime_error("could not read number of blocks");
        }
        throw std::runtime_error("could not open file");
    }
    throw std::runtime_error("Bad Params bro");
}
