#include "../include/LRU.hpp"


size_t LRU::operator()(const std::string &fname) {
   	auto pageRequests = read_page_requests(fname);
    initlruTable();
 	return 0;
}

void LRU::initlruTable(){
    lruTable(frame_table.begin(),frame_table.end());
}
