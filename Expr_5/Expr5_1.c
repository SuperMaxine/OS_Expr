#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>

int open_queue(int keyval) 
{
int qid;
qid = msgget(keyval, IPC_CREAT | 0666);
    if (qid == -1) {
        perror("Failed in calling msgget");
        return (-1);
    }
    return (qid);
}


struct msg_st
{
    long int msg_type;
    char text[100];
};

int main(){
    int msgid = -1;
    msgid = open_queue(1234);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed width error\n");
        exit(EXIT_FAILURE);
    }
    //子进程1
    if(fork() == 0){
        struct msg_st b;
        long int msgtype = 0;
        b.msg_type = 1;
        while(1){
            if(msgrcv(msgid, (void *)&b, 100, msgtype, 0) == -1){
                // printf("err\n");
            }else{
                if(!strcmp(b.text, "exit"))break;
                else printf("The message is %s\n", b.text);
            }
        }
        exit(0);
    }else{
        //子进程2
        if(fork() > 0)wait(NULL);
        else{
            int msgid = msgget((key_t)1234, IPC_CREAT|0666);
            struct msg_st a;
            a.msg_type = 1;
            scanf("%s",a.text);
            while(1){
                msgsnd(msgid, (void *)&a, 100, 0);
                scanf("%s",a.text);
                // printf("input %s", a.text);
                if(!strcmp(a.text, "exit"))break;
            }
            exit(0);
        }
    }
    // wait(NULL);
    return 0;
}