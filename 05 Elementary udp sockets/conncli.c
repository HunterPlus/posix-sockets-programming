#include "lib.h"

#define SERV_PORT       2022

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen);
int main(int argc, char *argv[])
{
        int     sockfd;
        struct sockaddr_in servaddr;

        if (argc != 2)
                err_quit("usage: udpcli <IPaddress>");

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        Inet_pton(AF_INET, argv[1], &servaddr.sin_addr.s_addr);

        sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

        dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        exit(0);
}

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
        int     n;
        char    sendline[MAXLINE], recvline[MAXLINE + 1];

        Connect(sockfd, (struct sockaddr *) pservaddr, servlen);

        while (Fgets(sendline, MAXLINE, fp) != NULL) {
                Write(sockfd, sendline, strlen(sendline));
                n = Read(sockfd, recvline, MAXLINE);                
                recvline[n] = 0;        /* null terminate */
                Fputs(recvline, stdout);
        }
}