#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
static void sig_usr(int signo)		/* argument is signal number */
{
	if (signo == SIGUSR1){
        printf("received SIGUSR1\n");
    }
    if (signo == SIGINT){
        printf("received SIGINT\n");
    }
else
	{
		printf("received signal %d\n", signo);
		// exit(1);
	}
}
int main(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
	{
		printf("can't catch SIGUSR1\n");
		exit(1);
	}
	for ( ; ; )
		pause();
}
