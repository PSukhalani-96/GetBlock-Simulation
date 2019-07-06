#include "bufferCache.h"

struct buffer *hq_header[4];

/*--------------------------------------------Hash-Queue-Implementation----------------------------------------*/
void initialise() {
	for(int i=0; i<4; i++)
		hq_header[i] = NULL;
}

int find_hash_queue_no(int block_num) {

	/*
		Objective:		To Find the index of hash queue in which block num to be searched/to be inserted.
		Return Type:	integer
						return the index of hash_queue.
		Parameters:
			block_num:	Specifies Block Number to be inserted/deleted/searched in hash queue.
		Approach:		perform using modulo function
						index-of-hash-queue = (block_num) % 4
	*/

  	return (block_num % 4);
}

struct buffer *search_block_num(int hq_index, int block_num) {

	/*
		Objective:		To search a block in Hash Queue.
		Return Type:	structure buffer
		Parameters:		
			hq_index:	specifies the index of hash queue
			block_num:	specifies block number to be searched
		Approach:		perform using Linked list searching for specific number.
	*/

  	if (hq_header[hq_index] == NULL)
		return NULL;

  	struct buffer *temp = hq_header[hq_index];
  	while(temp != NULL && temp->block_num != block_num)
		temp = temp->hnext;

  	if (temp == NULL)
		return NULL;

  	return temp;

}

void insert_in_hash_queue(int bnum, int i) {

	/*	
		Objective:		To insert a block at the end of specific hash-queue
		Return Type:	void(null)
		Parameter:
			bnum:		specifies block to be inserted.
			i:			specifies index of hash queue.
		Approach:		perform using insertion at the end of Linked List mechanism.

	*/

  	struct buffer *temp1 = (struct buffer*)malloc(sizeof(struct buffer));
  	temp1->block_num = bnum;
  	temp1->hnext = NULL;
  	temp1->hprev = NULL;
  	temp1->busy  = true;
  	temp1->time_allocated = clock();
  	temp1->hq_present = true;

  	srand(time(NULL));
  	int no = rand() % 2;
  	if(no == 0)
  		temp1->old = false;
  	else
  		temp1->old = true;

  	if (hq_header[i] == NULL) {
		hq_header[i] = temp1;
	}

  	else {
	
		struct buffer *temp = hq_header[i];
		while ( temp->hnext != NULL)
	  		temp = temp->hnext;

		temp->hnext = temp1;
		temp1->hprev = temp;
	}
}

void display_hash_queue(int i) {

	/*
		Objective:		To display contents of a specific Hash Queue.
		Return Type:	void(null)
		Parameter:
			i:			specifies index of hash queue to be displayed.
		Approach:		perform display using linked list mechanism.
	*/

  	struct buffer *temp = hq_header[i];
  	struct buffer *temp1 = fheader;
  	struct buffer *temp2 = delayed;

  	printf("\n Hash_Queue[%d] : ",i);
  	
  	while (temp != NULL) {
  		if ( temp->hnext == NULL)
  			printf("%d", temp->block_num);
  		else
			printf("%d->",temp->block_num );
		temp = temp -> hnext;
  	}

  	while(temp1 != NULL) {
  		if (temp1->block_num % 4 == i && temp1->hq_present == true) {
  			if ( temp1->fnext == NULL)
  				printf("%d", temp1->block_num);
  			else
  				printf("->%d",temp1->block_num );
  		}
  		temp1 = temp1->fnext;
  	}

  	while(temp2 != NULL) {
  		if ( temp2->fnext == NULL)
  				printf("%d", temp2->block_num);
  			else
  				printf("->%d",temp2->block_num );
  		temp2 = temp2->fnext;
  	}

  	printf("\n");
}

void delete_from_begin_HQ(int index) {

	/*
		Objective:		To delete a buffer from beginning of hash queue.
		Return Type:	void(null)
		Parameter:
			index:		Specifies index of hash-queue
		Approach:		perform deletion using Linked list deletion mechanism.
	*/

  	if (hq_header[index] == NULL)
		return;
  	else if(hq_header[index]->hnext == NULL)
		hq_header[index] = NULL;
  	else {
		hq_header[index] = hq_header[index]->hnext;
		hq_header[index]->hprev = NULL;
  	}
}

void delete_from_hash_queue(int bnum, int index) {

	/*
		Objective:		To delete a buffer from Hash Queue.
		Return Type:	void(null)
		Parameter:	
			bnum:		specifies block number to be deleted.
			index:		specifies index of hash queue
		Approach:		perform deletion using Linked list deletion mechanism.		
	*/

  	struct buffer *temp = hq_header[index];

  	while (temp!=NULL && temp->block_num != bnum) 
		temp = temp->hnext;

  	if( temp==NULL )
		return;

  	if (temp == hq_header[index]) {
		delete_from_begin_HQ(index);
  	}

  	else if (temp->hnext == NULL)
		temp->hprev->hnext = NULL;
  
  	else {
		temp->hprev->hnext = temp->hnext;
		temp->hnext->hprev = temp->hprev;  
  	}
}
/*-------------------------------------------------------------------------------------------------------------*/