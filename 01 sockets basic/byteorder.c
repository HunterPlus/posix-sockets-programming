#include <stdio.h>
#include <sys/utsname.h>

int main(int argc, char *argv[])
{
	union {
		short	s;
		char	c[sizeof(short)];
	} un;
	
	un.s = 0x0102;		/* s = 16^2 + 2 = 258 */
	struct utsname  utsn;
	
	uname(&utsn);
	printf("%s-%s-%s: ", utsn.machine, utsn.sysname, utsn.release);
	if (sizeof(short) == 2) {
		if (un.c[0] == 1 && un.c[1] == 2)
			printf("big-endian.\n");
		else if (un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian.\n");
		else
			printf("unknown.\n");
	} else
		printf("sizeof(short) = %ld\n", sizeof(short));
	
	return 0;
}
