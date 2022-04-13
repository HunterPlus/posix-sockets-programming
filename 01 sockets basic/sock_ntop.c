#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
	uint16_t	port;
	char		portstr[8];
	static char	str[128];	/*  Unix domain is largest */
	
	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;
		
		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return NULL;
		if ((port = ntohs(sin->sin_port)) != 0) {
			snprintf(portstr, sizeof(portstr), ":%d", port);
			strcat(str, portstr);
		}
		return str;
	}
	case AF_INET6: {
		struct sockaddr_in6	*sin6 = (struct sockaddr_in6) sa6;
		
		str[0] = '[';
		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
			return NULL;
		if ((port = ntohs(sin6->sin6_port)) != 0) {
			snprintf(portstr, sizeof(portstr), "]:%d", port);
			strcat(str, portstr);
			return str;
		}
		return str + 1;
	}
	default:
		snprintf(str, sizeof(str), "sock_ntop: unknown AFxxx: %d, len %d", sa->sa_family, salen);
		return str;
	}
	return NULL;
}
