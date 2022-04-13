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
	}
	return NULL;
}
