#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "prefetch.h"
#include "dump_header.h"
#include "dump_info.h"
#include "dump_file_metrics.h"

int main(int argc, char *argv[]) {
	int8_t fd;
	uint32_t version;
	t_prefetch_info info = {0};
	
	if (argc < 2) {
		fprintf(stderr, "usage here\n");
		return -1;
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		return -1;
	}

	if ((version = dump_file_header(fd)) == (uint32_t) -1) {
		return -1;
	}

	dump_file_info(&info, version, fd);
	dump_file_metrics(fd, version, &info);
	
	if (close(fd) < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		return -1;
	}
	return 0;
}
