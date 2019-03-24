#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t p1,p2,p3,p4,p5;
    p1=fork();
    int i=0;
    if(p1<0)
    {
	 printf("Node is %d, error!\n",getpid());
    }
    if(p1==0)
    {
	p4=fork();
	if(p4>0)
	   p5=fork();
	while(1){
	printf("Node is pid %d, it's parent pid %d.\n",getpid(),getppid());
	sleep(1);
	i++;
	if(i>5)
		exit(0);
	}
    }
    if(p1>0)
    {
	p3=fork();
	while(1){
	printf("Node is pid %d, it's parent pid %d.\n",getpid(),getppid());
	sleep(1);}
    }
    return 0;
}
