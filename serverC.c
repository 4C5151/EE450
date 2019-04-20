#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#define C_PORT 23160

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];
   char value[10];
   float bar;
//********************************UDP****************************************
//This part is modified from http://www.linuxhowtos.org/data/6/server_udp.c
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(C_PORT);
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   printf("The Server C is up and running using UDP on port %d.\n",htons(server.sin_port));
   while(1)
{
//----------------------------------receive the value and do the computation x*x*x*x*x----------
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       strcpy(value, buf);
       bar=atof(value);
       printf("The Server C received input <%g>\n", bar);
       bar=bar*bar*bar*bar*bar;      // X to the power 5
       printf("The Server C calculated 5th power: <%g>\n", bar);
       sprintf(value,"%g",bar);
//----------------------------------send it back to AWS------------------------------------------
       n = sendto(sock,value,sizeof(value),
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");
	printf("The Server C finished sending the output to AWS\n");
	bzero(buf,1024);
}
   return 0;
 }

