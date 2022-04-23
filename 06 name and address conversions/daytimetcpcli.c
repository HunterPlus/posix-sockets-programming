#include "lib.h"

int tcp_connect(char *, char *);
int main(int argc, char *argv[])
{
        int     sockfd, n;
        char    recvline[MAXLINE + 1];
        socklen_t len;
        struct sockaddr_storage ss;

        if (argc != 3)
                err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");
        sockfd = tcp_connect(argv[1], argv[2]);

        len = sizeof(ss);
        Getpeername(sockfd, (struct sockaddr *) &ss, &len);
        printf("connected to %s\n", Sock_ntop((struct sockaddr *) &ss, len));

        while ((n = Read(sockfd, recvline, MAXLINE)) > 0) {
                recvline[n] = 0;        /* null terminate */
                Fputs(recvline, stdout);
        }
        exit(0);
}