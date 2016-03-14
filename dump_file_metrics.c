#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "dump_file_metrics.h"
#include "dump_info.h"
#include "utils.h"

static void dump_file_metrics_0x11(const int8_t fd, const t_prefetch_info *info) {
	/* needs to be tested */
	uint8_t buffer[SEC_A_SIZE_0X11 + 1];
	size_t i = 0;

	lseek(fd, (off_t)info->offset_to_section_A, SEEK_SET);
	memset(&buffer, 0, SEC_A_SIZE_0X11);
	if (read(fd, &buffer, SEC_A_SIZE_0X11) < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(-1);
	}
	buffer[SEC_A_SIZE_0X11] = 0;
	
	fprintf(stdout, "Start time:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
	i += 4;
	fprintf(stdout, "Duration:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
}

static void dump_file_metrics_0x17(const int8_t fd, const t_prefetch_info *info) {
	uint8_t buffer[SEC_A_SIZE_0X17 + 1];
	size_t i = 0;

	lseek(fd, (off_t)info->offset_to_section_A, SEEK_SET);
	memset(&buffer, 0, SEC_A_SIZE_0X17);
	if (read(fd, &buffer, SEC_A_SIZE_0X17) < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(-1);
	}
	buffer[SEC_A_SIZE_0X17] = 0;
	
	fprintf(stdout, "Start time:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
	i += 4;
	fprintf(stdout, "Duration:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
	i += 4;
	fprintf(stdout, "Average duration:\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
}

void dump_file_metrics(const int8_t fd, const uint32_t prefetch_version, const t_prefetch_info *info) {
	fprintf(stdout, "#------ METRICS ------#\n");
	switch (prefetch_version) {
	case 0x11:
		dump_file_metrics_0x11(fd, info); 
		break;
	case 0x17:
	case 0x1A:
	case 0x1E:
		dump_file_metrics_0x17(fd, info);
		break;
	}
}
