#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

int main()
{
	int fd;
	int n;
	const char filename[] = "blah";
	const char bufw[] = "abcde";
	const int rd_bytes = 3;
	char bufr[rd_bytes];

	assert(rd_bytes <= strlen(bufw));

	/* create & write */
	fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1) {
		printf("failed to open %s\n", filename);
		return 1;
	}

	n = write(fd, bufw, strlen(bufw));
	if (n != strlen(bufw)) {
		printf("failed to write to %s\n", filename);
		close(fd);
		return 2;
	}

	close(fd);

	/* open & read */
	fd = open(filename, O_RDWR);
	if (fd == -1) {
		printf("failed to open %s\n", filename);
		return 3;
	}

	n = read(fd, bufr, rd_bytes);
	if(n != rd_bytes) {
		printf("failed to read from %s\n", filename);
		close(fd);
		return 4;
	}
	bufr[n] = 0;
	printf("%d bytes read: %s\n", n, bufr);

	close(fd);

	return 0;
}

