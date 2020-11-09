#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>

// 开启消息队列函数
int open_queue(int keyval) 
{
int qid;
qid = msgget(keyval, IPC_CREAT | 0666);
    if (qid == -1) {
        perror("Failed in calling msgget");
        exit(EXIT_FAILURE);
    }
    return (qid);
}

// 定义消息队列数据结构
struct msg_st
{
    long int msg_type;
    char text[100];
};

int main(){
    // 开启消息队列
    int msgid = open_queue(1234);

    //子进程1
    if(fork() == 0){
        struct msg_st b;
        b.msg_type = 1; // 表示获取队列中第一个可用的消息

        // 循环接受消息队列中的消息
        while(1){
            if(msgrcv(msgid, (void *)&b, 100, 0, 0) == -1){
                // printf("err\n");
            }else{
                // 成功接受到消息
                if(!strcmp(b.text, "exit"))break; // 如果是“exit”则退出循环
                else printf("The message form other process is %s\n", b.text); // 否则打印消息
            }
        }
        exit(0); // 退出子进程
    }else{
        //子进程2
        if(fork() > 0)wait(NULL); // 为了使子进程中的scanf可以获得终端中的输入，主进程要阻塞等待子进程的结束
        else{
            struct msg_st a;
            a.msg_type = 1; // 表示获取队列中第一个可用的消息
            scanf("%s",a.text); // 从终端获取输入
            if(!strcmp(a.text, "exit"))exit(0); // 如果输入内容为“exit”则直接退出子进程
            while(1){
                msgsnd(msgid, (void *)&a, 100, 0); // 发送终端输入的内容到消息队列
                scanf("%s",a.text); // 从终端输入下一条消息
                // printf("input %s", a.text);
                if(!strcmp(a.text, "exit"))break; // 如果输入内容为“exit”则直接退出子进程
            }
            exit(0); // 退出子进程
        }
    }

    return 0;
}