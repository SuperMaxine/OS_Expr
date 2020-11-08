#include<stdio.h>
#include<pthread.h>

void* subThread(void* arg){
    char* c=(char*)arg;
    printf("This is %s \n",c);

    if(c=="Thread1")pthread_exit("Thread1 exit"); //如果是myThread1，则执行pthread_exit
    return "Thread2 exit";
}

int main(){
    pthread_t tid1;pthread_t tid2; //定义线程执行体
    pthread_create(&tid1,NULL,subThread,"Thread1"); //创建myThread1
    pthread_create(&tid2,NULL,subThread,"Thread2"); //创捷myThread2

    void* status;
    pthread_join(tid1,&status); //主线程等待myThread1结束后退出，并获取myThread1中通过pthread_exit返回的返回值
    printf("%s\n",(char*)status);
    pthread_join(tid2,&status); //主线程等待myThread1结束后退出，并获取myThread2中通过函数返回值返回的返回值
    printf("%s\n",(char*)status);

    return 0;
}