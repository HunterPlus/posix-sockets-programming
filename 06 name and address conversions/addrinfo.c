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

int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
        int     listenfd, n;
        const int on = 1;
        struct addrinfo hints, *res, *ressave;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_flags = AI_PASSIVE;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
                err_quit("tcp_listen error for %s, %s: %s", host, serv, gai_strerror(n));
        ressave = res;

        do {
                listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
                if (listenfd < 0)
                        continue;       /* error, try next one */
                Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
                if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
                        break;          /* success */
                Close(listenfd);        /* bind error, close and try next one */
        } while ((res = res->ai_next) != NULL);

        if (res == NULL)        /* errno from final socket() or bind() */
                err_sys("tcp_listen error for %s, %s", host, serv);
        
        Listen(listenfd, 3);

        if (addrlenp)
                *addrlenp = res->ai_addrlen;    /* return size of protocol */
        freeaddrinfo(ressave);

        return (listenfd);
}

int
udp_client(const char *host, const char *serv, struct sockaddr **saptr, socklen_t *lenp)
{
        int     sockfd, n;
        struct addrinfo hints, *res, *ressave;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;

        if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
                err_quit("udp_client error for %s, %s: %s", host, serv, gai_strerror(n));
        ressave = res;

        do {
                sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
                if (sockfd >= 0)
                        break;          /* success */
        } while ((res = res->ai_next) != NULL);

        if (res == NULL)        /* errno set from final socket() */
                err_sys("udp_client error for %s, %s", host, serv);
        *saptr = Malloc(res->ai_addrlen);
        memcpy(*saptr, res->ai_addr, res->ai_addrlen);
        *lenp = res->ai_addrlen;

        freeaddrinfo(ressave);

        return (sockfd);
}

int 
udp_connect(const char *host, const char *serv)
{
        int     sockfd, n;
        struct addrinfo hints, *res, *ressave;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;

        if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
                err_quit("udp_connect error for %s, %s: %s", host, serv, gai_strerror(n));
        ressave = res;

        do {
                sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
                if (sockfd < 0)
                        continue;       /* ignore this one */
                if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
                        break;          /* success */
                Close(sockfd);          /* ignore this one */
        } while ((res = res->ai_next) != NULL);

        if (res == NULL)                /* errno set from final connect() */
                err_sys("udp_connect error for %s, %s", host, serv);

        freeaddrinfo(ressave);
        return (sockfd);
}

int
udp_server(const char *host, const char *serv, socklen_t *addrlenp)
{
        int     sockfd, n;
        struct addrinfo hints, *res, *ressave;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_flags = AI_PASSIVE;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;

        if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
                err_quit("udp_server error for %s, %s: %s", host, serv, gai_strerror(n));
        ressave = res;

        do {
                sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
                if (sockfd < 0)
                        continue;       /* error - try next one */
                if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0)
                        break;          /* success */
                Close(sockfd);          /* bind error - close and try next one */
        } while ((res = res->ai_next) != NULL);

        if (res == NULL)        /* errno from final socket() or bind() */
                err_sys("udp_server error for %s, %s", host, serv);
        if (addrlenp)
                *addrlenp = res->ai_addrlen;
        
        freeaddrinfo(ressave);
        return (sockfd);
}