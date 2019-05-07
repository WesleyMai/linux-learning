/*
 * =====================================================================================
 *
 *       Filename:  mmap_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年04月20日 16时14分27秒
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
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

int var = 100;

int main()
{
	int *p;
	pid_t pid;

	int fd;
	fd = open("temp",O_RDWR|O_CREAT|O_TRUNC, 0644);
	if(fd < 0) {
		perror("open error");
		exit(1);
	}
	unlink("temp");
	ftruncate(fd, 4);

	p = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//p = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
	if(p == MAP_FAILED) {     //注意不是p == NULL
		perror("mmap error");
		exit(1);
	}
	close(fd);         //映射区建立完毕，即可关闭文件

	pid = fork();
	if(pid == 0) {
		*p = 2000;
		var = 1000;
		printf("child, *p = %d, var = %d\n",*p, var);
	}
	else {
		sleep(1);
		printf("child, *p = %d, var = %d\n",*p, var);
		wait(NULL);
		
		int ret = munmap(p, 4);
		if(ret == -1) {
			perror("munmap error");
			exit(1);
		
		}
	}

	return 0;
}














