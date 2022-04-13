#include <unistd.h>
#include <errno.h>

ssize_t readn(int fd, void *buff, size_t n)
{
        size_t  nleft;
        ssize_t nread;
        char    *ptr;

        ptr = buff;
        nleft = n;
        while (nleft > 0) {
                if ((nread = read(fd, ptr, nleft)) < 0) {
                        if (errno == EINTR)
                                nread = 0;      /* and call read() again */
                        else
                                return (-1);
                } else if (nread == 0)
                        break;          /* EOF */
                
                nleft -= nread;
                ptr += nread; 
        }
        return (n - nleft);
}