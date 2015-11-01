#include "../include/ALRU.hpp"


size_t ALRU::operator()(const std::string &fname) {
    if(!fname.empty()){
        //get the page requests from the file
        auto pageRequests = read_page_requests(fname);

        //set the virtual clock to track the time
        size_t Vclock = 0;
        size_t faults = 0;
        size_t requests = 0;
        //int counter = 0;

        for(auto request : pageRequests){
            requests++;
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
	    		
	    		uint32_t frameToRemoveId = argMin();
	    		// if(frameToRemoveId == 0){
	    		// 	counter++;
	    		// 	if(counter > 500){
	    		// 		throw std::runtime_error("mess up");
	    		// 	}
	    			
	    		// }
	    		//get the frame to be removed
	    		auto& frame = frame_table[frameToRemoveId];
                page_table[frame.page_table_idx].valid = false;
               	
                frame.page_table_idx = request;

                //set the access_bit to shows its been referenced
                frame.access_bit = 0x80;
                //read data from backing store
                if(!read_from_back_store(frame.data, frame.page_table_idx)){
                    throw std::runtime_error("Could not read from backing store during page fault");
                }

                //update the new pages vaild and frame id
                page_table[request].frameId = frameToRemoveId;
                page_table[request].valid = true;

	    	}
	    	//icrement the clock
	    	Vclock++;
        }
        std::cout<<"Requsts were "<<requests<<std::endl;
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
	}
}

uint32_t ALRU::argMin(){
	//start with lowest at the highest value of a 8 bit int
	uint8_t lowestTrackingByte = 0xFF;
	//if all are at the same value make defualt the first index to remove
	uint32_t lowestIndex = 0;
	
	for(uint32_t i = 0; i < frame_table.size(); i++){
		if(frame_table[i].tracking_byte < lowestTrackingByte){
			lowestIndex = i;
			lowestTrackingByte = frame_table[i].tracking_byte;
		}
	}
	//std::cout<<"lowest index "<<lowestIndex<<std::endl;
	return lowestIndex;
}
