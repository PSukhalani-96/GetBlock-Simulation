#include "bufferCache.h"

#define PORT 10000
int process = 1;

/*-------------------------------------------------------------------------------------------------------------------*/

void processing(int i) {

	/*
		Objective:		To open a separate Gnome-Terminal corresponding to each process executing 
						it system call.
		Return Type:	void(null)
		Parameter:		
			Integer i:	gives the Process No.
	*/

	int arr1[10] = {616,1025,616,1025,616,1025,616,1025};
	int arr2[10] = {0,0,300,300,600,600,900,900};
	
	char temp[5];
	char cmd[100];
	strcpy(cmd,"gnome-terminal --geometry 40x14+");
	sprintf(temp, "%d", arr1[i-1]);
	strcat(cmd,temp);
	strcat(cmd,"+");
	sprintf(temp, "%d", arr2[i-1]);
	strcat(cmd,temp);
	strcat(cmd," -- \"./dependency\" ");
	sprintf(temp, "%d",i);
	strcat(cmd,"\"");
	strcat(cmd,temp);
	strcat(cmd,"\"");
	system(cmd);
}

/*--------------------------------------------Kernel-Processing----------------------------------------------------*/
int main() {

	/*
		Objective:	Main-Function establishes a socket connection(to support multiprogramming) between
					kernel and process.
					
					It follows First-Come-First-Serve Scheduling Policy.
					->	Initially, Every process will get a chance to get the block.
					->	Then, each process will each get the block or enter into sleeping queue depending on 
						type of request.
					->	If a Process gets the block, then process will be inserted into Process_queue. It 
						will go to indefinite sleep until signal is being sent from process to kernel.This signal 
						indicates that a process has completed its system call execution.
						Now the process will unlock this block, and kernel will wake up one of the process from
						sleeping queue to check whether chosen sleeping process request can be fullfilled by 
						currently unlocked buffer.
						->	If request can be fullfilled, buffer will be allocated to the chosen sleeping process,
							and then this process starts execution of system call(enters into Process_queue).
						->	Else chosen sleeping process will again be inserted into sleeping queue.
					->	Now, the process which initially released/unlocked its buffer, can request for another 
						block.
					->	These above steps continues, until a Process exits/terminates from the system.
				
	*/


	int sockfd, ret;
  	struct sockaddr_in serverAddr;

  	int newSocket;
  	struct sockaddr_in newAddr;

  	socklen_t addr_size;

  	char buf[1024];

  	sockfd = socket(AF_INET, SOCK_STREAM, 0);
  	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
  	}

  	memset(&serverAddr, '\0', sizeof(serverAddr));
  	serverAddr.sin_family = AF_INET;
  	serverAddr.sin_port = htons(PORT);
  	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
  	}
  	
	if(listen(sockfd, 10) > 0 || listen(sockfd, 10) < 0){
		printf("[-]Error in binding.\n");
  	} 

  	for(int i=0; i < BUFFER; i++) {
		insert_at_end_of_free_list(0, false, false);
	}
	
	initialise();

	no_of_process = PROCESS;

	while(no_of_process > 0){
		
		printf("\e]2;Kernel\a");
		system("clear");
  		printf("\e[8;37;62t");

  		add_to_free_list_from_delayed();

  		printf("\n--------------------KERNEL PROCESS--------------------------\n");
  		printf("\n Degree Of Multiprogramming                   : %d", no_of_process);
  		printf("\n Number of process in Ready Queue             : %d", no_of_process - count_process_in_system_call() - count_no_of_process_in_SQ() - count_no_of_process_in_SQ_specificBuffer());
  		printf("\n No of Free Buffers                           : %d", count_free_buffer());
  		printf("\n No of Buffers Present in Delayed Write Queue : %d", count_delayed_write());
  		printf("\n No of Processes sleeping in Waiting-Queue    : %d", count_no_of_process_in_SQ() + count_no_of_process_in_SQ_specificBuffer());
  		printf("\n No of Processes executing their System Call  : %d\n", count_process_in_system_call());

  		printf("\n------------------------------------------------------------");

  		int arr1[no_of_process], arr2[no_of_process], arr3[no_of_process];

  		for (int i=0;i < no_of_process; i++)
  			arr1[i] = 0;

  		for(int i=0; i < no_of_process; i++) {
  			arr2[i] = 0;
  			arr3[i] = 0;
  		}

  		int i=0;
  		if ( count_process_in_system_call() > 0) {
  			
  			struct mem_process *temp1 = Proc;
  			
  			while (temp1 != NULL) {
  				arr1[i] = temp1->pid;
  				arr2[i] = temp1->allocated_block->block_num;
  				i++;
  				temp1 = temp1->next;
  			}
  		}

		if (count_no_of_process_in_SQ() > 0) {
  			struct sleep_queue *temp2 = front;
  			while(temp2 != NULL) {
  				arr1[i] = temp2->Process_no;
  				arr3[i] = temp2->block_no;
  				i++;
  				temp2 = temp2->next;
  			}
   		}

   		if (count_no_of_process_in_SQ_specificBuffer() > 0) {
   			struct sleep_queue *temp3 = specific_front;
   			while (temp3 != NULL) {
   				arr1[i] = temp3->Process_no;
   				arr3[i] = temp3->block_no;
   				i++;
   				temp3 = temp3->next;
   			}
   		}

  		sleep(1);
  		printf("\n\t     Current Status of Processes are ");
  		printf("\n------------------------------------------------------------");

  		printf("\n     Process-Id\t    Locked-Buffer\tWaiting-Buffer");
  		for(int i=0; i < no_of_process; i++) {
  			if (arr1[i] == 0)
  				printf("\n\t  -");
  			else
  				printf("\n\t  %d",arr1[i]);
  			if (arr2[i] == 0)
  				printf("\t\t  -");
  			else
  				printf("\t\t  %d", arr2[i]);
  			if (arr3[i] == 0)
  				printf("\t\t      -");
  			else
  				printf("\t\t      %d", arr3[i]);
  		}

  		printf("\n------------------------------------------------------------");
		sleep(1);

		if ( count_process_in_system_call() + count_no_of_process_in_SQ() + count_no_of_process_in_SQ_specificBuffer()  < no_of_process  ) {
			
			int status = 0;
			system("gnome-terminal --geometry 65x20+616+0 -- \"./Process\"");
			newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		
			if(newSocket < 0){
				exit(1);
			}
			int blockNo;
			char res[5];
			sprintf(res, "%d", process);
			send(newSocket, res, 1024 ,0);
			bzero(buf, sizeof(buf));
			recv(newSocket, buf, 1024, 0);
			
			if(strcmp(buf,"exit") == 0 || strcmp(buf,"Exit") == 0 || strcmp(buf,"EXIT") == 0) {

				printf("\n Process %d wants to exit from the system.", process);
				printf("\n Releasing Buffer associated to process.");
				brelse(process);
				status = 5;
			}

			else {
				printf("\n Currently Executing Process is : %d ",process);
				printf("\n Required Block by the process  : %d ", atoi(buf));
				blockNo = atoi(buf);
				
				printf("\n---------------------GETBLOCK PROCESSING--------------------");
				sleep(2);
				printf("\n Processing...");
				sleep(2);
				printf("\n Processing...");
				sleep(2);
				printf("\n Processing...");
				printf("\n---------------------STATUS OF HASH QUEUE-------------------\n");
				
				status = getblk(blockNo);

				if (status != 3) {
					display_hash_queue(0);
					display_hash_queue(1);
					display_hash_queue(2);
					display_hash_queue(3);	
				}
			}
			bzero(buf, sizeof(buf));

			if (status == 1) {
				printf("\n Block-Number %d is successfully allocated to process %d.",blockNo, process);
				strcpy(buf, " Block is present in both Hash-queue and Free-list.\n Required Block Successfully Allocated.");
				strcat(buf,"\n Process goes to infinite sleep until it completes system call.");
				send(newSocket, buf, 1024, 0);
			}

			else if (status == 2) {
				printf("\n Block-Number %d is successfully allocated to process %d.",blockNo, process);
				strcpy(buf, " Block is not present in Hash Queue but Free List is non-empty.\n Required Block Successfully Allocated.");
				strcat(buf,"\n Process goes to infinite sleep until it completes system call.");
				send(newSocket, buf, 1024, 0);
			}

			else if (status == 3) {
				printf("\n Block-Number %d cannot be allocated to process as block is currently busy in Hash Queue.",blockNo );
				printf("\n Send Process %d to sleep", process);
				strcpy(buf, " Block is present in Hash Queue but it is currently BUSY.\n Process goes to SLEEP.");
				strcat(buf, "\n Adding Process to Sleep Queue waiting for a specific buffer.");
				send(newSocket, buf, 1024, 0);
			}

			else if (status == 4) {
				printf("\n Block-Number %d cannot be allocated to process %d as free list is empty.",blockNo ,process);
				printf("\n Send Process %d to sleep", process);
				strcat(buf, " Block is not present in Hash Queue and Free List is EMPTY.\n Process goes to SLEEP.");
				strcat(buf, "\n Adding Process to Sleep Queue waiting for any buffer.");
				send(newSocket, buf, 1024, 0);
			}

			else if (status == 5 || status == 0) {
				printf("\n Process %d is successfully executed.\n Terminate Process.",process);
				no_of_process--;
				strcpy(buf, " Terminating Process...\n Terminating Process...\n Terminating Process");
				send(newSocket, buf, 1024, 0);
			}

			else {
				printf("\n Invalid Buffer");
				strcpy(buf, " You have not entered a valid Block Number.\n Try Again next. ");
				send(newSocket, buf, 1024, 0);	
				continue;
			}

			printf("\n------------------------------------------------------------");
			printf("\n");

			process++;
			if (process == PROCESS+1)
				process = 1;

			if (no_of_process < count_process_in_system_call() ) {
				printf("\n Context Switching to another Process ");
				printf("\n Taking Time to context-switch.");
			}
						
			sleep(10);
			close(newSocket);
		}
		else {

			struct mem_process *temp = Proc;
			while (temp != NULL ) {
				if (temp->curently_open) {
					processing(temp->pid);
					temp->curently_open = false;
				}
				temp = temp->next;
			}

			/*Pipes are used to send signal from one process to another.*/
			int fd;
    		char * myfifo = "/tmp/myfifo";
    		char result[1024];
    		*result = '\0';
			fd = open(myfifo, O_RDONLY);
    		read(fd, result, 1024);
    		close(fd);
    		
    		int processNo = atoi(result);
    		printf("\n Process %d has successfully completed its current system call.", processNo);
			brelse(processNo);
			sleep(10);
			
		}
	}
	
	return 0;
}
/*-------------------------------------------------------------------------------------------------------------------*/