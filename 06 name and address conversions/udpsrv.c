#include "lib.h"
#include <time.h>

int udp_server(const char *, const char *, socklen_t *);
int main(int argc, char *argv[])
{
        int     sockfd;
        ssize_t n;
        char    buff[MAXLINE];
        time_t  ticks;
        socklen_t len;
        struct sockaddr_storage cliaddr;

        if (argc == 2)
                sockfd = udp_server(NULL, argv[1], NULL);
        else if (argc == 3)
                sockfd = udp_server(argv[1], argv[2], NULL);
        else
                err_quit("usage: daytimeudpsrv [ <host> ] <service or port>");

        while (1) {
                len = sizeof(cliaddr);
                n = Recvfrom(sockfd, buff, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len);
                printf("datagram from %s\n", Sock_ntop((struct sockaddr *) &cliaddr, len));

                ticks = time(NULL);
                snprintf(buff, sizeof(buff), "%.24s\n", ctime(&ticks));
                Sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &cliaddr, len);
        }
}