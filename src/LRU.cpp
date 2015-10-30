#include "../include/LRU.hpp"


size_t LRU::operator()(const std::string &fname) {
	if(!fname.empty()){
	   	//get the page requests from the file
	   	auto pageRequests = read_page_requests(fname);
	    //set up the tracking list with the current frames;
	    initlruTable();
	    //set the virtual clock to track the time
	    uint Vclock = 0;
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
	    		bool valid = (*pagePos).vaild;
	    		if(vaild){
	    			uint32_t tempId = *lsuTable.begin();
	    			//get front because its the last one accessed 
	    			auto frame = std::find_if(frame_table.begin(),frame_table.end(),[tempId](const frame_entry& frame){
	    				return frame.page_table_idx == tempId;
	    			});
	    			(*frame).page_table_idx = (*pagePos).idx;
	    			//read data from backing store
	    			if(!read_from_back_store((*frame).data, (*frame).page_table_idx)){
            			throw std::runtime_error("Could not read from backing store during page fault");
        			}

        			//moved the newly updated from front to the back since it was used
        			lsuTable.splice(lsuTable.end(),lsuTable,lsuTable.begin());
	    		}

	    	}
	    	//incrememnt clock
	    	Vclock++;
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
