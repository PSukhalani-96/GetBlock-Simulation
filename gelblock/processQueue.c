#include "bufferCache.h"

struct mem_process *Proc = NULL;

/*-------------------------------------------------Process-Queue-----------------------------------------------*/
void insert_in_process_queue(struct buffer *temp) {

	/*
		Objective:		To insert a process in Process_queue
		Return Type:	void(null)
		Parameter:		
			structure buffer *temp:		insert buffer temp and its corresponding to process queue
		Approach:		perform insertion using linked list insertion mechanism.
	*/

	struct mem_process *temp1 = (struct mem_process*)malloc(sizeof(struct  mem_process));;
	temp1->pid = process;
	temp1->allocated_block = temp;
	temp1->next = NULL;
	temp1->curently_open = true;

	if (Proc == NULL) {
		Proc = temp1;
	}

	else {
		struct mem_process *temp2 = Proc;
		while (temp2->next != NULL)
			temp2 = temp2->next;
		temp2->next = temp1;
			
	}
}

void delete_from_process_queue(struct mem_process *temp1) {

	/*
		Objective:		To delete a process from Process queue
		Return Type:	void(null)
		Parameter:
			structure mem_process *temp1:	specifies which process is to be deleted from
											Process_queue
		Approach:		perform deletion using linked list deletion mechanism.
	*/

	if (Proc == NULL) 
		return;

	if(Proc == temp1 && Proc->next == NULL){
		Proc = NULL;
	}

	else {
		if(temp1 == Proc )
			Proc = Proc->next;

		else {
			struct mem_process *temp = Proc;
			while(temp->next != NULL && temp->next != temp1)
				temp = temp->next;

			if(temp->next == NULL)
				return;
			else if (temp->next->next == NULL)
				temp->next = NULL;

			else
				temp->next = temp->next->next;
		}
	}
}

int count_process_in_system_call() {
	
	/*
		Objective:		To count no. of processes which are executing their system call.
		Return Type:	Integer
							returns no. of processes in Process_queue
		Parameter:		None.
		Approach:		perform using linked list traversal mechanism.
	*/
	
	int count = 0;
	struct mem_process *temp = Proc;
	while(temp != NULL) {
		count++;
		temp = temp->next;
	}
	return count;
}

struct mem_process *search_mem_process(int processNo) {
	
	/*
		Objective:		To search a "mem_process" indicated by procfrom  
		Return Type:	structure mem_process 
		Parameter:
			processNo:	specifies the process no. to be searched
		Approach:		perform using linked list traversal
	*/
	
	struct mem_process *temp = Proc;
	while (temp != NULL){
		if(temp->pid == processNo)
			return temp;
		temp = temp->next;
	}
	return NULL;
}
/*-------------------------------------------------------------------------------------------------------------*/