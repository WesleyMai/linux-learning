/*
 * =====================================================================================
 *
 *       Filename:  mmap_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年04月20日 12时48分14秒
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
#include <string.h>
#include <sys/mman.h>

int main()
{
	int len, ret;
	char *p = NULL;
	int fd = open("mytest.txt", O_CREAT|O_RDWR, 0644);
	if(fd < 0) {
		perror("open error");
		exit(1);
	}
	len = ftruncate(fd, 4);
	if(len == -1) {
		perror("ftruncate error");
		exit(1);
	}
	
	p = mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(p == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	strcpy(p, "abc");
	ret = munmap(p, 4);
	if(ret == -1) {
		perror("munmap error");
		exit(1);
	}
    close(fd);
	return 0;
}



