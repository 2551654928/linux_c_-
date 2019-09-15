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
	struct sockaddr_in client;
	int listenfd,connectfd,opt,addrlen,num,i;
	char buff[MAXDATASIZE];
	char flag[] = "quit";
	char buf[MAXDATASIZE];
	char q[MAXDATASIZE];
	
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd == -1){
		printf("[-] socket() error!");
		return 0;
	}
	opt = SO_REUSEADDR;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(8000);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listenfd,(struct sockaddr *)&server,sizeof(server)) == -1){
		printf("bind() error!");
		return 0;
	}
	if(listen(listenfd,10) == -1){
		printf("[-] listen() error!");
		return 0;
	}
	printf("[+] waiting...\n");
	addrlen = sizeof(client);
	if((connectfd = accept(listenfd,(struct sockaddr *)&client,&addrlen)) == -1){
		printf("[-] accept() error!");
		return 0;
	}
	printf("[!] Remote client's ip:%s, Remote client's port:%d\n",inet_ntoa(client.sin_addr),htons(client.sin_port));
	send(connectfd,"Welcome\n",8,0);
	while((num = recv(connectfd,buff,MAXDATASIZE,0)) > 0)
	{
		buff[num] = '\0';
		printf("[+] Recv Message:%s\n",buff);
		strcpy(buf,buff);
		for(i = 0;i < num;i++)
		{
			q[num-1-i] = buf[i];
		}
		q[num] = '\0';
		if(strcmp(buf,flag) == 0){
			printf("[!] exit Message,Process exit!\n");
			return 0;
		}
		else{
			printf("[*] After flipping,the string is:%s\n",q);
			send(connectfd,q,strlen(q),0);
			printf("[+] Send Message:%s\n",q);
		}
	}
	close(connectfd);
	close(listenfd);
	return 0;
}
