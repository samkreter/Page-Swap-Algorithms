#include "../include/LRU.hpp"


size_t LRU::operator()(const std::string &fname) {
	//param check
	if(!fname.empty()){

        //get the page requests from the file
	   	auto pageRequests = read_page_requests(fname);

        //set up the tracking list with the current frames;
	    initlruTable();

        //the number of page faults
	    size_t faults = 0;

	    //loop through all the requests
	    for( auto request : pageRequests){
	    	//check if request is in the frame table bases off the lru table
            //since it will be faster to search
            auto pageIndex = std::find(lsuTable.begin(),lsuTable.end(),request);

            //if there is no page fault, move the page to the end for most recently used
	    	if(pageIndex != lsuTable.end()){
	    		lsuTable.splice(lsuTable.end(),lsuTable,pageIndex);
	    	}
	    	//we had a page fault
	    	else{
	    		//increment faults tacker
	    		faults++;

                //make sure that there wasn't a mistake in the indexing that sends us out of range
                if(*(lsuTable.begin()) > page_table.size() || request > page_table.size()){
                    throw std::runtime_error("Something doesn't match up in the page table");
                }

                //get a reference to the page who's frame will be removed
	    		auto& pageToRemove = page_table[*(lsuTable.begin())];
                
                //set its valid bit to false
                pageToRemove.valid = false;
                //get it's frame id
                uint32_t frameToRemoveId = pageToRemove.frameId;

                //check just to make sure no indexing mistakes
                if(frameToRemoveId <= frame_table.size()){
                    //get the frame to replace the data
                    auto& frame = frame_table[frameToRemoveId];
                    
                    //write the data from the frame back to the backing store
                    if(!write_to_back_store(frame.data,frame.page_table_idx)){
            			throw std::runtime_error("Error while writing to backstore");
       				}

       				//set the new frame's page id
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
