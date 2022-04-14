#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define MAXLINE 1024
int main(int argc, char *argv[])
{
        int     listenfd, connfd;
	socklen_t	len;
        struct sockaddr_in servaddr, cliaddr;
        char    buff[MAXLINE];
        time_t  ticks;

        if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                fprintf(stderr, "socket error: can not create.\n");
                exit(1);
        }
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(13);          /* daytime server */

        if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
                fprintf(stderr, "bind error: can not bind.\n");
                exit(1);
        }
        if (listen(listenfd, 5) == -1) {
                fprintf(stderr, "listen error: can not listen.\n");
                exit(1);
        }
        while (1) {
		len = sizeof(cliaddr);
                if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) == -1) {
                        fprintf(stderr, "accept error: can not accept.\n");
                        exit(1);
                }
		printf("connect from %s, port %d\n", 
		       inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
		       ntohs(ccliaddr.sin_port));
		
                ticks = time(NULL);
                snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
                if (write(connfd, buff, strlen(buff)) == -1) {
                        fprintf(stderr, "write error\n");
                        exit(1);
                }
                if (close(connfd) == -1) {
                        fprintf(stderr, "close error\n");
                        exit(1);
                }
        }

}
