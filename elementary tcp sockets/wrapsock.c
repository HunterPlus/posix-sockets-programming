#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

void Listen(int fd, int backlog)
{
	char	*ptr;
	
	if ((ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);
	
	if (listen(fd, backlog) == -1) {
		fprintf("stderr, "listen error\n");
		exit(1);
	}
}
