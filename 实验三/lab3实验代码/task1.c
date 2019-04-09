#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>

sem_t *mySem = NULL;	//进程2和进程3互斥信号量
sem_t *mySem2 = NULL;	//进程2结束信号量
sem_t *mySem3 = NULL;	//进程3结束信号量
void *createP2(void *arg) {	
   pid_t  p2;
   sem_wait(mySem);
   while((p2=fork())==-1);
   if(p2>0)
	printf("I am the process P2!\n");	
   sem_post(mySem);
   sem_post(mySem2);
   return NULL;
}
void *createP3(void *arg) {
   pid_t  p3;
   sem_wait(mySem);
   while((p3=fork())==-1);
   if(p3>0)
	printf("I am the process P3!\n");	
   sem_post(mySem);
   sem_post(mySem3);
   return NULL;
}
void *createP4(void *arg) {
   pid_t  p4;
   sem_wait(mySem2);
   sem_wait(mySem3);
   while((p4=fork())==-1);
   if(p4>0)
	printf("I am the process P4!\n");	
   sem_post(mySem2);
   sem_post(mySem3);
   return NULL;
}
int main(int argc, char* argv[])
{
   pid_t  p1;
   pthread_t pp2, pp3, pp4;
   while((p1=fork())==-1);
   if(p1>0){
	printf("I am the process P1!\n");
	mySem = sem_open("mySem", O_CREAT, 0666, 1); 
	mySem2 = sem_open("mySem2", O_CREAT, 0666, 0); 
	mySem3 = sem_open("mySem3", O_CREAT, 0666, 0); 
	pthread_create(&pp2, NULL, createP2, NULL);
	pthread_create(&pp3, NULL, createP3, NULL);
	pthread_create(&pp4, NULL, createP4, NULL);
	pthread_join(pp2, NULL);
	pthread_join(pp3, NULL);
	pthread_join(pp4, NULL);
	sem_close(mySem);//线程结束
	sem_close(mySem2);
	sem_close(mySem3);
	sem_unlink("mySem");
	sem_unlink("mySem2");
	sem_unlink("mySem3");
   }
   return 0;
}

