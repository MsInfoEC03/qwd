#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/ioctl.h>
#define BUFFSIZE 1024
#define MEM_CLEAR 0x1
int main()
{
    int fd,n,pos;
    char buf[BUFFSIZE];
    if((fd =  open("/dev/globalmem",O_RDWR)) == -1)
    printf("globalmem open error\n");
    else
    printf("globalmem is opened,fd is %d\n",fd);
    while((n = read(fd,buf,BUFFSIZE))>0)
        if(write(STDOUT_FILENO,buf,n)!=n)
	    printf("write error\n");
    if(n < 0)
    printf("read error\n");
    if((pos=lseek(fd,5,SEEK_SET)) == -1)
    printf("can't seek\n");
    ioctl(fd,MEM_CLEAR,0);
    while((n = read(fd,buf,BUFFSIZE))>0)
	if(write(STDOUT_FILENO,buf,n)!=n)
	    printf("write error\n");
    if(n < 0)
    printf("read error");
    close(fd);
    return 0;
}
