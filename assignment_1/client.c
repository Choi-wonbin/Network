#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc, char* argv[]){
	int sock;
	int str_len;
	char message[BUF_SIZE];
	struct sockaddr_in serv_adr; // ip, port
	
	//input example
	//gcc clint.c -o clint
	// ./clint 127.0.0.1 9091
	if(argc != 3){
		printf("Usage: %s <IP> <PORT>\n", argv[0]);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1) error_handling("socket() errer");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; //tcp ip connection
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]); //ip address
	serv_adr.sin_port = htons(atoi(argv[2])); //port number

	if(connect(sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1) error_handling("connect() errer");
	else puts("Connected--------->");

	while(1){
		fputs("Input message(Q, q to quit): ", stdout);
		fgets(message,BUF_SIZE, stdin);

		if(!strcmp(message, "q\n") || (!strcmp(message, "Q\n"))) break;

		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE - 1);
		message[str_len] = 0;
		printf("Message from server: %s", message);	
	}
	close(sock);
	return 0;
}

void error_handling(char* message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
