#include "bufferCache.h"

struct buffer *delayed = NULL;

/*------------------------------------------Delayed-Write-Implementation----------------------------------------*/
void insert_in_delayed_write_queue(struct buffer *temp) {

	/*
		Objective:		To insert a buffer in delayed write mode.
		Return Type:	void(null)
		Parameter:		structure buffer *temp
							specifies the buffer(temp) to be inserted in delayed write queue
		Approach:		perform using linked list insertion mechanism.
	*/

	temp->time_allocated = clock();
	if (delayed == NULL)
		delayed = temp;

	else {
		struct buffer *temp1 = delayed;
		while (temp1->fnext != NULL)
			temp1 = temp1->fnext;
		temp1->fnext = temp;
	}

}

void delete_from_delayed_queue(struct buffer *temp) {

	/*
		Objective:		To remove a buffer from delayed_write mode
		Return Type:	void(null)
		Parameter:		structure buffer *temp
							specifies the buffer which is to be removed from delayed queue
		Approach:		perform using linked list deletion mechanism.
	*/

	if(delayed == NULL)
		return;

	if(delayed == temp && delayed->fnext == NULL)
		delayed = NULL;

	else if(delayed == temp && delayed->fnext != NULL)
		delayed = delayed->fnext;

	else {
		struct buffer *temp1 = delayed;
		while(temp1->fnext != NULL && temp1->fnext != temp)
			temp1 = temp1->fnext;

		if(temp1->fnext == NULL)
			return;

		if(temp1 == delayed)
			delayed = delayed->fnext;

		else if(temp1->fnext->fnext == NULL)
			temp1->fnext = NULL;

		else
			temp1->fnext = temp1->fnext->fnext;
	} 
}

struct buffer *search_in_delayed_write(int block_num) {

	struct buffer *temp = delayed;
	while (temp != NULL) {
		if (temp->block_num == block_num)
			return temp;
		temp = temp->fnext;
	}
	return NULL;

}

void add_to_free_list_from_delayed() {

	/*
		Objective:		To add a buffer from delayed queue to free list.
		Return Type:	void(null)
		Parameter:		None
		Approch:		perform using linked list traversal, deletion and above defined timeout
						mechanism.
	*/

	// ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 
  
  	if(*str == '\0') {
  		*str = '\0';
  		 //detach from shared memory  
    	shmdt(str); 
    
    	// destroy the shared memory 
    	shmctl(shmid,IPC_RMID,NULL);
  	}
  	else {
  		int block_no = atoi(str);
  		struct buffer *temp = search_in_delayed_write(block_no);
  		delete_from_delayed_queue(temp);
  		printf("\n Buffer with block num %d has removed from delayed Write queue and added to the front of Free List..", temp->block_num);
		insert_at_begin_of_free_list(temp->block_num, false, false);
		*str = '\0';
      
    	//detach from shared memory  
    	shmdt(str); 
    
    	// destroy the shared memory 
    	shmctl(shmid,IPC_RMID,NULL);
  	}
}

int count_delayed_write() {

	/*
		Objective:		To count no of buffers present in delayed_write queue
		Return Type:	Integer
							return count of buffers
		Parameter:		None
		Approach:		perform using linked list traversal.
	*/

	struct buffer *temp = delayed;
	int count = 0;
	while(temp != NULL) {
		count++;
		temp = temp->fnext;
	}
	return count;
}

void delayed_write_processing(struct buffer *temp) {

	/*
		Objective:	To simulate asynchronous writing.
	*/
	char temp1[5];
	char cmd[100];
	strcpy(cmd,"gnome-terminal --geometry 40x17+200+200");
	strcat(cmd," -- \"./delayedProcessing\" ");
	sprintf(temp1, "%d",temp->block_num);
	strcat(cmd,"\"");
	strcat(cmd,temp1);
	strcat(cmd,"\"");
	system(cmd);
}
/*-------------------------------------------------------------------------------------------------------------*/