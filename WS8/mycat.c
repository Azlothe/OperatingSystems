#include "apue.h"

#define	BUFFSIZE	4096

int
main(void)
{
	int		n;
	char	buf[BUFFSIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		fprintf(stderr, "%d\n", n);

	if (n < 0)
		err_sys("read error");

	exit(0);
}
