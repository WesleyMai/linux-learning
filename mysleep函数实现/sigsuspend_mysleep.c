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
	sigset_t newmask, oldmask, suspmask;
	unsigned int unslept;

    act.sa_handler = catch_signal;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    ret =  sigaction(SIGALRM, &act, &oldact);
	if(ret == -1) {
		perror("sigaction error");
		exit(1);
	}
	
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);


	alarm(seconds);

	suspmask = oldmask;
	sigdelset(&suspmask, SIGALRM);  //确保这个信号一定解除了屏蔽

	//ret = pause();
	//if(ret == -1 && errno == EINTR) 
	//	printf("pause success");
	sigsuspend(&suspmask);


	unslept = alarm(0);
	sigaction(SIGALRM, &oldact, NULL);   //恢复SIGALRM信号旧有处理方式

	sigprocmask(SIG_SETMASK, &oldmask, NULL);  //恢复默认信号屏蔽字
	return unslept;
}

int main()
{
	while(1) {
		mysleep(3);
		printf("-----------\n");
	}
	return 0;
}
