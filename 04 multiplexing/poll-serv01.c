//#include <netinet/in.h>
#include "lib.h"

#define SERV_PORT      2022

int main(int argc, char *argv[])
{
        int     i, maxi, listenfd, connfd, sockfd;
        int     nready;
        ssize_t n;
        char    buf[MAXLINE];
        socklen_t clilen;
        struct pollfd client[FOPEN_MAX];
        struct sockaddr_in cliaddr, servaddr;

        listenfd = Socket(AF_INET, SOCK_STREAM, 0);

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERV_PORT);

        Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        Listen(listenfd, 3);

        client[0].fd = listenfd;
        client[0].events = POLLRDNORM;
        for (i = 1; i < FOPEN_MAX; i++)
                client[i].fd = -1;      /* -1 indicates available entry */
        maxi = 0;                       /* max index into client[] array */

        while (1) {
                nready = Poll(client, maxi + 1, -1);           /* INFTIM: -1 */

                if (client[0].revents & POLLRDNORM) {   /* new client connection */
                        clilen = sizeof(cliaddr);
                        connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

                        for (i = 1; i < FOPEN_MAX; i++)
                                if (client[i].fd < 0) {
                                        client[i].fd = connfd;  /* save descriptor */
                                        break;
                                }
                        if (i == FOPEN_MAX)
                                err_quit("too many clients");
                        client[i].events = POLLRDNORM;
                        if (i > maxi)
                                maxi = i;       /* max index in client[] array */
                        if (--nready <= 0)
                                continue;       /* no more readable descriptors */
                }
                for (i = 1; i <= maxi; i++) {   /* check all clients for data */
                        if ((sockfd = client[i].fd) < 0)
                                continue;
                        if (client[i].revents & (POLLRDNORM | POLLERR)) {
                                if ((n = read(sockfd, buf, MAXLINE)) < 0) {
                                        if (errno == ECONNRESET) {      /* connection reset by client */
                                                Close(sockfd);
                                                client[i].fd = -1;
                                        } else
                                                err_sys("read error");
                                } else if (n == 0) {            /* connection close by client */
                                        Close(sockfd);
                                        client[i].fd = -1;
                                } else
                                        Writen(sockfd, buf, n);
                                if (--nready <= 0)
                                        break;          /* no more readable descriptors */
                        }
                }
        }
}