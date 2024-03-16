#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc, char *argv[]){
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len;
	char reply[BUF_SIZE] = "컴퓨터소프트웨어학부 2017030382 최원빈\n";
	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	
	socklen_t clnt_adr_sz;

	if(argc != 2){ // ./server 9091
		printf("Usage: %s <PORT> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1) error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; // tcp ip
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // using my ip
	serv_adr.sin_port = htons(atoi(argv[1])); // port number
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1) error_handling("bind() error");
	
	if(listen(serv_sock, 5) == -1) error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);

	for(int i = 0; i < 5; i++){
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &clnt_adr_sz);
	      	if(clnt_sock == -1) error_handling("clnt_sock error");
		else printf("Connected client %d \n", i + 1);

		while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0){
			if(!strcmp(message, "hello\n")) write(clnt_sock, reply, strlen(reply));
			else write(clnt_sock, message, str_len);
		}
		
		close(clnt_sock);
	}

	close(serv_sock);	
}

void error_handling(char* message){
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}
