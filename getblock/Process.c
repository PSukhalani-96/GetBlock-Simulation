#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 10000					//	Specifies the port no. for socket programming. 

/*------------------------------------------------Process-Main-Function()--------------------------------------------*/
int main(){

	/*
		Objective:	To create a socket connection between Kernel and Process.
					
					-	"send line" is used to send the Block-No required by the process
						to the kernel.
					
					-	"recv line" is used to receive the status of Block-No(response to 
						process request).
						status: specifies whether block is allocated to a process or not. 
	*/

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}

	recv(clientSocket, buffer, 1024, 0);
	printf("\n-----------------------------------------------------------------");
	printf("\n Process %d is scheduled for execution.",atoi(buffer));
	printf("\n-----------------------------------------------------------------");

	int process = atoi(buffer);
	char temp[5];
	sprintf(temp, "%d",process);
	char proc[5];
	strcpy(proc, "\e]2;");
	strcat(proc, "Executing Process - ");
	strcat(proc,temp);
	strcat(proc,"\a");
	printf("%s",proc);
	
	printf("\n>> Enter the required Block No.: 	");
	scanf("%s", &buffer[0]);
	
	printf("\n------------------------------STATUS-----------------------------");
	printf("\n");

	send(clientSocket, buffer, 1024, 0);

	if(recv(clientSocket, buffer, 1024, 0) < 0)
		printf(" Error in receiving Block.\n");
	
	else { 
		printf("\n%s",buffer );
		printf("\n-----------------------------------------------------------------");
		printf("\n");
	}
	
	sleep(8);
	return 0;
}
/*-------------------------------------------------------------------------------------------------------------------*/