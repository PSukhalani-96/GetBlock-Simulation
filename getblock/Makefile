CC=gcc

CFLAGS=-c -Wall 

all: kernel
	$(CC) Process.c -o Process
	$(CC) dependency.c -o dependency
	$(CC) delayedProcessing.c -o delayedProcessing

kernel: brelease.o delayedWrite.o freeList.o getBlock.o hashQueue.o kernel.o processQueue.o sleepQueue.o
		$(CC) brelease.o delayedWrite.o freeList.o getBlock.o hashQueue.o kernel.o processQueue.o sleepQueue.o -o kernel

brelease.o: brelease.c
		$(CC) $(CFLAGS) brelease.c

delayedWrite.o: delayedWrite.c
		$(CC) $(CFLAGS) delayedWrite.c

freeList.o: freeList.c
		$(CC) $(CFLAGS) freeList.c

getBlock.o: getBlock.c
		$(CC) $(CFLAGS) getBlock.c

hashQueue.o: hashQueue.c
		$(CC) $(CFLAGS) hashQueue.c

kernel.o: kernel.c
		$(CC) $(CFLAGS) kernel.c

processQueue.o: processQueue.c
		$(CC) $(CFLAGS) processQueue.c

sleepQueue.o: sleepQueue.c
		$(CC) $(CFLAGS) sleepQueue.c

clean:
	rm -rf *o kernel
	rm -rf ./kernel

run:
	./kernel