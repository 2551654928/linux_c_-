#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXDATASIZE 100

int main(int argc, char *argv[]){
	struct sockaddr_in server;
	int num,addrlen,sockfd;
	char buff[MAXDATASIZE];
	struct hostent *he;
	char flag[] = "quit";
	
	if(argc != 2){
		printf("[-] Usage:%s<IP Address>\n",argv[0]);
		return 0;
	}
	
	if((he = gethostbyname(argv[1])) == NULL){
		printf("[-] gethostbyname() error!\n");
		return 0;
	}
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		printf("[-] socket() error!");
		return 0;
	}
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(8000);
	server.sin_addr = *((struct in_addr *)he->h_addr);
	if(connect(sockfd,(struct sockaddr *)&server,sizeof(server)) == -1){
		printf("[-] connect() error!");
		return 0;
	}
	if((num = recv(sockfd,buff,MAXDATASIZE,0)) == -1){
		printf("[-] recv() error!");
		return 0;
	}
	buff[num-1] = '\0';
	printf("[+] Server Message:%s\n",buff);
	while(1){
		printf(">>>");
		scanf("%s",buff);
		if(strcmp(buff,flag) == 0){
			send(sockfd,buff,strlen(buff),0);
			printf("[!] Process exit!\n");
			return 0;
		}
		else
		{
			send(sockfd,buff,strlen(buff),0);
			num = recv(sockfd,buff,MAXDATASIZE,0);
			buff[num] = '\0';
			printf("[+] Server Message:%s\n",buff);
		}
		}
		close(sockfd);
		return 0;
}
