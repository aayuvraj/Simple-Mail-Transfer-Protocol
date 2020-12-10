#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#define MAX 1000
#define PORT 45001
#define SA struct sockaddr

#define MAX_LEN  1024


void *function(void *socket_desc)
{
	char buffer[MAX];
	int n;
	
	
    char from[50];
    char to[50];
    char data[100] = "Ready?";
	char filename[100];
	FILE *fp;
	int i,j;
	int t = 1;
	int sent;

    //Get the socket descriptor

    int sockfd = *(int*)socket_desc;

    int read_size;
    
   char input[MAX_LEN] = "Domain Name doesnot exist\0";
    int data_len;

    
    data_len = 1;
	
	for(;;)
	{
		
		sleep(t);
		// Reading ELHO client mesage
		bzero(buffer,MAX);
		read(sockfd,buffer,sizeof(buffer));
		printf("From client :  %s \n",buffer);
		
		if(strcmp(buffer,"ELHO") != 0) 
		  exit(0);
		  
		bzero(buffer,MAX);
		n=0;
		
		sleep(t);
		// reply 250 to client 
		strcpy(buffer,"250");
		write(sockfd,buffer,sizeof(buffer));
		
		sleep(t);
		// reading mail from
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From client : %s \n",buffer);
		strcpy(from,buffer);
		
		int len = strlen(buffer);
		const char *last = &buffer[len-9];
		const char *last1 = &buffer[len-11];
		
		if(strcmp(last,"gmail.com") != 0 && strcmp(last1,"outlook.com") != 0)
			exit(0);
		
	
		sleep(t);
		// reply 250 to client
		strcpy(buffer,"250");
		write(sockfd,buffer,sizeof(buffer));
		
		sleep(t);
		// reading rcpt to
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From client : %s \n",buffer);
		strcpy(to,buffer);

		len = strlen(buffer);
		last = &buffer[len-9];
		last1 = &buffer[len-11];
		

		if(strcmp(last,"gmail.com") == 0)
			strcpy(filename,"gmail");
		else if(strcmp(last1,"outlook.com") == 0)
			strcpy(filename,"outlook");
		else
			exit(0);

		len = strlen(filename);
		filename[len] = '/';
		
		for(i=0,j=len+1; i < strlen(to); i++,j++){
			if(to[i]!='@')
				filename[j]=to[i];
			else 
				break;
		}
	
		sleep(t);
		// reply 250 to client
		strcpy(buffer,"250");
		write(sockfd,buffer,sizeof(buffer));
		
		sleep(t);
		// reading data
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From client : %s \n",buffer);
		
		
		if(strcmp(buffer,"Ready?") != 0) 
		  exit(0);
		  

		sleep(t);
		// reply 354 to client
		strcpy(buffer,"354");
		write(sockfd,buffer,sizeof(buffer));
		
	
		sleep(t);
		// reading mail line by line
		strcat(filename,"/inbox.txt");
		fp = fopen(filename,"w");

		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From client : %s \n",buffer);
		

		if(strcmp(buffer,".") != 0){ 
								
			fprintf(fp,"%s",buffer);

			bzero(buffer,sizeof(buffer));
			read(sockfd,buffer,sizeof(buffer));
			printf("From client : %s \n",buffer);

		}

		fclose(fp);

		sleep(t);
		// reading end of message (".")		
		if(strcmp(buffer,".") != 0) 
		  exit(0);  
		

		sleep(t);
		// reply 250 to client
		strcpy(buffer,"250");
		write(sockfd,buffer,sizeof(buffer));
		

		sleep(t);
		// reading QUIT message
		bzero(buffer,sizeof(buffer));
		read(sockfd,buffer,sizeof(buffer));
		printf("From client : %s \n",buffer);
		
		if(strcmp(buffer,"QUIT") != 0) 
		  exit(0);  
		
	
		sleep(t);
		// reply 221 to client
		strcpy(buffer,"221");
		write(sockfd,buffer,sizeof(buffer));
	
	
		if(strncmp("exit",buffer,4)==0)
		{
			printf("Server Exit.\n");
			break;
		}


	}
	
}





int main()
	{
	int sock,client_sock,sent;
    struct sockaddr_in server, client;
    int i=0;
    unsigned int len;
    char data[MAX_LEN];
     char input[MAX_LEN];
    int data_len,c;
    sock = socket(AF_INET,SOCK_STREAM,0);

    if (sock == -1)
    {
        perror ("socket:");
        exit (-1);
    }
   
    server.sin_family = AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=INADDR_ANY;
    bzero(&server.sin_zero,8);
   
    len= sizeof(struct sockaddr_in);
    if ((bind(sock,(struct sockaddr *)&server, len )) == -1)
    {
        perror("bind");
        exit (-1);
    }
    if ((listen (sock, 5)) == -1)
    {
        perror ("listen");
        exit (-1);
    }
    

     //Accept and incoming connection

    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
     

    //Accept and incoming connection

    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);

	pthread_t thread_id;

	

    while( (client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c)) )

    {

        puts("Connection accepted");

         

        if( pthread_create( &thread_id , NULL ,  function , (void*) &client_sock) < 0)

        {

            perror("could not create thread");

            return 1;

        }

         

        //Now join the thread , so that we dont terminate before the thread

        //pthread_join( thread_id , NULL);

        puts("Handler assigned");

    }

     

    if (client_sock < 0)

    {

     perror("accept failed");

        return 1;

    }

     

    return 0;
}


