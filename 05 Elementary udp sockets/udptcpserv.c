#include "lib.h"

#define SERV_PORT       2022

void str_echo(int);
int main(int argc, char *argv[])
{
        int     listenfd, connfd, udpfd, nready, maxfdp1;
        char    mesg[MAXLINE];
        pid_t   childpid;
        fd_set  rset;
        ssize_t n;
        socklen_t len;
        const int on = 1;
        struct sockaddr_in cliaddr, servaddr;

        listenfd = Socket(AF_INET, SOCK_STREAM, 0);

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERV_PORT);

        Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        Listen(listenfd, 3);

        udpfd = Socket(AF_INET, SOCK_DGRAM, 0);
/*
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERV_PORT);
*/
        Bind(udpfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        FD_ZERO(&rset);
        maxfdp1 = max(listenfd, udpfd) + 1;
        while (1) {
                FD_SET(listenfd, &rset);
                FD_SET(udpfd, &rset);
                nready = Select(maxfdp1, &rset, NULL, NULL, NULL);

                if (FD_ISSET(listenfd, &rset)) {
                        len = sizeof(cliaddr);
                        connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &len);

                        if ((childpid = Fork()) == 0) {
                                Close(listenfd);
                                str_echo(connfd);
                                exit(0);
                        }
                        Close(connfd);
                }
                if (FD_ISSET(udpfd, &rset)) {
                        len = sizeof(cliaddr);
                        n = Recvfrom(udpfd, mesg, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len);

                        Sendto(udpfd, mesg, n, 0, (struct sockaddr *) &cliaddr, len);
                }
        }
}

void str_echo(int sockfd)
{
        ssize_t n;
        char    buf[MAXLINE];

again:
        while ((n = read(sockfd, buf, MAXLINE)) > 0) 
                Writen(sockfd, buf, n);

        if (n < 0 && errno == EINTR)
                goto again;
        else if (n < 0)
                err_sys("str_echo: read error");
}