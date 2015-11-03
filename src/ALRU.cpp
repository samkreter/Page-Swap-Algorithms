#include "../include/ALRU.hpp"


size_t ALRU::operator()(const std::string &fname) {
    //params check
    if(!fname.empty()){
        //get the page requests from the file
        auto pageRequests = read_page_requests(fname);

        //set the virtual clock to track the time
        size_t Vclock = 0;
        //init the faults counter
        size_t faults = 0;

        //loop through all the requests
        for(auto request : pageRequests){
            //check if we need to adjust the tracking bits on time interval
            if(Vclock % 100 == 0){
            	adjustTrackingBit();
            }
            
            //check if the page requested is in the current frames
            auto framePos = std::find_if(frame_table.begin(),frame_table.end(),[request](const frame_entry& frame){
                return frame.page_table_idx == request;
            });

            //check if theres a page fault
            if(framePos != frame_table.end()){
            	//set most significan bit of the access bit
                (*framePos).access_bit = 0x80;

            }
            else{

                //increment faults tacker
	    		faults++;

	    		//set up results struct and set request
	    		page_algorithm_results pResults;
	    		pResults.page_requested = request;

	    		//get the frame with the lowest access tracking bit
	    		uint32_t frameToRemoveId = argMin();

	    		//set frame to remove 
	    		pResults.frame_replaced = frameToRemoveId;
	    		
	    		//make sure no indexing mistakes that could crash the program 
	    		if(frameToRemoveId > frame_table.size()){
	    			throw std::runtime_error("Frame Id out of range");
	    		}
	    		//get the frame to be removed
	    		auto& frame = frame_table[frameToRemoveId];

                
	    		//set page to remove
	    		pResults.page_replaced = frame.page_table_idx;

                //another index checking, probably unnesseary but helps me sleep...I really like sleep
	    		if(frame.page_table_idx > page_table.size()){
	    			throw std::runtime_error("Page Id out of range");
	    		}

	    		//set page valid page to false
                page_table[frame.page_table_idx].valid = false;

                //write the data from the frame back to the backing store
                if(!write_to_back_store(frame.data,frame.page_table_idx)){
            		throw std::runtime_error("Error while writing to backstore");
       			}

       			//set new page id
                frame.page_table_idx = request;

                //set the access_bit to shows its been referenced
                frame.access_bit = 0x80;
                
                //read data from backing store of new page data
                if(!read_from_back_store(frame.data, frame.page_table_idx)){
                    throw std::runtime_error("Could not read from backing store during page fault");
                }

                //update the new pages vaild and frame id
                page_table[request].frameId = frameToRemoveId;
                page_table[request].valid = true;

                //print out the fault information
                fault_printer(pResults.page_requested,pResults.frame_replaced,pResults.page_replaced);
	    	}
	    	//icrement the clock
	    	Vclock++;
        }
        return faults;
     }

    throw std::runtime_error("File name not provided in ALRU");
}

void ALRU::adjustTrackingBit(){
	for(auto& frame : frame_table){
		
		//shift the tracking byte over by 1
		frame.tracking_byte >>= 1;
		
		//mask the tracking byte with the access bit
		frame.tracking_byte |= frame.access_bit;

		//set access bit back to zero
		frame.access_bit = 0;
	}
}

uint32_t ALRU::argMin(){
	//start with lowest at the highest value of a 8 bit int
	uint8_t lowestTrackingByte = 0xFF;
	
	//if all are at the same value make defualt the first index to remove
	uint32_t lowestIndex = 0;
	
	//looked into std::min_element but didn't want to mess withit
	for(uint32_t i = 0; i < frame_table.size(); i++){
		//just checkout to tfind lowest value
		if(frame_table[i].tracking_byte < lowestTrackingByte){
			lowestIndex = i;
			lowestTrackingByte = frame_table[i].tracking_byte;
		}
	}

	return lowestIndex;
}
