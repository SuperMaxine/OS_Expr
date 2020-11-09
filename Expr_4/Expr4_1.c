#include<stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include "semaphore.h"

#define N 2

sem_t space;
sem_t doing;
sem_t content;

int Buf[N] = {0};
int in = 0;
int out = 0;

void put(int a){
	Buf[in] = a;
	if(in + 1 >= N)sem_post(&content);
	in = (++in)%N;
}

int take(){
	int a = Buf[out];
	out = (++out)%N;
	return a;
}


void* producer1(){
	FILE *fp;
    fp = fopen("./1.dat", "rb");
	for(int i = 0; i < 10; ++i){
		sem_wait(&space);
		sem_wait(&doing);
		put(getw(fp));
		sem_post(&doing);
	}
	fclose(fp);
}

void* producer2(){
	FILE *fp;
    fp = fopen("./2.dat", "rb");
	for(int i = 0; i < 10; ++i){
		sem_wait(&space);
		sem_wait(&doing);
		put(getw(fp));
		sem_post(&doing);
	}
	fclose(fp);
}

void* consumer1(){
	int a, b;
	for(int i = 0; i < 5; ++i){
		sem_wait(&content);
		sem_wait(&doing);
		a = take();
		b = take();
		printf("%d + %d = %d\n",a, b, a+b);
		sem_post(&doing);
		sem_post(&space);
		sem_post(&space);
	}
}

void* consumer2(){
	int a, b;
	for(int i = 0; i < 5; ++i){
		sem_wait(&content);
		sem_wait(&doing);
		a = take();
		b = take();
		printf("%d * %d = %d\n",a, b, a*b);
		sem_post(&doing);
		sem_post(&space);
		sem_post(&space);
	}
}

int main(){
	sem_init(&space, 0, N);
	sem_init(&doing, 0, 1);
	sem_init(&content, 0, 0);

	FILE *fp1;FILE *fp2;
    int num1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int num2[10] = {-1, -2, -3, -4, -5, -6, -7, -8, -9, -10};

    fp1 = fopen("./1.dat", "wb");
	fp2 = fopen("./2.dat", "wb");

	for(int i = 0; i < 10; ++i){
		putw(num1[i],fp1);
		putw(num2[i],fp2);
	}

    fclose(fp1);
	fclose(fp2);

	pthread_t p1, p2, c1, c2; //定义线程执行体
	pthread_create(&p1,NULL,producer1,NULL); //创建p1
	pthread_create(&p2,NULL,producer2,NULL); //创建p2
	pthread_create(&c1,NULL,consumer1,NULL); //创建c1
	pthread_create(&c2,NULL,consumer2,NULL); //创建c2

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(c1,NULL);
	pthread_join(c2,NULL);
    return 0;
}
