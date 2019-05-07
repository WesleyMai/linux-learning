#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

void catch_signal(int signo)
{
	;
}

unsigned int mysleep(unsigned int seconds) 
{
	int ret;
	struct sigaction act, oldact;

    act.sa_handler = catch_signal;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    ret =  sigaction(SIGALRM, &act, &oldact);
	if(ret == -1) {
		perror("sigaction error");
		exit(1);
	}
	
	alarm(seconds);
	ret = pause();
	if(ret == -1 && errno == EINTR) 
		printf("pause success");
	
	ret = alarm(0);
	sigaction(SIGALRM, &oldact, NULL);   //恢复SIGALRM信号旧有处理方式
	return ret;
}

int main()
{
	while(1) {
		mysleep(3);
		printf("-----------\n");
	}
	return 0;
}
