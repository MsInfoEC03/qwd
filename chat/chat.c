#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <signal.h>

#define MAXLINE 4096
int    sockfd;
struct sockaddr_in     servaddr,broad;;
int addrlen = sizeof(servaddr);
char noton[255]="online: ";
char notoff[255]="offline: ";
char up[255]="update: ";
char    ipaddr[20];
char    buff[MAXLINE];
static int num=1;
static void sig_usr(int signo)
{
    if(signo == SIGINT){
        servaddr.sin_addr.s_addr = inet_addr("255.255.255.255");  //这是对局域网全网段  
        sendto(sockfd,notoff,sizeof(notoff),0,(struct sockaddr *)&servaddr,addrlen);
        close(sockfd); 
        exit(0);
    }
}
struct msg             
{
    char ip[20];
    char name[20];
};

struct msg people[20]={0};
void message()
{  
    int i=0,len=0,date=0;
    if(((buff[0]=='o')&&(buff[1]=='n')&&(buff[6]==':'))||((buff[0]=='u')&&(buff[1]=='p')&&(buff[2]=='d')))
    {
        date=num;
        for(i=0;i<num;i++)
        {
            if(people[i].name[0] == '\0')
            {  date=i; break;  }
        }
        if(date == num) num++;

        for(i=7;buff[i]!='\0';i++)
        {
            people[date].name[len]=buff[i];
            len++;
        }

        people[date].name[len]='\0';    
        len=0;
        for(i=0;i<13;i++)
        {
            people[date].ip[len]=ipaddr[i];
            len++;
        }
        people[date].ip[len]='\0';      
    }

    if((buff[0]=='o')&&(buff[1]=='f')&&(buff[2]=='f')&&(buff[7]==':'))
    {
        for(i=0;i<num;i++)
        {
            if((people[i].ip[10]==ipaddr[10])&&(people[i].ip[11]==ipaddr[11]))
            {
                memset(people[i].name,'\0',sizeof(people[i].name));
                memset(people[i].ip,'\0',sizeof(people[i].ip));
            }
        }
    }

    if(((buff[0]==':')&&(buff[1]=='l')&&(buff[2]=='s'))&&(strcmp(people[0].ip,ipaddr)==0))
    {
        for(i=1;i<num;i++)
        {
            if(people[i].name[0] != '\0')
            printf("%-10s : %-13s\n",people[i].name,people[i].ip);
        }
    }
}
int main(int argc, char** argv)
{ 
   
    
    int  n;  
    int flag = 1;                                //0表示关闭属性,非0表示打开属性 
    pid_t pid;
    if((pid=fork())<0)
        printf("fork error\n");
    strcat(noton,argv[1]);                                           //名字拼接
    strcat(notoff,argv[1]);
    strcat(up,argv[1]);
    if(pid>0)
    {
        unsigned char count = 0,i=0;                                    //记录在线人数
        
        sleep(1);
        char msg[10][255];
        //服务端
        if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
        }
        
        setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));  //广播设置  
        
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(6666); 

        if( bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
        }
        
        printf("======waiting for client's request======\n");

        while(1){
            if((n = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&servaddr, &addrlen)) > 0)
			{
                buff[n] = '\0';
                /********************************收到上线通知,发送自己名字*******************************/
                if(buff[0]=='o'&&buff[1]=='n'&&buff[2]=='l'&&buff[3]=='i'&&buff[4]=='n'&&buff[5]=='e'){
                    servaddr.sin_port = htons(6666); 
                    if( sendto(sockfd,up, strlen(up), 0, (struct sockaddr*)&servaddr, addrlen) == -1){
                    printf("sendto msg error: %s(errno: %d)\n", strerror(errno), errno);
                    exit(0);
                    }
                }
                inet_ntop(AF_INET,&servaddr.sin_addr,ipaddr,sizeof(ipaddr));
                message();
                printf("%s : %s \n",ipaddr, buff);
			}
        }
        close(sockfd);  
    }else if(pid==0)
    {
        if(signal(SIGINT,sig_usr) == SIG_ERR)
        printf("can't catch SIGINT\n");
        sleep(1);    
        //UDP 客户端
        printf("======waiting for service's request======\n");
        if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        exit(0);
        }
        setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));  //广播设置  
        
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;  
        servaddr.sin_port = htons(6666); 

        servaddr.sin_addr.s_addr = inet_addr("255.255.255.255");            //这是对局域网全网段   
        sendto(sockfd,noton,sizeof(noton),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
         while(1){
            fgets(buff, MAXLINE, stdin);
            if(buff[0]==':'){
                /***************************更改ip*******************************/
                if(buff[4]=='.'&&buff[8]=='.'&&buff[10]=='.'&&buff[15]=='\0'){
                    strncpy(ipaddr,buff+1,13);
                    if( inet_pton(AF_INET, ipaddr, &servaddr.sin_addr) == -1){
                        printf("inet_pton error for %s\n",ipaddr);
                        exit(0);
                    }
                    continue;
                }
                /****************************广播********************************/
                if(buff[1]=='a'&&buff[2]=='l'&&buff[3]=='l'&&buff[5]=='\0'){
                    servaddr.sin_addr.s_addr = inet_addr("255.255.255.255");  //这是对局域网全网段 
                    continue;
                }
            }
			if( sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&servaddr, addrlen) == -1){
				printf("sendto msg error: %s(errno: %d)\n", strerror(errno), errno);
				exit(0);
			}
        }
        close(sockfd);
        exit(0);
    }  
}
