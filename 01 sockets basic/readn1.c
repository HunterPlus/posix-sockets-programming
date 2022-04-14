#include <unistd.h>

int readn(int fd, void *buff, size_t n)
{
	int	nread, nleft;
	char	*bufp;
	
	bufp = buff;
	nleft = n;
	nread = 0;
	while (nleft && (nread = read(fd, bufp, nleft)) > 0) {
		nleft -= nread;
		bufp += nread;
	}
	
	if (nread < 0)
		return -1;
	return n - nleft;
}
