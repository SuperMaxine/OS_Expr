#include<stdio.h>
#include<pthread.h>

pthread_mutex_t mutex; //互斥锁

void* myThread1(void* arg){
    int* a = (int*)arg;
    int b[10];

    for(int i = 0; i < 10; ++i)b[i] = a[i]; //拷贝数组

    //排序
    for(int i = 1; i <= 10; ++i){
        for(int j = 0; j < 10 - i; ++j){
            if(b[j] > b[j + 1]){
                int tmp = b[j];
                b[j] = b[j + 1];
                b[j + 1] = tmp;
            }
        }
    }

    for(int i = 0; i < 10; ++i){
        printf("Thread1:%d\n",b[i]);
    }

    return "Thread1 exit";
}

void* myThread2(void* arg){
    int* a=(int*)arg;
    int b[10];

    for(int i = 0; i < 10; ++i)b[i] = a[i]; //拷贝数组

    //排序
    for(int i = 1; i <= 10; ++i){
        for(int j = 0; j < 10 - i; ++j){
            if(b[j] < b[j + 1]){
                int tmp = b[j];
                b[j] = b[j + 1];
                b[j + 1] = tmp;
            }
        }
    }

    for(int i = 0; i < 10; ++i){
        printf("Thread2:%d\n",b[i]);
    };

    return "Thread2 exit";
}

int main(){
    pthread_t tid1;pthread_t tid2; //定义线程执行体
    int a[10] = {1,5,3,7,8,0,2,4,6,9};
    pthread_create(&tid1,NULL,myThread1,a); //创建myThread1
    pthread_create(&tid2,NULL,myThread2,a); //创捷myThread2

    void* status;
    pthread_join(tid1,&status); //主线程等待myThread1结束后退出，并获取myThread1中通过pthread_exit返回的返回值
    printf("%s\n",(char*)status);
    pthread_join(tid2,&status); //主线程等待myThread1结束后退出，并获取myThread2中通过函数返回值返回的返回值
    printf("%s\n",(char*)status);

    return 0;
}