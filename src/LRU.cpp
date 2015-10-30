#include "../include/LRU.hpp"


size_t LRU::operator()(const std::string &fname) {
	if(!fname.empty()){
	   	//get the page requests from the file
	   	auto pageRequests = read_page_requests(fname);
	    //set up the tracking list with the current frames;
	    initlruTable();
	    //the number of page faults
	    size_t faults = 0;

	    for( auto request : pageRequests){
	    	auto pageIndex = std::find(lsuTable.begin(),lsuTable.end(),request);
	    	// //we find it so no page fault, move to most recently used
	    	if(pageIndex != lsuTable.end()){
	    		lsuTable.splice(lsuTable.end(),lsuTable,pageIndex);
	    	}
	    	//we had a page fault
	    	else{
	    		//increment faults tacker
	    		faults++;

                handlePageFault([](request,lsuTable){
                    //moved the newly updated from front to the back since it was used
                    lsuTable->erase(lsuTable.begin());
                    lsuTable->push_back(newId);
                });

	    	}
	    }


 		return faults;
	}
	throw std::runtime_error("Problem with filename in LRU");
}

void LRU::initlruTable(){
    //add all the current frames to the "stack" pull from back with page fault
    for(auto frame : frame_table){
    	lsuTable.push_front(frame.page_table_idx);
    }
}



