#include "../include/ALRU.hpp"


size_t ALRU::operator()(const std::string &fname) {
    if(!fname.empty()){
        //get the page requests from the file
        auto pageRequests = read_page_requests(fname);

        //set the virtual clock to track the time
        size_t Vclock = 0;

        for(request : pageRequests){
            //check if the page requested is in the current frames
            auto framePos = std::find(frame_table.begin(),frame_table.end()[request](const frame_entry& frame){
                return frame.page_table_idx == request;
            });

            //check if theres a page fault
            if(framePos == frame_table.end()){

            }
            else{
                //set most significan bit of the access bit
                (*framePos).access_bit = 0x80;
            }
        }
    }

    throw std::runtime_error("File name not provided in ALRU");
}
