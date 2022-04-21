#include "lib.h"

#define SERV_PORT       2022

int main(int argc, char *argv[])
{
        int     sockfd;
        socklen_t len;
        struct sockaddr_in cliaddr, servaddr;

        if (argc != 2)
                err_quit("usage: outcli <IPaddress>");

        sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        Inet_pton(AF_INET, argv[1], &servaddr.sin_addr.s_addr);

        Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        len = sizeof(cliaddr);
        Getsockname(sockfd, (struct sockaddr *) &cliaddr, &len);
        printf("local address %s\n", Sock_ntop((struct sockaddr *) &cliaddr, len));

        exit(0);
}