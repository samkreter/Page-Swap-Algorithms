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

	    		//get page from page table to find
	    		auto pagePos = std::find_if(page_table.begin(),page_table.end(),[request] (const page_entry& page){
	    		 return page.idx == request;
	    		});

	    		if(pagePos == page_table.end()){
	    			throw std::runtime_error("Page requested not in page table...Damn thats not good");
	    		}

	    		//had to have it out of the scope to get it later
	    		uint32_t newId;
	    		if((*pagePos).valid){
	    			uint32_t tempId = *lsuTable.begin();
	    			//get front because its the last one accessed
	    			auto frame = std::find_if(frame_table.begin(),frame_table.end(),[tempId](const frame_entry& frame){
	    				return frame.page_table_idx == tempId;
	    			});
	    			if(frame == frame_table.end()){
	    				throw std::runtime_error("Page Id not found in frame table, does not make since man");
	    			}

	    			newId = (*pagePos).idx;
	    			(*frame).page_table_idx = newId;
	    			//read data from backing store
	    			if(!read_from_back_store((*frame).data, (*frame).page_table_idx)){
            			throw std::runtime_error("Could not read from backing store during page fault");
        			}

        			//moved the newly updated from front to the back since it was used
        			lsuTable.erase(lsuTable.begin());
        			lsuTable.push_back(newId);
	    		}

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
