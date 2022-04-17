#include "sum.h"

void str_echo(int);
int main(int argc, char *arg[])
{
        int     listenfd, connfd;
        pid_t   childpid;
        socklen_t clilen;
        struct sockaddr_in cliaddr, servaddr;

        if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                fprintf(stderr, "socket create error\n");
                exit(1);
        }
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERV_PORT);

        Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
        Listen(listenfd, 3);
        while (1) {
                clilen = sizeof(cliaddr);
                connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
                if ((childpid = fork()) == 0) {         /* child process */
                        Close(listenfd);                /* close listening socket */
                        str_echo(connfd);               /* process the request */
                        exit(0);
                }
                Close(connfd);
        }
}

void str_echo(int sockfd)
{
        ssize_t n;
        struct args args;
        struct result result;

        while (1) {
                if ((n = Readn(sockfd, &args, sizeof(args))) == 0)
                        return;         /* connection closed by other end */
                
                result.sum = args.arg1 + args.arg2;
                Writen(sockfd, &result, sizeof (result));
        }
}