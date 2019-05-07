#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#define SERV_PORT 9527

int main()
{
	int sfd, cfd;
	int len, i;
	char buf[BUFSIZ], clie_IP[BUFSIZ];
	struct sockaddr_in serv_addr, clie_addr;
	socklen_t clie_addr_len;

	//创建socket，指定ipv4协议族 tcp协议
	sfd = socket(AF_INET, SOCK_STREAM, 0);

	//初始化一个addr man 7 ip查看对应信息
	bzero(&serv_addr, sizeof(serv_addr));        //将整个结构体清零
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    //监听本地所有IP地址
	serv_addr.sin_port = htons(SERV_PORT);

	bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));  //绑定服务器

	/*设定链接上限,注意此处不阻塞*/
	listen(sfd, 64);      //同一时刻允许向服务器发起链接请求的数量

	printf("wait for client connect...\n");

	clie_addr_len = sizeof(clie_addr);
	cfd = accept(sfd, (struct sockaddr*)&clie_addr, &clie_addr_len);    /*监听客户端链接, 会阻塞*/

	printf("client IP:%s\tport:%d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)),
			ntohs(clie_addr.sin_port));
	
	while (1) {
        /*读取客户端发送数据*/
        len = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);

        /*处理客户端数据*/
        for (i = 0; i < len; i++)
            buf[i] = toupper(buf[i]);

        /*处理完数据回写给客户端*/
        write(cfd, buf, len); 
    }

	/*关闭链接*/
    close(sfd);
    close(cfd);

	return 0;


}

