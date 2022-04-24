#include "lib.h"
#include <time.h>

int tcp_listen(const char *, const char *, socklen_t *);
int main(int argc, char *argv[])
{
        int     listenfd, connfd;
        socklen_t len;
        char    buff[MAXLINE];
        time_t  ticks;
        struct sockaddr_storage cliaddr;

        if (argc != 2)
                err_quit("usage: daytimetcpsrv1 <service or port#>");
        listenfd = tcp_listen(NULL, argv[1], NULL);
        while (1) {
                len = sizeof(cliaddr);
                connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &len);
                printf("connection from %s\n", Sock_ntop((struct sockaddr *) &cliaddr, len));

                ticks = time(NULL);
                snprintf(buff, sizeof(buff), "%.24s\n", ctime(&ticks));
                Write(connfd, buff, strlen(buff));

                Close(connfd);
        }
}