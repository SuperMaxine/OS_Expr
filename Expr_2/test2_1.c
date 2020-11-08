#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
    pid_t pid;int count = 0;
    pid = fork(); //创建子进程b
    if(pid > 0)pid = fork(); //创建子进程c
    else count++; 

    if(pid > 0){
        printf("a\n"); //父进程输出a
    }else{
        if(count == 1)printf("b\n");//子进程b输出b
        else printf("c\n");//子进程c输出c
    }
    return 0;
}