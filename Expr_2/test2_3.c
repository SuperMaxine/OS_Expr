#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
    pid_t pid;int count = 0;
    pid = fork(); //创建子进程b
    if(pid > 0)pid = fork(); //创建子进程c
    else count++; 

    if(pid > 0){
        for(int i = 0; i < 1000; ++i)printf("a"); //父进程输出a
    }else{
        if(count == 1)for(int i = 0; i < 1000; ++i)printf("b");//子进程b输出b
        else for(int i = 0; i < 1000; ++i)printf("c");//子进程c输出c
    }
    return 0;
}

/*
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
    pid_t pid;int count = 0;
    pid = fork(); //创建子进程b
    if(pid > 0)pid = fork(); //创建子进程c
    else count++; 

    if(pid > 0){
        printf("a"); //父进程输出a
        printf("a"); //父进程输出a
        printf("a"); //父进程输出a
        printf("a"); //父进程输出a
        printf("a"); //父进程输出a
        printf("a"); //父进程输出a
        printf("a"); //父进程输出a
        printf("a"); //父进程输出a
        printf("a"); //父进程输出a
        printf("a"); //父进程输出a
    }else{
        if(count == 1){
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
            printf("b");//子进程b输出b
        }
        else {
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
            printf("c");//子进程c输出c
        }
    }
    return 0;
}
*/
