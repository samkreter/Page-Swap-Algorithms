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
	    	//check if request is in the frame table bases off the lru table
            //since it will be faster to search
            auto pageIndex = std::find(lsuTable.begin(),lsuTable.end(),request);

            //if there is no page fault, move the page to the end for most recently used
	    	if(pageIndex != lsuTable.end()){
	    		std::cout<<"Page Index "<<*pageIndex<<std::endl;
	    		std::cout<<"value at that index"<<lsuTable
	    		lsuTable.splice(lsuTable.end(),lsuTable,pageIndex);
	    		std::cout<<"new end of list "<<lsuTable.back()<<std::endl;
	    	}
	    	//we had a page fault
	    	else{
	    		//increment faults tacker
	    		faults++;

                //make sure that there wasn't a mistake in the indexing that sends us out of range
                if(*(lsuTable.begin()) > page_table.size() || request > page_table.size()){
                    throw std::runtime_error("Something doesn't match up in the page table");
                }
	    		auto& pageToRemove = page_table[*(lsuTable.begin())];
                pageToRemove.valid = false;
                uint32_t frameToRemoveId = pageToRemove.frameId;

                //get the frame based on the index
                if(frameToRemoveId <= frame_table.size()){
                    auto& frame = frame_table[frameToRemoveId];

                    frame.page_table_idx = request;

                    //read data from backing store
                    if(!read_from_back_store(frame.data, frame.page_table_idx)){
                        throw std::runtime_error("Could not read from backing store during page fault");
                    }

                    //update the new pages vaild and frame id
                    page_table[request].frameId = frameToRemoveId;
                    page_table[request].valid = true;

                    //moved the newly updated from front to the back since it was used
                    lsuTable.erase(lsuTable.begin());
                    lsuTable.push_back(request);

                }
                else{
                    throw std::runtime_error("frame frome page table was not in frametable");
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
