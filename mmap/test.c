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
	int fd,i=0;
	char *start;
	char *buf = "It is a mmap test";
	fd = open("/dev/globalmem",O_RDWR);
	start = mmap(NULL,BUFFSIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    for(i=0;i<BUFFSIZE;i++)
    {
        if(start[i]!=0)
        printf("start = %c ",start[i]);
    }
    
	munmap(start,BUFFSIZE);
    //sleep(10);
	close(fd);
	return 0;
}




































