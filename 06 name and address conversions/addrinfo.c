#include "lib.h"
#include <netdb.h>

struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype)
{
        int     n;
        struct addrinfo hints, *res;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_flags = AI_CANONNAME;          /* always return canonical name */
        hints.ai_family = family;               /* AF_UNSPEC, AF_INET, AF_INET6, etc. */
        hints.ai_socktype = socktype;           /* 0, SOCK_STREAM, SOCK_DGRAM, etc. */

        if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
                return (NULL);
        return (res);           /* return pointer to first on linked list */
}

int
tcp_connect(const char *host, const char *serv)
{
        int     sockfd, n;
        struct addrinfo hints, *res, *ressave;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
                err_quit("tcp_connect error for %s, %s: %s", host, serv, gai_strerror(n));
        ressave = res;

        do {
                sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
                if (sockfd < 0)
                        continue;       /* ignore this one */
                if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
                        break;          /* success */
                Close(sockfd);          /* ignore this one */
        } while ((res = res->ai_next) != NULL);

        if (res == NULL)        /* errno set from final connect() */
                err_sys("tcp_connect error for %s, %s", host, serv);
        freeaddrinfo(ressave);

        return (sockfd);
}