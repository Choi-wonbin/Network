#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 50
void error_handling(char *message);

int main(int argc, char *argv[]){
	int serv_sd, clnt_sd;
	FILE *fp;
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	
	socklen_t clnt_adr_sz;

	if(argc != 2){ // ./server 9091
		printf("Usage: %s <PORT> \n", argv[0]);
		exit(1);
	}
	
	fp = fopen("ass1.txt", "rb");
	serv_sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; // tcp ip
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // using my ip
	serv_adr.sin_port = htons(atoi(argv[1])); // port number
	
	bind(serv_sd, (struct sockaddr*) &serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr*) &clnt_adr, &clnt_adr_sz);
	
	while(1){
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if(read_cnt < BUF_SIZE){
			write(clnt_sd, buf, read_cnt);
			break;
		}

		write(clnt_sd, buf, read_cnt);
	}
	
	shutdown(clnt_sd, SHUT_WR);
	read(clnt_sd, buf, BUF_SIZE);
	printf("message from client: %s \n", buf);

	fclose(fp);
	close(clnt_sd);
	close(serv_sd);
	return 0;	
}

void error_handling(char* message){
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}
