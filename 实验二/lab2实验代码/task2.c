#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int execl(const char* path,const char* arg,...); 

int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
		perror ("fork");
	else if(pid == 0){ // 子进程
        	printf ("I am the baby!\n");
        	int ret;
        	ret = execl("/usr/bin/vi","vi","/Desktop/OS/lab2/text.txt",NULL);
        	if (ret == -1) 
        	{
            		perror ("execl");
        	}
	}else if(pid > 0){ // 父进程
        	printf ("I am the parent of pid=%d!\n", pid);
		while(1){
			sleep(1);
		}
	}
	
	return 0;
}

