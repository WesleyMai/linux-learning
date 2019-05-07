/*
 * =====================================================================================
 *
 *       Filename:  a.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年04月22日 13时34分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void printed(sigset_t *ped)
{
	int i;
	for(i = 1; i < 32; i++) {
		if(sigismember(ped, i) == 1) {
			putchar('1');
				
		}
		else
			putchar('0');
	
	}
	printf("\n");


}

int main()
{
	sigset_t myset, oldset, ped;
	sigemptyset(&myset);
	sigaddset(&myset, SIGQUIT);
	sigaddset(&myset, SIGINT);
	sigaddset(&myset, SIGTSTP);

	sigprocmask(SIG_BLOCK, &myset, &oldset);

	while(1) {
		sigpending(&ped);
		printed(&ped);
		sleep(1);
	
	}
	return 0;

}
