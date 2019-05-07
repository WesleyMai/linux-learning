/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年04月30日 19时22分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
 
struct production {
    int num;
    struct production *next;
};                  
 
struct production *head = NULL;  //定义全局指针head
struct production *rer = NULL;   //定义全局指针rer
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //静态初始化
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  //静态初始化
 
void *productor( void *arg )    //生产者
{
        srand( time( NULL ) );
 
        while(1){
            pthread_mutex_lock( &mutex );
            rer = (struct production *)malloc( sizeof(struct production) );
            rer->num = (rand( )%400 + 1);
            rer->next = head;
            head = rer;
            printf("--------The production is %d.\n",rer->num);
            pthread_mutex_unlock( &mutex );
 
            pthread_cond_signal( &cond );  //唤醒因wait阻塞的线程
 
            sleep( rand( )%3 );
        }
 
 
        return NULL;
}
 
void *consumer( void *arg )  //消费者
{
        srand( time( NULL ) );
 
        while(1){
            pthread_mutex_lock( &mutex );
            while( head == NULL )   //注意不能是if，必须采用循环
                pthread_cond_wait( &cond, &mutex );
            rer = head;
            head = rer->next;
            printf("++++++++The consumer is %d.\n",rer->num);
            free(rer);
			pthread_mutex_unlock( &mutex );
            sleep( rand( )%3 );
        }
 
        return NULL;
}
 
int main( void )
{
        pthread_t pid, cid;
        int ret;
 
        ret = pthread_create( &pid, NULL, productor, NULL);
        if( ret != 0)
        {
            fprintf(stderr,"pthread_create error1: %s\n",strerror(ret));
            exit(1);
        }
        ret = pthread_create( &cid, NULL, consumer, NULL);
        if( ret != 0)
        {
            fprintf(stderr,"pthread_create error2: %s\n",strerror(ret));
            exit(1);
        }
 
        ret = pthread_join(pid,NULL);
        if( ret != 0)
        {
            fprintf(stderr,"pthread_join error1: %s\n",strerror(ret));
            exit(1);
        }
 
        ret = pthread_join(cid,NULL);
        if( ret != 0)
        {
            fprintf(stderr,"pthread_join error2: %s\n",strerror(ret));
            exit(1);
        }
 
        pthread_cond_destroy(&cond);         //销毁条件变量
        pthread_mutex_destroy(&mutex);       //销毁互斥锁
 
        return 0;
}

