#include <unistd.h>
#include <errno.h>

ssize_t readline(int fd, void *buff, size_t maxlen)
{
        ssize_t n, rc;
        char    c, *ptr;

        ptr = buff;
        for (n = 1; n < maxlen; n++) {
again:
                if ((rc = read(fd, &c, 1)) == 1) {
                        *ptr++ = c;
                        if (c == '\n')
                                break;  /* newline is stored, like fgets() */                        
                } else if (rc == 0) {
                        *ptr = 0;
                        return (n - 1);         /* EOF, n-1 bytes were read */                        
                } else {
                        if (errno == EINTR)
                                goto again;
                        return (-1);            /* error, errno set by read() */
                }
        }

        *ptr = 0;       /* null terminate like fgets() */
        return (n);
}