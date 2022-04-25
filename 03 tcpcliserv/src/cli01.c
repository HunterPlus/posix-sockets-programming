#include <netinet/in.h>

#include "lib.h"
#define SERV_PORT       2022

void str_cli(FILE *fp, int sockfd);
int main(int argc, char *argv[])
{
        int     sockfd;
        struct sockaddr_in servaddr;

        if (argc != 2)
                err_quit("usage: tcpcli <IPaddress>");
        sockfd = Socket(AF_INET, SOCK_STREAM, 0);
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

        Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        str_cli(stdin, sockfd);

        exit(0);
}

void str_cli(FILE *fp, int sockfd)
{
        char    buf[BUFSIZ];
        size_t     n, nleft;

        while ((n = Read(0, buf, BUFSIZ)) > 0) {
                Writen(sockfd, buf, n);

                nleft = n;
                while ((n = Read(sockfd, buf, BUFSIZ)) > 0) {
                        Writen(1, buf, n);
                        if ((nleft -= n) <= 0)
                                break;
                }
        }
}