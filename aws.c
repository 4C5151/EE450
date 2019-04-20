//EE450
//Quanquan Lu
//  11/13/2017
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define AWS_TCP_PORT 25160
#define AWS_UDP_PORT 24160
#define A_PORT 21160
#define B_PORT 22160
#define C_PORT 23160

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     char funct[3];
     char value[10];
     float p1,p2,p3,p4,p5,p6;
     float result;
//*********************************************** TCP ********************************************************
//********************This part is modified from http://www.linuxhowtos.org/data/6/server.c
     struct sockaddr_in serv_addr, cli_addr;
     int n_tcp;
     printf("The AWS is up and running.\n");
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = AWS_TCP_PORT;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
//-------------------------------------------get the value and function-------------------------------------------------
     bzero(buffer,256);
     n_tcp = read(newsockfd,buffer,3);
     if (n_tcp < 0) error("ERROR reading from socket");
     strcpy(funct,buffer);
     bzero(buffer,256);
     n_tcp = read(newsockfd,buffer,10);
     if (n_tcp < 0) error("ERROR reading from socket");
     strcpy(value,buffer);
     if (strcmp(funct,"LOG") ==1 )
       printf("The AWS received input <%s> and function=LOG from the client using TCP over port %d  \n",value, portno);
    else
        printf("The AWS received input <%s> and function=DIV from the client using TCP over port %d  \n",value,portno);
     p1=atof(value);



//-------------------------------UDP--------------------------------------------------------
//**********************This part is modified from http://www.linuxhowtos.org/data/6/client_udp.c
int sock_udp, n;
unsigned int length;
struct sockaddr_in serverA, fromA, serverB, fromB, serverC, fromC;
struct hostent *hp;
char buffer_udp[20];

//printf("AWS is sending!\n");
sock_udp= socket(AF_INET, SOCK_DGRAM, 0);
if (sock_udp < 0) error("socket\n");
serverA.sin_family = AF_INET;
serverB.sin_family = AF_INET;
serverC.sin_family = AF_INET;

hp = gethostbyname("localhost");
if (hp==0) error("Unknown host");

bcopy((char *)hp->h_addr, 
        (char *)&serverA.sin_addr,
         hp->h_length);
serverA.sin_port = htons(A_PORT);

bcopy((char *)hp->h_addr, 
        (char *)&serverB.sin_addr,
         hp->h_length);
serverB.sin_port = htons(B_PORT);

bcopy((char *)hp->h_addr, 
        (char *)&serverC.sin_addr,
         hp->h_length);
serverC.sin_port = htons(C_PORT);

length=sizeof(struct sockaddr_in);


//-------------------------------UDPA to get X2-----------------------------------------
n=sendto(sock_udp,value,
            strlen(value),0,(const struct sockaddr *)&serverA,length);
if (n < 0 ) error("sendtoA");
printf("The AWS sent <%s> to Backend-Server A\n", value);

n = recvfrom(sock_udp,buffer_udp,20,0,(struct sockaddr *)&fromA, &length);
   if (n < 0) error("recvfrom");
    p2=atof(buffer_udp);
    
//-------------------------------UDPB  to get X3-----------------------------------------

n=sendto(sock_udp,value,
            strlen(value),0,(const struct sockaddr *)&serverB,length);
if (n < 0 ) error("sendtoB");
printf("The AWS sent <%s> to Backend-Server B\n", value);

   n = recvfrom(sock_udp,buffer_udp,20,0,(struct sockaddr *)&fromB, &length);
   if (n < 0) error("recvfrom");
   p3=atof(buffer_udp);
//-------------------------------UDPC to get X5-----------------------------------------

n=sendto(sock_udp,value,
            strlen(value),0,(const struct sockaddr *)&serverC,length);
if (n < 0 ) error("sendtoC");
printf("The AWS sent <%s> to Backend-Server C\n", value);
n = recvfrom(sock_udp,buffer_udp,20,0,(struct sockaddr *)&fromC, &length);
   if (n < 0) error("recvfrom");
   p5=atof(buffer_udp);

//-----------------------------------------------------------------end sending

    printf("The AWS received  <%g> Backend-Server <A> using UDP port over <%d>\n", p2, AWS_UDP_PORT);
    printf("The AWS received  <%g> Backend-Server <B> using UDP port over <%d>\n", p3, AWS_UDP_PORT);
    printf("The AWS received  <%g> Backend-Server <C> using UDP port over <%d>\n", p5, AWS_UDP_PORT);
//-------------------------------UDPA to get X4-----------------------------------------
char v4[10];
sprintf(v4,"%g",p2);
n=sendto(sock_udp,v4,
            strlen(v4),0,(const struct sockaddr *)&serverA,length);
if (n < 0 ) error("sendtoA");
printf("The AWS sent <%s> to Backend-Server A\n", v4);
n = recvfrom(sock_udp,buffer_udp,20,0,(struct sockaddr *)&fromA, &length);
   if (n < 0) error("recvfrom");
   p4=atof(buffer_udp);
//-------------------------------UDPB to get X6-----------------------------------------
char v6[10];
sprintf(v6,"%g",p2);
n=sendto(sock_udp,v6,
            strlen(v6),0,(const struct sockaddr *)&serverB,length);
if (n < 0 ) error("sendtoB");
printf("The AWS sent <%s> to Backend-Server B\n", v6);
n = recvfrom(sock_udp,buffer_udp,20,0,(struct sockaddr *)&fromB, &length);
   if (n < 0) error("recvfrom");
   p6=atof(buffer_udp);
//------------------------------------------------------end sending----------------
printf("The AWS received  <%g> Backend-Server <A> using UDP port over <%d>\n", p4, AWS_UDP_PORT);
printf("The AWS received  <%g> Backend-Server <B> using UDP port over <%d>\n", p6, AWS_UDP_PORT);
printf("Values from powers received by AWS: <%g,%g,%g,%g,%g,%g>\n",p1,p2,p3,p4,p5,p6);
//--------------------Compute Unit-----------------------------------------------
if (strcmp(funct,"LOG") ==1 )
{
   result=-p1-p2/2-p3/3-p4/4-p5/5-p6/6;  
   printf("AWS calculated LOG on <%g>: <%f>\n",p1,result);
}
else
{
   result=1+p1+p2+p3+p4+p5+p6;
   printf("AWS calculated DIV on <%g>: <%f>\n",p1,result);
}
//--------------------------end computing-------------------------------------

//-----------------------------send it back to client-----------------------------
    char reserve[10];
    sprintf(reserve,"%g",result);
    n_tcp = write(newsockfd,reserve,sizeof(reserve));
    if (n_tcp < 0) error("ERROR writing to socket");
    printf("The AWS sent <%f> to client\n",result);
     close(newsockfd);
     close(sockfd);
     close(sock_udp);
     return 0; 
}
