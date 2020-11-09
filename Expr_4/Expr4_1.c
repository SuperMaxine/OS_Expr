#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "semaphore.h"

#define N 2 //缓冲区长度为2

sem_t space; //信号量，表示缓冲区剩下多少可写区块
sem_t doing; //信号量，表示有线程正在对缓冲区进行读/写
sem_t content; //信号量，表示缓冲区内的内容达到2，可以进行一次读取

int Buf[N] = {0}; //环形缓冲区
int in = 0; //环形缓冲区写位置
int out = 0; //环形缓冲区读位置

//环形缓冲区写操作
void put(int a){
	Buf[in] = a;
	if(in + 1 >= N)sem_post(&content); //如果缓冲区内的数据满足一次读取，则增加content信号量的值表明可以读取
	in = (++in)%N; //环形置数
}
//环形缓冲区读操作
int take(){
	int a = Buf[out]; 
	out = (++out)%N; //环形取数
	return a;
}

//生产者1
void* producer1(){
	FILE *fp;
    fp = fopen("./1.dat", "rb"); //读取1.dat中的内容
	for(int i = 0; i < 10; ++i){
		sem_wait(&space); //等待环形缓冲区中有剩余可写位置
		sem_wait(&doing); //等待无人在环形缓冲区中进行读写操作
		put(getw(fp)); //从文件中读取一个数放在环形缓冲区中
		sem_post(&doing); //增加doing信号量的值表明在缓冲区中的读写操作已经完成
	}
	fclose(fp);
}
//生产者2
void* producer2(){
	FILE *fp;
    fp = fopen("./2.dat", "rb"); //读取2.dat中的内容
	for(int i = 0; i < 10; ++i){
		sem_wait(&space); //等待环形缓冲区中有剩余可写位置
		sem_wait(&doing); //等待无人在环形缓冲区中进行读写操作
		put(getw(fp)); //从文件中读取一个数放在环形缓冲区中
		sem_post(&doing); //增加doing信号量的值表明在缓冲区中的读写操作已经完成
	}
	fclose(fp);
}

//消费者1
void* consumer1(){
	int a, b;
	for(int i = 0; i < 5; ++i){
		sem_wait(&content); //等待环形缓冲区中的内容足够一次读取
		sem_wait(&doing); //等待无人在环形缓冲区中进行读写操作
		a = take(); //从环形缓冲区中取一个数作为第一操作数
		b = take(); //从环形缓冲区中再取一个数作为第二操作数
		printf("%d + %d = %d\n",a, b, a+b); //计算两操作数之和并打印
		sem_post(&doing); //增加doing信号量的值表明在缓冲区中的读写操作已经完成
		for(int j = 0; j < 2; ++j)sem_post(&space); //表示环形缓冲区中已取两数，新增2可写位置
	}
}

//消费者2
void* consumer2(){
	int a, b;
	for(int i = 0; i < 5; ++i){
		sem_wait(&content); //等待环形缓冲区中的内容足够一次读取
		sem_wait(&doing); //等待无人在环形缓冲区中进行读写操作
		a = take(); //从环形缓冲区中取一个数作为第一操作数
		b = take(); //从环形缓冲区中再取一个数作为第二操作数
		printf("%d * %d = %d\n",a, b, a*b); //计算两操作数之积并打印
		sem_post(&doing); //增加doing信号量的值表明在缓冲区中的读写操作已经完成
		for(int j = 0; j < 2; ++j)sem_post(&space); //表示环形缓冲区中已取两数，新增2可写位置
	}
}

int main(){
	sem_init(&space, 0, N); //初始化信号量space为缓冲区长度，开始时缓冲区中有2可写位置
	sem_init(&doing, 0, 1); //初始化信号量doing为1，表示同一时刻只能由1个线程对缓冲区进行读写操作
	sem_init(&content, 0, 0); //初始化信号量content为0，开始时缓冲区中还未放置数据

	//将数组的数据存入文件
	FILE *fp1;FILE *fp2;
    int num1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; //待存入文件的数组1
	int num2[10] = {-1, -2, -3, -4, -5, -6, -7, -8, -9, -10}; //待存入文件的数组2
    fp1 = fopen("./1.dat", "wb");
	fp2 = fopen("./2.dat", "wb");
	for(int i = 0; i < 10; ++i){
		putw(num1[i],fp1);
		putw(num2[i],fp2);
	}
    fclose(fp1);
	fclose(fp2);

	//分别创建两个生产者线程和两个消费者线程
	pthread_t p1, p2, c1, c2; //定义线程执行体
	pthread_create(&p1,NULL,producer1,NULL); //创建p1
	pthread_create(&p2,NULL,producer2,NULL); //创建p2
	pthread_create(&c1,NULL,consumer1,NULL); //创建c1
	pthread_create(&c2,NULL,consumer2,NULL); //创建c2

	//等待四个子线程都执行结束后再退出
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(c1,NULL);
	pthread_join(c2,NULL);
    return 0;
}
