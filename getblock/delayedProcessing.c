#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/stat.h>

/*-------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char const *argv[]) {

	/*
		Objective:		To simulate Asynchronous Writing.
	*/
	
	char proc[250];
	strcpy(proc, "\e]2;");
	strcat(proc, "Asynchronous Write to Block-");
	strcat(proc, argv[1]);
	strcat(proc,"\a");
	printf("%s",proc);
	
	clock_t startTime = clock();
	
    char result[15];

	while (1) {

		clock_t currentTime = clock() - startTime;
  		double time_taken = ((double)currentTime)/CLOCKS_PER_SEC;
		
		if ( time_taken > 0.0020) {	
			strcpy(result,argv[1]);
			break;
		}
		printf("\nWriting Buffer-Content to Disk. %s", result);
		sleep(1);
	}
	printf("\nMoving This buffer to front of the Free List.");
	printf("\n");
	sleep(2);
	
	// ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 
  
    strcpy(str, argv[1]);

    printf("Data written in memory: %s\n",str); 
      
    //detach from shared memory  
    shmdt(str);
	
	return 0;
}
/*-------------------------------------------------------------------------------------------------------------------*/
