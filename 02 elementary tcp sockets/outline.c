#include <unistd.h>
#include <sys/socket.h>

pid_t   pid;
int     listenfd, connfd;

/* fill in sockaddr_in{} with server's well-known port */
bind(listenfd, ...);
listen(listenfd, 5);

while (1) {
        connfd = accept(listenfd, ...);         /* probably blocks */

        if ((pid = fork()) == 0) {
                close(listenfd);        /* child closes listening socket */
                doit(connfd);           /* process the request */
                close(connfd);          /* child terminates */
                exit(0);
        }

        close(connfd);                  /* parent close connected socket */
}