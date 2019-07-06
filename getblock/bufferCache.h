#ifndef PROJ_bufferCache_H
#define PROJ_bufferCache_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define PROCESS 6
#define BUFFER 4

extern int process;
int no_of_process;

struct buffer {
	int block_num;
  	struct buffer *hnext, *hprev;
  	struct buffer *fnext, *fprev;
  	bool busy;
  	bool delayed_write; 
  	bool hq_present;
  	bool old;
  	clock_t time_allocated; 
};

struct mem_process {
	int pid;
	struct buffer *allocated_block;
	struct mem_process *next;
	bool curently_open;
};

struct sleep_queue{
	int Process_no;
	int block_no;
	struct sleep_queue *next;
};

/*-------------------------------------------------------------------------------------------------------------------*/
extern struct buffer *fheader;
extern struct buffer *hq_header[4];
extern struct buffer *delayed;
extern struct mem_process *Proc;

extern struct sleep_queue *front;
extern struct sleep_queue *rear;

extern struct sleep_queue *specific_front;
extern struct sleep_queue *specific_rear;
/*-------------------------------------------------------------------------------------------------------------------*/

/* GetBlock Function Declaration */
int getblk(int block);

/* Free-List Functions */
void insert_at_end_of_free_list(int bnum, bool present, bool status);
void insert_at_begin_of_free_list(int bnum, bool present, bool status);
void delete_from_begin();
struct buffer *delete();
void delete_from_free_list(int bnum);
struct buffer *search_block_in_free_list(int block_num);;
int count_free_buffer();
void display_free_list();

/* Hash-Queue Functions */
void initialise();
int find_hash_queue_no(int block_num);
struct buffer *search_block_num(int hq_index, int block_num);
void insert_in_hash_queue(int bnum, int i);
void display_hash_queue(int i);
void delete_from_begin_HQ(int index);
void delete_from_hash_queue(int bnum, int index);
void display_HQ();

/* Process-Queue Functions */
void insert_in_process_queue(struct buffer *temp);
void delete_from_process_queue(struct mem_process *temp1);
int count_process_in_system_call();
struct mem_process *search_mem_process(int processNo);

/* Delayed-Write Functions*/
void insert_in_delayed_write_queue(struct buffer *temp);
void delete_from_delayed_queue(struct buffer *temp);
void add_to_free_list_from_delayed();
int count_delayed_write();
void delayed_write_processing(struct buffer *temp);
struct buffer *search_in_delayed_write(int block_num);

/* Sleep-Queue Functions(Any-Buffer) */
void insert_at_end_of_SQ(int processNo, int bnum);
struct sleep_queue *delete_from_SQ(int num);
int search_in_SQ(int proc);
int count_no_of_process_in_SQ();
struct sleep_queue *wakeup_anyBuffer();

/* Sleep-Queue Functions(Specific-Block) */
void insert_at_end_of_SQ_specificBuffer(int processNo, int bnum);
void delete_from_SQ_specificBuffer(int pid);
int search_in_SQ_specificBuffer(int proc);
int count_no_of_process_in_SQ_specificBuffer();
struct sleep_queue *wakeup_specificBuffer(int block_num);
int count_occurrences_of_specificBuffer(int block_num);

/* Release a buffer Function */
void brelse(int processNo);

#endif
/*-------------------------------------------------------------------------------------------------------------------*/