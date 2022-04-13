#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 1024

int main(int argc, char *argv[])
{
        int     sockfd, n;
        char    recvline[MAXLINE + 1];
        struct sockaddr_in servaddr;

        if (argc != 2) {
                fprintf(stderr, "usage: a.out <IPaddress>\n");
                exit(1);
        }
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                fprintf(stderr, "socket error: can not create.\n");
                exit(1);
        }
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(13);          /* daytime server */
        if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
                fprintf(stderr, "inet_pton error: %s\n", argv[1]);
                exit(1);
        }
        if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
                fprintf(stderr, "connect error\n");
                exit(1);
        }

        while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
                recvline[n] = 0;        /* null terminate */
                if (fputs(recvline, stdout) == EOF) {
                        fprintf(stderr, "fputs error\n");
                        exit(1);
                }
        }
        if (n < 0) {
                fprintf(stderr, "read error\n");
                exit(1);
        }
        return 0;
}