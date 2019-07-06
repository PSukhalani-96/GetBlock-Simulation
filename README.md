# GetBlock-Simulation
File Management(Operating System) lower-level algorithm.

# Description
A Process can request ‘Kernel’ for a block. Kernel runs ‘getblk-algorithm’ corresponding to	requested block. A block can
either be ‘allocated’ or ‘not allocated’ to a process, depending on one of **five scenarios** :-
<p align="justify">
	
**Scenario-1:**   The requested block is present in both Hash queue and Free List. The requested block is successfully 
				  allocated to the process.
	
**Scenario-2:**   The requested block is not present in Hash Queue but Free list is non-empty and removed buffer is not
				  marked delayed write. Thus,requested block is successfully allocated to the process.
					
**Scenario-3:**   The requested block is not present in Hash Queue and buffer removed from free List is marked Delayed 
				  write. This removed buffer will be inserted into Hash-Queue-Delayed-Write-Mode and again getblk algo
				  is called on the requested buffer.
					
**Scenario-4:**   The requested block is not present in Hash Queue and Free List is Empty.The requesting process is inserted 				   into sleeping queue(Sleep-Queue), waiting for any buffer to become free.
					
**Scenario-5:**   The requested block is present in Hash Queue but it is currently busy with other process.The requesting 
				  process is inserted into sleeping queue(Sleep-Queue),waiting for specific buffer to become free.
					
A process can release a buffer using **brelse-algorithm** .When a process releases it’s associated buffer, all processes 
waiting for any or ‘this-specific’ to become free are woken-up and one of the process from these waiting-processes, can 
acquired the released-block.Simultaneously, the process which has released its buffer can either terminate using **EXIT** 
or **request** ‘kernel’ for a new block.
</p>

# Download and run
	$ git clone https://github.com/PSukhalani-96/GetBlock-Simulation.git
	$ cd GetBlock-Simulation/getblock
	$ make
	$ ./kernel
	
# Sample Screenshots
![1](https://user-images.githubusercontent.com/50991324/60758883-202edc00-a03a-11e9-885c-dc87780ece66.png)

![2](https://user-images.githubusercontent.com/50991324/60758897-70a63980-a03a-11e9-8dd1-d3883e496351.png)

![3](https://user-images.githubusercontent.com/50991324/60758908-929fbc00-a03a-11e9-9aa0-1ed893996558.png)

![4](https://user-images.githubusercontent.com/50991324/60758909-959aac80-a03a-11e9-923d-e12fadbdc977.png)

![5](https://user-images.githubusercontent.com/50991324/60758912-992e3380-a03a-11e9-8eb7-dbbfa7721a4b.png)

![6](https://user-images.githubusercontent.com/50991324/60758913-9c292400-a03a-11e9-8162-bb69fd5c1cf8.png)

![7](https://user-images.githubusercontent.com/50991324/60758914-9df2e780-a03a-11e9-97f4-6ea5ffe544d7.png)

![8](https://user-images.githubusercontent.com/50991324/60758915-9e8b7e00-a03a-11e9-8610-15cdad98c928.png)

![9](https://user-images.githubusercontent.com/50991324/60758917-a1866e80-a03a-11e9-852b-c056360fc654.png)

![10](https://user-images.githubusercontent.com/50991324/60758918-a2b79b80-a03a-11e9-8213-7685209bf596.png)

![11](https://user-images.githubusercontent.com/50991324/60758920-a6e3b900-a03a-11e9-94ae-b6367c8fd3f8.png)

![12](https://user-images.githubusercontent.com/50991324/60758921-a6e3b900-a03a-11e9-809e-acddac8d5f88.png)

![13](https://user-images.githubusercontent.com/50991324/60758922-a8ad7c80-a03a-11e9-8506-5b77547b7cbd.png)

![14](https://user-images.githubusercontent.com/50991324/60758924-aba86d00-a03a-11e9-9323-66787b6d2fb7.png)

![15](https://user-images.githubusercontent.com/50991324/60758927-b105b780-a03a-11e9-8a62-fe1183cece56.png)
