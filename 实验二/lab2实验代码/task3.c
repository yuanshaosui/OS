#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t p1,p2,p3,p4,p5;
    int cnt=0;
    while((p1=fork())==-1);

    if(!p1){
	    printf("Node p1 pid is %d, it's parent pid %d.\n",getpid(),getppid());
	    while((p2=fork())==-1);
	    wait(0);
    	    if(!p2){
		    printf("Node p2 is p1's child with pid %d, it's parent pid %d.\n",getpid(),getppid());
	    	    while((p4=fork())==-1);
		    wait(0);
                    if(!p4){
			    printf("Node p4 is p2's child with pid %d, it's parent pid %d.\n",getpid(),getppid());
			    exit(0);
		           }             
                    while((p5=fork())==-1);
		    wait(0);
                    if(!p5){
                            printf("Node p5 is p2's child with pid %d, it's parent pid %d.\n",getpid(),getppid());
			    exit(0);
			   } 
		    exit(0);    
		   }   
	  while((p3=fork())==-1);
	  wait(0);
          if(!p3){
                  printf("Node p3 is p1's child with pid %d, it's parent pid %d.\n",getpid(),getppid());
	          exit(0);
		  }
	 exit(0);
	} 
    return 0;
}

