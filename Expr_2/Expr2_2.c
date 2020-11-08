#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
    pid_t pid;int count = 0;
    pid = fork(); //创建子进程b
    if(pid > 0)pid = fork(); //创建子进程c
    else count++; 

    if(pid > 0){
        printf("Father\tgetpid()=%d\tgetppid()=%d\n",getpid(),getppid()); //父进程输出“Father”和当前进程pid
    }else{
        if(count == 1)printf("Child\tgetpid()=%d\tgetppid()=%d\n",getpid(),getppid());//子进程输出“Child”、当前进程的pid和父进程pid
        else printf("Child\tgetpid()=%d\tgetppid()=%d\n",getpid(),getppid());//子进程输出“Child”、当前进程的pid和父进程pid
    }
    return 0;
}