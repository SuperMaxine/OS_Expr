#include <stddef.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>

#define TEXT_SZ 2048
 
struct shared_use_st{
    int written; // 作为一个标志，非0：表示可读，0：表示可写
    int payload;
};

int main(){
	void *shm = NULL;
    struct shared_use_st *shared; // 指向shm
    int shmid; // 共享内存标识符

	// 创建共享内存
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if (shmid == -1){
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
	
	if(fork() == 0){
		// 将共享内存连接到当前进程的地址空间
		shm = shmat(shmid, 0, 0);
		if (shm == (void *)-1){
			fprintf(stderr, "shmat failed\n");
			exit(EXIT_FAILURE);
		}

		// 设置共享内存
		shared = (struct shared_use_st*)shm; // 注意：shm有点类似通过 malloc() 获取到的内存，所以这里需要做个 类型强制转换
		shared->written = 0;
		// 读取共享内存中的数据
		while (1){
			// 没有进程向内存写数据，有数据可读取
			if (shared->written == 1){
				// 输入了 end，退出循环（程序）
				if (shared->payload == 54188){
					break;
				}
				printf("The square of this number is: %d\n", shared->payload * shared->payload);
	
				// 读取完数据，设置written使共享内存段可写
				shared->written = 0;
	
			}
			//else sleep(1);
		}

		// 把共享内存从当前进程中分离
		if (shmdt(shm) == -1)
		{
			fprintf(stderr, "shmdt failed\n");
			exit(EXIT_FAILURE);
		}

		exit(0);
	}else{
		if(fork() > 0)wait(NULL);
		else{
			// 将共享内存连接到当前的进程地址空间
			shm = shmat(shmid, (void *)0, 0);
			if (shm == (void *)-1)
			{
				fprintf(stderr, "shmat failed\n");
				exit(EXIT_FAILURE);
			}

			// 设置共享内存
			shared = (struct shared_use_st *)shm;
			while (1)
			{		
				// 向共享内存中写入数据
				printf("Enter a number(Enter 54188 to exit): ");
				scanf("%d",&shared->payload);
				// printf("you input %d in 1\n",shared->payload);
				// 写完数据，设置written使共享内存段可读
				shared->written = 1;

				// 输入了end，退出循环（程序）
				if (shared->payload == 54188)
				{
					break;
				}

				sleep(1);
			}

			// 把共享内存从当前进程中分离
			if (shmdt(shm) == -1)
			{
				fprintf(stderr, "shmdt failed\n");
				exit(EXIT_FAILURE);
			}

			exit(0);
		}
	}

	// 删除共享内存
    if (shmctl(shmid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
	return 0;
}
