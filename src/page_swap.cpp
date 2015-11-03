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

        //the first 512 pages will be vaild and set to the frame table
        //also set up the frameId
        if(tempPageIdCount < 512){
            page.valid = true;
            page.frameId = tempPageIdCount;
        }
        else{
            page.valid = false;
        }
        //set the page table id
        page.idx = tempPageIdCount++;

        //allocate a block for each page that we have
        if(block_store_allocate(backing_store) == 0){
            //I tried adding the whon whon sound from mario for all the exceptions
            // but it broke...so i took it out
            throw std::runtime_error("Error when allocating blockstore block");
        }

        //init the block to array of @ signs using hex...
        //yea I finally learned hex to acii :)
        uint8_t buffer[frame_size] = {0x40};
        if(!write_to_back_store(buffer,page.idx)){
            throw std::runtime_error("Error while writing to backstore");
        }
    }

    //init the frame table and reading from the blockstore for the data
    for( auto& frame : frame_table){
        //initialize the pageId for the frame
        frame.page_table_idx = tempFrameIdCounter++;

        //read data from backing store into each frame's data
        if(!read_from_back_store(frame.data, frame.page_table_idx)){
            throw std::runtime_error("Could not read from backing store when initilizing frame table");
        }
        //set up the tracking bytes
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
    //param check
    if(buffer && pageId >= 0){
        //I'm just going to hard code the block size value for reading the entire block...yep best practices
        //also since i'll always be reading in the entire block, another great hardcode
        if(block_store_read(backing_store,pageId+8,buffer,frame_size,0)){
            return true;
        }
        return false;
    }
    return false;

}

bool page_swap_algorithm::write_to_back_store(uint8_t *buffer, uint32_t pageId) {
    //params check
    if(buffer && pageId >= 0){
        //have to add 8 to pageid to account for begining in the backing store
        if(block_store_write(backing_store,pageId+8,buffer,frame_size,0)){
            return true;
        }
        return false;
    }
    return false;
}


std::vector<uint32_t> page_swap_algorithm::read_page_requests(const std::string &fname) {
    //check string isn't null
    if(!fname.empty()){
        //get file descriptor for bin file
        const int fd = open(fname.c_str(),O_RDONLY);
        //check file was opened correctly
        if (fd >= 0) {
            //init vecotre to hold the page requests
            std::vector<uint32_t> pRequests;
            uint32_t numRequests;
            //try to read in the number of Requests in the file
            if(read(fd,&numRequests,sizeof(uint32_t)) == sizeof(uint32_t)){

                uint32_t buffer;
                for(uint32_t i = 0;i < numRequests; i++){
                    //init buffer to zero each time
                    buffer = 0;
                    if(read(fd,&buffer,sizeof(uint32_t)) != sizeof(uint32_t)){
                        throw std::runtime_error(std::string("failed to read request "));
                    }
                    //add the request from the file onto the vector
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
