#include "lib.h"
#include <netinet/in.h>

#define SERV_PORT	2022

void str_cli(FILE *fp, int sockfd);
int main(int argc, char *argv[])
{
	int	sockfd;
	struct sockaddr_in servaddr;
	
	if (argc != 2)
		err_quit("usage: selct_cli <IPaddress>");
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr.s_addr);
	
	Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	str_cli(stdin, sockfd);
	
	exit(0);
}

void str_cli(FILE *fp, int sockfd)
{
	int	maxfdp1;
	fd_set	rset;
	char	sendline[MAXLINE], recvline[MAXLINE];
	
	FD_ZERO(&rset);
	while (1) {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(sockfd, fileno(fp)) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);
		
		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("server terminate prematurely");
			Fputs(recvline, stdout);
		}
		
		if (FD_ISSET(fileno(fp), &rset) {	/* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL)
				return ;	/* all done */
			Writen(sockfd, sendline, strlen(sendline));
		}
		    }  
}
