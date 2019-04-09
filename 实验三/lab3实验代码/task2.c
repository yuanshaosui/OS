#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
int pthread_yield(void);
int ticketCount = 1000;
sem_t *mySem = NULL;
void *ticket(void *arg) {	//售票
   int temp;
   while(1){
	if(ticketCount>0){
		sem_wait(mySem);
		temp=ticketCount;
		pthread_yield();
		temp=temp-1;
		pthread_yield();
		ticketCount=temp;		
		sleep(1);
		printf("售票1张,现有票数：%d!\n",ticketCount);
		sem_post(mySem);
		}	
	}
   return NULL;
}
void *refund(void *arg) {	//退票
   int temp;
   while(1){
	if(ticketCount<1000){
		sem_wait(mySem);
		temp=ticketCount;
		pthread_yield();
		temp=temp+1;
		pthread_yield();
		ticketCount=temp;		
		sleep(1);
		printf("退票1张,现有票数：%d!\n",ticketCount);
		sem_post(mySem);
		}	
	}
   return NULL;
}
int main(void)
{
   pthread_t p1, p2;
   printf("现有票数：%d张,售票退票线程开始!\n",ticketCount);
   mySem = sem_open("mySem", O_CREAT, 0666, 1); 
   pthread_create(&p1, NULL, ticket, NULL);
   pthread_create(&p2, NULL, refund, NULL);	
   pthread_join(p1, NULL);
   pthread_join(p2, NULL);
   sem_close(mySem);//线程结束
   sem_unlink("mySem");//主程序结尾
   return 0;
}

