//EE450
//Quanquan Lu
//11/12/2017
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define AWS_PORT 25160
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, n,check;
    struct sockaddr_in serv_addr, serv_addr_client;
    struct hostent *server;
    int clientaddr=sizeof(serv_addr_client);
    printf("The client is up and running\n");
    char buffer[256];
    char funct[3];
//--------------------------------------Check your input-----------------------------------------------------
    if (argc < 2) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    if (strcmp(argv[1],"LOG")==0 && strcmp(argv[1],"DIV")==0) 
{
       printf("The function field should be 'LOG' or 'DIV', the command should be ./client <funct>  <value>\n");
       exit(0);
}
//------------------You input should be 'LOG' or 'DIV','log' and 'div' doesn't work here-------
    strcpy(funct,argv[1]);
    for (int j=0; j<3; j++)
{
    if(funct[j] >= 'a')
    {
	printf("The function field should be 'LOG' or 'DIV', the command should be ./client <funct>  <value>\n");
        exit(0);
}
}
    if ((atof(argv[2])>=1) || (atof(argv[2])<0))
{
       printf("The value should between 0 and 1,(not include 0 and 1), and the command should be ./client <funct>  <value>\n");
       exit(0);
}
    printf("The client sent <%s> and <%s> to AWS.\n",argv[2],argv[1]);
//*******************************************************TCP*****************************************************
//****************This part is modified from http://www.linuxhowtos.org/data/6/client.c
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(AWS_PORT);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
//----------------------------get the dynamic port------------------------------------------------------------------
//---------------------------This part is modified from EE450 project manual--------------------------------------- 
     check=getsockname(sockfd, (struct sockaddr *)&serv_addr_client, (socklen_t *)&clientaddr);
	if (check== -1) {
	perror("getsockname\n");
	exit(1);}
//------------------------------------------send the function and value to AWS--------------
    n = write(sockfd,argv[1],strlen(argv[1]));
    if (n < 0) 
         error("ERROR writing to socket");
    n = write(sockfd,argv[2],strlen(argv[2]));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
//--------------------------------------------waiting for the result--------------------------
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("According to AWS, %s on <%s>: <%s>\n",argv[1],argv[2],buffer);
    close(sockfd);
    return 0;
}
