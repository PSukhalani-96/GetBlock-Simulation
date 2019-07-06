#include "bufferCache.h"

struct buffer *fheader = NULL;

/*--------------------------------------------Free-List-Implementation-----------------------------------------*/
void insert_at_end_of_free_list(int bnum, bool present, bool status) {

	/*
		Objective: 		To insert a buffer at the end of Free List.
		Return Type:	void(null)
		Parameters:
			bnum: 		Specifies Block Number to be inserted.
			present: 	Specifies whether a block is currently removed from hash queue or not.
			status:		Specifies whether a block is marked for delayed write or not.
		Approach:		perform using Linked list insertion at the end.

	*/

	struct buffer *temp;
  	temp = (struct buffer*)malloc(sizeof(struct buffer));
  	temp->fprev = NULL;
  	temp->fnext = NULL;
  	temp->block_num = bnum;
  	temp->busy = false;
  	temp->delayed_write = status;
  	temp->hq_present = present;

  	if ( fheader == NULL ) {
		fheader = temp;
  	}

  	else {
		struct buffer *temp1 = fheader;
		while ( temp1->fnext != NULL )
	  		temp1 = temp1->fnext;

		temp1->fnext = temp;
		temp->fprev = temp1;
	}
}

void insert_at_begin_of_free_list(int bnum, bool present, bool status) {

	/*
		Objective:		To insert a buffer at the beginning of free list(in case of delayed-write completion).
		Return Type:	void(null)
		Parameters:		
			bnum: 		Specifies Block Number to be inserted.
			present: 	Specifies whether a block is currently removed from hash queue or not.
		Approach:		perform using Linked list insertion at the beginning.
	
	*/

  	struct buffer *temp;
  	temp = (struct buffer*)malloc(sizeof(struct buffer));
  	temp->block_num = bnum;
  	temp->busy = false;
  	temp->delayed_write = status;
  	temp->fprev = NULL;
  	temp->hq_present = present;

  	if(fheader == NULL) {
		temp->fnext = NULL; 
		fheader = temp;
  	}

  	else {
		temp->fnext = fheader;
		fheader->fprev = temp;
		fheader = temp;
 	}
}

void delete_from_begin() {

	/*
		Objective:		To delete a block from beginning of Free List.
		Return Type:	void(null)
		Parameters:		
			No paramters required.
		Approach:		perform using Linked list deletion from the beginning.
	*/
  	
  	if (fheader == NULL)
		return;
  	else if(fheader->fnext == NULL)
		fheader = NULL;
  	else {
		fheader = fheader->fnext;
		fheader->fprev = NULL;
  	}	
}

struct buffer *delete() {

	/*
		Objective:		To delete a block from beginning of Free list.
		Return Type:	structure buffer
						returns deleted buffer
		Parameters:		
			No parameters required.
		Approach:		perform deletion from front of the free list using Linked list.
	*/

  	if (fheader == NULL)
		return NULL;
  	struct buffer *temp = fheader;
  	fheader = fheader->fnext;
  	if (fheader != NULL)
		fheader->fprev = NULL;
  	return temp;
}

void delete_from_free_list(int bnum) {

	/*
		Objective:		To delete a block from a free list.
		Return Type:	void(null)
		Parameters:
			bnum:		Specifies Block Number to be deleted.
		Approach:		perform deletion using Linked list deletion mechanism.
						To delete from Beginning:	use delete_from_begin()
						else:	perform necessary pointer manipulations descibed below. 
	*/

  	struct buffer *temp = fheader;
  	while (temp->block_num != bnum) 
		temp = temp->fnext;

  	if(temp==NULL)
		return;

  	if (temp == fheader) {
		delete_from_begin();
  	}	

  	else if (temp->fnext == NULL)
		temp->fprev->fnext = NULL;
  
  	else {
		temp->fprev->fnext = temp->fnext;
		temp->fnext->fprev = temp->fprev;  
  	}
}

struct buffer *search_block_in_free_list(int block_num) {

	/*
		Objective:		To search a specific block from free list
		Return Type:	structure buffer
						returns NULL,		if block is not found on Free list
						returns buffer,		if block is found on Free list
		Parameter:		
			block_num:	specifies the block number to be searched
		Approach:		perform using searching an element(block_num) from linked list
	*/

	struct buffer *temp = fheader;
	if (temp == NULL)
		return NULL;
	while(temp != NULL && temp->block_num != block_num) {
		temp = temp->fnext;
	}
	if (temp == NULL)
		return NULL;
	return temp;
}

int count_free_buffer() {

	/*
		Objective:		To count number of buffers in free list
		Return Type:	Integer
						returns the count of buffers
		Parameter:		None
		Approach:		perform using Linked list traversal mechanism and incrementing count if
						buffer is present.
	*/

	struct buffer *temp = fheader;
	int count = 0;
	while(temp != NULL) {
		count++;
		temp = temp->fnext;
	}
	return count;
}

void display_free_list() {

	struct buffer *temp = fheader;
	
	printf("\n The current status of Free List is : ");
	
	if (temp == NULL){
		printf(" NULL ");
	}
	else {
		while (temp != NULL) {
			printf("%d ", temp->block_num);
			temp = temp->fnext;
		}
	}
	printf("\n");
}
/*-------------------------------------------------------------------------------------------------------------*/