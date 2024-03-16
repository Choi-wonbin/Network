#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 50
void error_handling(char *message);

int main(int argc, char* argv[]){
	FILE *fp;
	
	int sd;
	int read_cnt;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr; // ip, port
	
	//input example
	//gcc clint.c -o clint
	// ./clint 127.0.0.1 9091
	if(argc != 3){
		printf("Usage: %s <IP> <PORT>\n", argv[0]);
	}

	fp = fopen("receive.txt", "wb"); // write mode / file create or open
	sd = socket(PF_INET, SOCK_STREAM, 0);
	//PF_INET : tcp/ip(ipv4)
	//PF_NET6 : tcp/tp(ipv6)
	//SOCK_STREAM : tcp, SOCK_DGRAM : udp
	//0 : IPPROTO_TCP
	if(sd == -1) error_handling("socket() errer");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; //tcp ip connection
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]); //server ip address
	serv_adr.sin_port = htons(atoi(argv[2])); //server port number

	connect(sd, (struct sockaddr*) &serv_adr, sizeof(serv_adr));
	// sockaddr* -> ip + port 형식
	
	while((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
		fwrite((void *)buf, 1, read_cnt, fp);

	puts("comp!");
	write(sd, "bye", 5);
	fclose(fp);
	close(sd);
	return 0;
}

void error_handling(char* message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
