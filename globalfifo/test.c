#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h> 
#define BUFFSIZE 4096
int main(void )
{
	int fd,n;
    char buf[11] = "hello world";
    char buff[11];
    pid_t pid;
	fd = open("/dev/globalmem",O_RDWR);
	if(fd<0)
        printf("open error\n");
    if((pid = fork()) < 0){
        printf("fork error\n");
    }
    if(pid == 0){
        while(1){
            printf("------------------\n");
            /*if(write(fd,buf,sizeof(buf))!=sizeof(buf))
                printf("write error\n");*/
        }
    }
	else{
        while(1){
            printf("******************\n");
            if((n=read(fd,buff,strlen(buf)))>0)
                printf("%s\n",buff);
            if(n<0)
                printf("read error\n");
          
        }
    }
	close(fd);
	return 0;
}




































