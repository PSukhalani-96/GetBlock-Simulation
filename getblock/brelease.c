#include "bufferCache.h"

/*------------------------------------------Brelse-Algorithm-Implementation----------------------------------------*/
void display_HQ() {
	display_hash_queue(0);
	display_hash_queue(1);
	display_hash_queue(2);
	display_hash_queue(3);
}

void brelse(int processNo) {

	/*
		Objective:		To release a buffer after a process cureently locking it, unlocks it.
		Return Type:	void(null)
		Parameter:		int processNo
							specifies the process which is unlocking its acquired buffer.
		Approach:		perform using following above described functions:
						
						->	search_mem_process(processNo):			to search the specific mem_process(processNo) 
																	from Process queue
						
						->	delete_from_process_queue(buff): 		delete buff(searched mem_process) from Process
															 		queue
						->	find_hash_queue_no(block_num):			to get the index of hash queue
						
						->	delete_from_hash_queue(block_num,index):to delete block (block_num) from Hash-Queue[index].
						
						->	wakeup():								to randomly wakeup one of the sleeping process
						
						->	insert_at_end_of_free_list(block_num):	to insert block_num at the end of free list(mark it delayed write)
						
						->	insert_at_front_of_free_list(block_num):to insert block_num at the front of free list(incase block_num is 
																	previously marked delayed write)
						
						->	getblk(block_num):						calling getblock to get/give the block_num to chosen sleeping process.
						
						Updating the processNo accordingly.
	*/

	struct mem_process *buff = search_mem_process(processNo);
	int status;

	if (buff != NULL) {
		
		printf("\n Rising CPU execution level to block Disk Interrupts. \n");
		printf("\n Unlocking Buffer %d of Process %d. \n",buff->allocated_block->block_num, buff->pid);
		printf("\n Waking up all processes waiting for any/specific buffer to become Free. \n");
		add_to_free_list_from_delayed();
		delete_from_process_queue(buff);
		int index = find_hash_queue_no(buff->allocated_block->block_num);
	  	delete_from_hash_queue(buff->allocated_block->block_num, index);
	  	
		struct sleep_queue *temp1 = wakeup_specificBuffer(buff->allocated_block->block_num);
		struct sleep_queue *temp2 = wakeup_anyBuffer();

		srand(time(NULL)); 
	    int no = (rand()%2);

	    if (buff->allocated_block->old == false) {

	    	printf("\n Released Buffer is not old. Hence, added at the end of free-List.");
	    	if (no == 0)
	    		insert_at_end_of_free_list(buff->allocated_block->block_num, true, false);
	    	else {
	    		printf("\n Process %d has changed the contents of released block %d.",buff->pid, buff->allocated_block->block_num );
				printf("\n Hence, Buffer with block-no %d is marked Delayed-Write.", buff->allocated_block->block_num);
	    		
	    		insert_at_end_of_free_list(buff->allocated_block->block_num, true, true);
	    	}

	    }
	    else {

	    	printf("\n Released Buffer is not old. Hence, added at the front of freeree-List.");

			if (no == 0)
				insert_at_begin_of_free_list(buff->allocated_block->block_num, true, false);
			else {
				printf("\n Process %d has changed the contents of released block %d.",buff->pid, buff->allocated_block->block_num );
				printf("\n Hence, Buffer with block-no %d is marked Delayed-Write.", buff->allocated_block->block_num);
				
				insert_at_begin_of_free_list(buff->allocated_block->block_num, true, true);		
			}
	    }

	  	if(temp1 != NULL) {
	  		printf("\n Woke-Up Process-identifier: %d", temp1->Process_no);
	  		printf("\n Trying to Fulfill Request of Woken-Up process...");
	  		printf("\n ");
	  		process = temp1->Process_no;
	  		status = getblk(temp1->block_no);
	  		printf("\n The status of Hash Queue is as follows \n:");
			display_HQ();
	  		printf("\n");
	  		sleep(1);
	  		if ( (status == 1 || status == 2 || status == 3) && temp2 != NULL) {
	  			printf("\n Woke-Up Process-identifier: %d", temp2->Process_no);
	  			printf("\n Trying to Fulfill Request of Woken-Up process...");
	  			printf("\n ");
	  			process = temp2->Process_no;
	  			status = getblk(temp2->block_no);
	  			printf("\n The status of Hash Queue is as follows \n:");
				display_HQ();
	  			sleep(1);
	  			process = processNo;
	  		}
	  		else {
	  			if (temp2 != NULL)
	  				insert_at_end_of_SQ(temp2->Process_no,temp2->block_no);
	  		}
	  		sleep(1);
	  		process = processNo;
		}
	  	else if (temp2 != NULL) {
	  		printf("\n Woke-Up Process-identifier: %d", temp2->Process_no);
	  		printf("\n Trying to Fulfill Request of Woken-Up process...");
	  		printf("\n ");
	  		process = temp2->Process_no;
	  		status = getblk(temp2->block_no);	
	  		printf("\n The status of Hash Queue is as follows \n:");
			display_HQ();
	  		sleep(1);
	  		process = processNo;
		}
	  	else {

	  		printf("\n No Process is currently waiting in Sleeping-Queue for any-Buffer.");
	  		printf("\n ");
	  		process = processNo;
	  	}
  	}
}
/*-------------------------------------------------------------------------------------------------------------------*/