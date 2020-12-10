#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>


#define MAX_SIZE 1000
#define PORT_NUM 45001
#define SA struct sockaddr


void function(int sockfd)
{
	char buffer[MAX_SIZE];
	int n;
	
	char data[100] = "Ready?";
    char from[100] = "niharshah25899@gmail.com";
	char to[100] = "aayushhemal@outlook.com";
    char filename[100];
	FILE *fp;
	int time = 1;

	while(1)
	{
		bzero(buffer,sizeof(buffer));
		n=0;

		sleep(time);	
		// sending ELHO message
		strcpy(buffer,"ELHO");
		write(sockfd,buffer,sizeof(buffer));
		
		sleep(time);
		// reading 250 from server
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From Server : %s \n",buffer);
		
		if(strcmp(buffer,"250") != 0)
		   exit(0);
		   
		sleep(time);
		// sending mail from 
		strcpy(buffer,from);
		write(sockfd,buffer,sizeof(buffer));
		

		sleep(time);
		// reading 250 from server
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From Server : %s \n",buffer);
		
		if(strcmp(buffer,"250") != 0) 
		   exit(0);
		   
		   
		sleep(time);
		// sending rcpt to
		strcpy(buffer,to);
		write(sockfd,buffer,sizeof(buffer));
		

        sleep(time);
		// reading 250 from server
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From Server : %s \n",buffer);
		
		
		if(strcmp(buffer,"250") != 0) 
		   exit(0);
		
		
		sleep(time);
		// sending data
		strcpy(buffer,data);
		write(sockfd,buffer,sizeof(buffer));
		
		sleep(time);
		// reading 354 from server
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From Server : %s \n",buffer);
		
		
		if(strcmp(buffer,"354") != 0) 
		   exit(0);
		

		sleep(time);
		// sending message line by line
		int i,j;
		int len = strlen(from);
		const char *last = &from[len-9];
		const char *last1 = &from[len-11];

		if(strcmp(last,"gmail.com") == 0)
			strcpy(filename,"gmail");
		else if(strcmp(last1,"outlook.com") == 0)
			strcpy(filename,"outlook");

		len = strlen(filename);
		filename[len] = '/';
		
		for(i=0,j=len+1; i < strlen(from); i++,j++){
			if(from[i]!='@')
				filename[j]=from[i];
			else 
				break;
		}
	
		strcat(filename,"/sent.txt");
		fp = fopen(filename,"r");

		char ch=getc(fp);
		int count = 0;

		while(ch != EOF){
			buffer[count] = ch;
			ch=getc(fp);
			count++;
		}

		write(sockfd,buffer,sizeof(buffer));
		fclose(fp);

		sleep(time);
		// sending '.' 
		strcpy(buffer,".");
		write(sockfd,buffer,sizeof(buffer));
		
		sleep(time);
		// reading 250 from server 
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From Server : %s \n",buffer);
		
		
		if(strcmp(buffer,"250") != 0) 
		   exit(0);
		   
		sleep(time);
		// sending Quit
		strcpy(buffer,"QUIT");
		write(sockfd,buffer,sizeof(buffer));
		
		sleep(time);
		// reading 221 from server
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From Server : %s \n",buffer);
		
		
		if(strcmp(buffer,"221") != 0) 
		   exit(0);
		

		if((strncmp(buffer,"exit",4))==0)
		{
			printf("Client Exit.\n");
			break;
		}
		
		
		printf("Clinet leaving.\n");
		exit(0);
	}
}

int main()
{
	int sockfd,connfd;
	struct sockaddr_in servaddr,cli;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	if(sockfd==-1)
	{
		printf("Error.\n");
		exit(0);
	}
	else
		printf("Socket successfully created.\n");
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servaddr.sin_port=htons(PORT_NUM);
	
	if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))!=0)
	{
		printf("Error.\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");
	
	function(sockfd);
	close(sockfd);
}
