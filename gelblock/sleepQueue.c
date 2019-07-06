#include "bufferCache.h"

struct sleep_queue *front = NULL;
struct sleep_queue *rear = NULL;
struct sleep_queue *specific_front = NULL;
struct sleep_queue *specific_rear  = NULL;

/*--------------------------------------------Sleep-Queue(Any-Buffer)-----------------------------------------*/
void insert_at_end_of_SQ(int processNo, int bnum) {

	/*
		Objective:		To insert a buffer in sleeping queue.
		Return Type:	void(null)
		Parameter:		
			processNo:	specifies process which request is not fullfilled
			bnum:		specifies block-no(request not completed) for which process has entered into sleep.
		Approach:		perform using linked link insertion mechanism.
	*/

	struct sleep_queue *temp;
	temp = (struct sleep_queue*)malloc(sizeof(struct  sleep_queue));
	temp->Process_no = processNo;
	temp->block_no = bnum;
	temp->next = NULL;

	if (front == NULL) {
		front = temp;
		rear = temp;
	}

	else {
		rear->next = temp;
		rear = temp;
	}
}

struct sleep_queue *delete_from_SQ(int num) {

	/*
		Objective:		To delete a process(sleeping process) from Sleep Queue
		Return Type:	structure sleep-queue process-buffer
		Parameter:	
			num:		specifies the position of sleep-queue node,i.e. to be deleted
		Approach:		perform using linked list deletion mechanism at a specific position. 
	*/

	struct sleep_queue *temp1;
	struct sleep_queue *temp = front;
	
	if (temp == NULL)
		return NULL;

	if (num == 0) {
		temp1 = front;
		if (temp->next == NULL) {
			front = NULL;
			rear = NULL;
		}
		else
			front = front->next;
		return temp1;
	}

	int count = 1;
	while (temp->next != NULL && count < num) { 
		temp = temp->next;
		count++;
	}

	if (temp->next == NULL && count < num){
		return NULL;
	}

	temp1 = temp->next;
	if(temp->next == rear) {
		temp->next = NULL;
		rear = temp;
	}

	else 
		temp->next=temp->next->next;

	return temp1;

}

int search_in_SQ(int proc) {
	
	/*
		Objective:		To search for a specific process in Sleep Queue
		Return Type:	Integer
							returns the block_no corresponding to specific process waiting in Sleeping Queue
		Parameter:		
			proc:		specifies the process number corresponding to which a block is needed to be
						searched in sleeping queue	
		
		Approach:		perform using linked list traversal.
	*/

	struct sleep_queue *temp = front;
	if (temp == NULL)
		return -1;

	while(temp != NULL && temp->Process_no != proc ) 
		temp = temp->next;

	if (temp == NULL)
		return -1;

	else
		return temp->block_no;
}

int count_no_of_process_in_SQ() {

	/*
		Objective:		To count no of processes waiting in sleeping queue
		Return Type:	Integer
							returns the count of processes in sleeping queue
		Parameter:		None.
		Approach:		perform using linked list traversal mechanism.
	*/

	int count = 0;
	if (front == NULL)
		return count;
	struct sleep_queue *temp = front;
	while(temp != NULL) {
		count++;
		temp = temp->next;
	}
	return count;
}

struct sleep_queue *wakeup_anyBuffer() {

	/*
		Objective:		To wakeup one of process from sleeping queue
		Return Type:	structure sleep-queue
						return waked-up Process
		Parameter:		None
		Approach:		perform using linked list traversals mechanism and random
						function.
						
						Initially a random number is selected,and then deletion is 
						called at specific position(i.e. at selected position).
	*/

	int count = count_no_of_process_in_SQ();
	
	if (count == 0)
		return NULL;
	
	srand(time(NULL)); 
    int no = (rand() % count);
    struct sleep_queue *temp = delete_from_SQ(no);
   	return temp;
}

/*-----------------------------------------Sleep-Queue(Specific-Buffer)----------------------------------------*/

void insert_at_end_of_SQ_specificBuffer(int processNo, int bnum) {

	/*
		Objective:		To insert a buffer in sleeping queue.
		Return Type:	void(null)
		Parameter:		
			processNo:	specifies process which request is not fullfilled
			bnum:		specifies block-no(request not completed) for which process has entered into sleep.
		Approach:		perform using linked link insertion mechanism.
	*/

	struct sleep_queue *temp;
	temp = (struct sleep_queue*)malloc(sizeof(struct  sleep_queue));
	temp->Process_no = processNo;
	temp->block_no = bnum;
	temp->next = NULL;

	if ( specific_front == NULL) {
		specific_front = temp;
		specific_rear = temp;
	}

	else {
		specific_rear->next = temp;
		specific_rear = temp;
	}
}

void delete_from_SQ_specificBuffer(int pid) {

	/*
		Objective:		To delete a process(sleeping process) from Sleep Queue
		Return Type:	structure sleep-queue process-buffer
		Parameter:	
			num:		specifies the position of sleep-queue node,i.e. to be deleted
		Approach:		perform using linked list deletion mechanism at a specific position. 
	*/

	struct sleep_queue *temp = specific_front;
	
	if (temp == NULL)
		return;

	if (temp->Process_no == pid && temp->next == NULL)
		specific_front = NULL;

	else if(temp->Process_no == pid)
		specific_front = specific_front->next;

	else {

		while(temp->next != NULL && temp->next->Process_no != pid)
			temp = temp->next;

		if (temp->next == NULL)
			return;

		temp->next = temp->next->next; 

	}
}



int search_in_SQ_specificBuffer(int proc) {
	
	/*
		Objective:		To search for a specific process in Sleep Queue
		Return Type:	Integer
							returns the block_no corresponding to specific process waiting in Sleeping Queue
		Parameter:		
			proc:		specifies the process number corresponding to which a block is needed to be
						searched in sleeping queue	
		Approach:		perform using linked list traversal.
	*/

	struct sleep_queue *temp = specific_front;
	if (temp == NULL)
		return -1;

	while(temp != NULL && temp->Process_no != proc ) 
		temp = temp->next;

	if (temp == NULL)
		return -1;

	else
		return temp->block_no;
}

int count_no_of_process_in_SQ_specificBuffer() {

	/*
		Objective:		To count no of processes waiting in sleeping queue
		Return Type:	Integer
							returns the count of processes in sleeping queue
		Parameter:		None.
		Approach:		perform using linked list traversal mechanism.
	*/

	int count = 0;
	if (specific_front == NULL)
		return count;
	struct sleep_queue *temp = specific_front;
	while(temp != NULL) {
		count++;
		temp = temp->next;
	}
	return count;
}

int count_occurrences_of_specificBuffer(int block_num) {

	struct sleep_queue *temp = specific_front;
	int count = 0;
	while (temp != NULL) {
		if (temp->block_no == block_num)
			count++;
		temp = temp->next;
	}
	return count;
}

struct sleep_queue *wakeup_specificBuffer(int block_num) {

	/*
		Objective:		To wakeup one of process from sleeping queue
		Return Type:	structure sleep-queue
						return waked-up Process
		Parameter:		None
		
		Approach:		perform using linked list traversals mechanism and random
						function.
						Initially a random number is selected,and then deletion is 
						called at specific position(i.e. at selected position).
	*/

	int count = count_occurrences_of_specificBuffer(block_num);
	if (count == 0)
		return NULL;

	srand(time(NULL)); 
	int no = (rand()%count) + 1;

	struct sleep_queue *temp = specific_front;
	// while (temp != NULL) {
	// 	if(temp->block_no == block_num) {
	// 		struct sleep_queue *temp1 = temp;
			// delete_from_SQ_specificBuffer(temp->block_no);
			// return temp1;
	// 	}
	// 	temp = temp->next;
	// } 
	// return NULL;

	int pid;
	while(temp != NULL) {
		if(temp->block_no == block_num) {
			pid = temp->Process_no;
			no--;
		}
		if(no == 0) {
			struct sleep_queue *temp1 = temp;
			delete_from_SQ_specificBuffer(pid);
			return temp1;
		}
		temp = temp->next;
	}
	return NULL;
}
/*-------------------------------------------------------------------------------------------------------------*/