#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> 
#include <sys/wait.h> 
#include <syslog.h>
int main(void )
{
    char *buff = "This is a Daemon\n";
    pid_t pid;
    int i,fd;
    if((pid = fork()) < 0){
        printf("fork error\n");
        exit(1);
    }
    if(pid > 0){
        exit(0);
    }
	else{
        setsid();
        chdir("/");
        umask(0);
        for(i=0; i< getdtablesize(); i++)
            close(i);
        while(1){
        if((fd = open("/tmp/daemon.log",O_CREAT|O_WRONLY|O_APPEND,0600)) < 0){
            syslog(LOG_DEBUG,"Open file error\n");
            exit(1);
        } 
        syslog(LOG_EMERG, "syssysysysysysy\n");        
        write(fd,buff,strlen(buff) + 1);
        close(fd);
        sleep(2);
        }
    }
	exit(0);
}




































