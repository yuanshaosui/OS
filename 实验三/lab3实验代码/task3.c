#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

char buffer[10];
int i=0,j=0;
sem_t *full = NULL;
sem_t *empty = NULL;
void *worker1(void *arg) {	//生产者
   while(1){
	sem_wait(empty);
	scanf("%c",&buffer[j]);j++;
	sleep(1);
	if(j>=10)j%=10;
	sem_post(full);
	}
   return NULL;
}
void *worker2(void *arg) {	//消费者
   while(1){
	sem_wait(full);
	printf("输出:%c\n",buffer[i]);i++;
	sleep(1);
	if(i>=10)i%=10;
	sem_post(empty);
	}
   return NULL;
}


int main(int argc, char *argv[])
{
   pthread_t p1, p2;
   full = sem_open("full", O_CREAT, 0666, 0); 
   empty = sem_open("empty", O_CREAT, 0666, 10); 
   pthread_create(&p1, NULL, worker1, NULL);
   pthread_create(&p2, NULL, worker2, NULL);
   pthread_join(p1, NULL);
   pthread_join(p2, NULL);
   sem_close(full);//线程结束
   sem_close(empty);//线程结束
   sem_unlink("full");//主程序结尾
   sem_unlink("empty");//主程序结尾
   return 0;
}
