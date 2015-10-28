#include "../include/page_swap.hpp"

page_swap_algorithm::page_swap_algorithm() {
    //create the block store
    init_backing_store();
    //set temp vars, couldve done this with one but you know it looks more cool with 
    //long var names
    uint32_t tempFrameIdCounter = 0;
    uint32_t tempPageIdCount = 0;

    //init the page table setting the id and setting vaild to true
    for( auto& page : page_table){
        page.idx = tempPageIdCount++;
        page.valid = true;
    }

    //init the frame table and reading from the blockstore for the data 
    for( auto& frame : frame_table){
        frame.page_table_idx = tempFrameIdCounter++;
        read_from_back_store(frame.data, frame.page_table_idx);
        frame.tracking_byte = 0;
        frame.access_bit = 0;
    }
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


bool page_swap_algorithm::read_from_back_store(uint8_t *buffer, uint32_t pageId) {
    //I'm just going to hard code the block size value for reading the entire block...yep best practices
    //also since i'll always be reading in the entire block, another great hardcode
    if(block_store_read(backing_store,pageId,buffer,1024,0)){
        return true;
    }
    return false;

}

bool page_swap_algorithm::write_to_back_store(/*???? TODO: add your parameters*/) {
    throw std::runtime_error("write_to_back_store INCOMEPLETE");
}


std::vector<uint32_t> page_swap_algorithm::read_page_requests(const std::string &fname) {
    if(!fname.empty()){

        const int fd = open(fname.c_str(),O_RDONLY);

        if (fd >= 0) {
            
            std::vector<uint32_t> pRequests;
            uint32_t numBlocks;
            
            if(read(fd,&numBlocks,sizeof(uint32_t)) == sizeof(uint32_t)){
                
                uint32_t* buffer = (uint32_t*)malloc(sizeof(uint32_t));
                for(uint32_t i = 0;i < numBlocks; i++){
                    *buffer = 0;
                    if(read(fd,buffer,sizeof(uint32_t)) != sizeof(uint32_t)){
                        throw std::runtime_error(std::string("failed to read request "));
                    }
                    pRequests.push_back(*buffer);
                }
                int counter = 0;
                for ( auto test : pRequests){
                    std::cout<<test<<" ";
                    if(counter > 10){
                        break;
                    }
                    counter++;
                }

                return pRequests;
                
            }
            throw std::runtime_error("could not read number of blocks");
        }
        throw std::runtime_error("could not open file");
    }
    throw std::runtime_error("Bad Params bro");
}
