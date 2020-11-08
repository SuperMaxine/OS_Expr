#include<stdio.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <unistd.h>

int main(){
    pid_t pid;int count = 0;
    int fp[2];
    pipe(fp);
    pid = fork();
    if(pid > 0)pid = fork();
    else count++;

    if(pid > 0){
        wait(NULL);
        close(fp[1]); //父进程从管道中读数据，所以关闭写端
        char buf[200];
        read(fp[0],buf,58);
        printf("%s\n",buf);
        // read(fp[0], buf, 29);
        // printf("%s\n",buf);
        // read(fp[0], buf, 29);
        // printf("%s\n",buf);
    }else{
        close(fp[0]); //子进程从管道中写数据，所以关闭读端
        if(count > 0){
            char buf[200] = "Child 1 is sending a message!";
            write(fp[1], buf, 29);
        }else{
            char buf[200] = "Child 2 is sending a message!";
            write(fp[1], buf, 29);
        }
    }
}