#include<stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include "semaphore.h"

#define N 2

int Buf[N] = {0};
int in = 0;
int out = 0;

void put(int a){
	Buf[in] = a;
	in = (++in)%N;
}

int take(){
	int a = Buf[out];
	out = (++out)%N;
	return a;
}


sem_t space;
sem_t doing;
sem_t content;

void producer1(){
	FILE *fp;
    fp = fopen("./1.dat", "rb");
	for(int i = 0; i < 10; ++i){
		printf("%d\n",getw(fp));
	}
	fclose(fp);
}

void producer2(){
	FILE *fp;
    fp = fopen("./2.dat", "rb");
	for(int i = 0; i < 10; ++i){
		printf("%d\n",getw(fp));
	}
	fclose(fp);
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

	producer1();
	producer2();

    return 0;
}
