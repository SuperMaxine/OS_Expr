#include<stdio.h>
#include<pthread.h>

void* myThread1(void* arg){
    char* c=(char*)arg;
    printf("%s\n",c);

    return "Thread1 exit";
}

void* myThread2(void* arg){
    int* c=(int*)arg;
    printf("%d\n",*c);

    return "Thread2 exit";
}

int main(){
    pthread_t tid1;pthread_t tid2; //定义线程执行体
    char message1[] = "T";int message2 = 2;
    pthread_create(&tid1,NULL,myThread1,message1); //创建myThread1
    pthread_create(&tid2,NULL,myThread2,&message2); //创捷myThread2

    void* status;
    pthread_join(tid1,&status); //主线程等待myThread1结束后退出，并获取myThread1中通过pthread_exit返回的返回值
    printf("%s\n",(char*)status);
    pthread_join(tid2,&status); //主线程等待myThread1结束后退出，并获取myThread2中通过函数返回值返回的返回值
    printf("%s\n",(char*)status);

    return 0;
}