#include "lib.h"

#define SERV_PORT	2022

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen);
int main(int argc, char *argv[])
{
	int	sockfd;
	struct sockaddr_in servaddr, cliaddr;
	
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	Bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	dg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
	int	n;
	socklen_t len;
	char	mesg[MAXLINE];
	
	while (1) {
		len = clilen;
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
		
		Sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}
