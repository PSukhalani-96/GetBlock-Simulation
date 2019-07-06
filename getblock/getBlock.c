#include "bufferCache.h"

/*---------------------------------------------GetBlock-Implementation-----------------------------------------*/
int getblk(int block_num) {
		
	/*
		Objective:		To get the block block_num.
		Return Type:	Integer
							returns the different status depending on different scenarios.
							
							status 1:	Buffer is both in Hash Queue and Free List.
										(Sucessfully Allocated)
							
							status 2:	Free list is not empty.(Block will be allocated from
										Free-List ).
							
							status 3:	Buffer is in Hash Queue...but it is busy.(Process enters 
										in sleep queue ).
							
							status 4:	Free List is empty.Process will insert into sleeping queue.
							recursive call to getblock(block_num) if a current buffer selected is in 
							delayed write mode.
		Parameter:				
			block_num:		specifies the block number which is required by the process.
		
		Approach:			perform getblock using following functions:
							->	find_hash_queue_no(block_num):			To get the index of hash queue(where block_num 
																	 	is to be searched/inserted).
							
							->	search_block_num(index, block_num):		To search the block(block_num) in hash-queue[index].
							
							->	add_to_free_list_from_delayed():		At each iteration of getblk(), program is checking for
																	 	delayed write buffers which have completed their asynchronous 
																	 	writing(timeout), to add them in the free list.
							
							->	search_block_in_free_list(block_num):	To search the block in free list.
							
							->	insert_at_end_of_SQ(process,block_num):	To insert the process in sleeping queue with required block 
																	 	"block_num" (if block_num requirement of a process is not 
																	 	fullfilled).
							
							->	delete_from_free_list(block_num):		To remove the buffer from free list.
							
							->	insert_in_process_queue(buffer):		To insert a process with specific "block_num" in Process-Queue
																	 	(i.e, process will now enter into indefinite sleep until it 
																	 	finishes its system call execution(Unlocking a buffer)).
							
							->	insert_in_hash_queue(block_num,index):	If block_num can be allocated to a process, inserting the "block_
																	 	num"  in hash_queue[index].
	
							->	insert_in_delayed_write_queue(temp): 	If buffer "temp" removed from Free-list is marked delayed-write,
																	 	insert "temp" in delayed_write queue.

							Scenario-1:		Block is in hash queue and free list.(Block allocated to process).
							
							Scenario-2:		Found a Free Buffer(which can be alloctated to a process, by updating its "block_num" and 
											adjusting it in proper hash queue).
							
							Scenario-3:		Found a free buffer in free list but it is marked delayed_write.(Delete this buffer from 
											free-list and start its asynchronous writing(insert at end of delayed_write queue) and then 
											choose another free buffer from free-list to fullfill the request, else insert the process at
											end of sleeping-queue).		
							
							Scenario-4:		Block is not presnt in Hash-Queue and Free List is empty.Process corresponding to its "block_num"
											is inserted into sleeping queue waiting for any buffer to become free.

							Scenario-5:		Block is present in Hash Queue but it is busy. Process corresponding to this "block_num" is inserted
											into sleeping queue waiting for specific buffer to become free.

	*/
	
	int index = find_hash_queue_no(block_num);
	struct buffer *buff = search_block_num(index, block_num);
	add_to_free_list_from_delayed();
	struct buffer *temp_buff = search_block_in_free_list(block_num);

	display_free_list();

	if ( buff != NULL  || (temp_buff != NULL && temp_buff->hq_present) ) {
		/* Scenario - 5 */
		if (buff!= NULL && buff->busy) {
			printf("\n Buffer is in Hash Queue...but it is BUSY.");
			printf("\n");
			insert_at_end_of_SQ_specificBuffer(process, block_num);
			sleep(2);
			return 3;
	  	}
	  	
	  	/* Scenario - 1	*/
	  	printf("\n Buffer is in Hash Queue and Free List.");
	  	printf("\n");
	  	delete_from_free_list(temp_buff->block_num);
	  	insert_in_process_queue(temp_buff);
	  	insert_in_hash_queue(block_num, index);
	  	sleep(2);
	  	return 1;
	}

	else {

	  	if ( fheader == NULL ) {
	  		/* Scenario - 4*/
			printf("\n Free List is Empty (Process enters into sleep queue.)!!!");
			insert_at_end_of_SQ(process, block_num);
			printf("\n");
			sleep(2);
			return 4;
	  	}

	  	struct buffer *temp = delete();
	  
		if (temp->delayed_write) {
	  		/* Scenario - 3 */
			printf("\n To fullfill this request: ");
			printf("\n Buffer Selected(containing block-no: %d) is in DELAYED WRITE Mode.", temp->block_num);
			printf("\n Select another Buffer From Free-List.");
			printf("\n");	
			delayed_write_processing(temp);
			insert_in_delayed_write_queue(temp);
			sleep(2);
			return getblk(block_num);
	  	} 

	  	/* Scenario - 2	*/
	  	printf("\n Free List is non-Empty.");
	  	printf("\n A Free-Buffer is successfully allocated to the Process.");
	  	printf("\n");
	  	temp->block_num = block_num;
	  	temp->hq_present = false;
	  	insert_in_process_queue(temp);
	  	insert_in_hash_queue(block_num, index);
	  	sleep(2);
	  	return 2;
	}

}
/*-------------------------------------------------------------------------------------------------------------*/