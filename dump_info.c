#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <inttypes.h>

#include "dump_info.h"
#include "utils.h"

static uint32_t read_four_bytes(uint8_t buffer[], size_t *offset) {
	uint32_t ret;
	ret = bytes_to_uint32(buffer, *offset);
	*offset += 4;
	return ret;
}

static time_t get_execution_time(uint8_t buffer[], size_t *offset) {
	time_t timestamp = bytes_to_unix_timestamp(buffer, *offset);
	*offset += 8;
	return timestamp;
}

static inline void get_base_info(t_prefetch_info *info, uint8_t buffer[], size_t *offset) {
	info->offset_to_section_A = read_four_bytes(buffer, offset);
	info->entries_in_section_A = read_four_bytes(buffer, offset);
	info->offset_to_section_B = read_four_bytes(buffer, offset);
	info->entries_in_section_B = read_four_bytes(buffer, offset);
	info->offset_to_section_C = read_four_bytes(buffer, offset);
	info->section_C_len = read_four_bytes(buffer, offset);
	info->offset_to_section_D = read_four_bytes(buffer, offset);
	info->entries_in_section_D = read_four_bytes(buffer, offset);
	info->section_D_len = read_four_bytes(buffer, offset);
}

static void dump_file_info_0x1A(const int8_t fd, t_prefetch_info *info) {
	/* needs to be tested */
	uint8_t buffer[FILE_INFO_LEN_0X1A + 1];
	size_t i = 0, j = 0;
	
	memset(&buffer, 0, FILE_INFO_LEN_0X1A);
	if (read(fd, &buffer, FILE_INFO_LEN_0X1A) < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(-1);
	}
	buffer[FILE_INFO_LEN_0X1A] = 0;
	get_base_info(info, buffer, &i);
	i += 8;
	while (j < 8) {
		info->last_executions[j] = get_execution_time(buffer, &i);
		i += 8;
		j++;
	}
	i += 16;
	info->run_count = read_four_bytes(buffer, &i);
}

static void dump_file_info_0x17(const int8_t fd, t_prefetch_info *info) {
	uint8_t buffer[FILE_INFO_LEN_0X17 + 1];
	size_t i = 0;
	
	memset(&buffer, 0, FILE_INFO_LEN_0X17);
	if (read(fd, &buffer, FILE_INFO_LEN_0X17) < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(-1);
	}
	buffer[FILE_INFO_LEN_0X17] = 0;
	get_base_info(info, buffer, &i);
	i += 8; /* We skip 8 bytes because their sense is unknown */
	info->last_executions[0] = get_execution_time(buffer, &i);
	i += 16; /* We skip 16 bytes because their sense is unknown */
	info->run_count = read_four_bytes(buffer, &i);
}

static void dump_file_info_0x11(const int8_t fd, t_prefetch_info *info) {
	/* needs to be tested */
	uint8_t buffer[FILE_INFO_LEN_0X11 + 1];
	size_t i = 0;
	
	memset(&buffer, 0, FILE_INFO_LEN_0X11);
	if (read(fd, &buffer, FILE_INFO_LEN_0X11) < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(-1);
	}
	buffer[FILE_INFO_LEN_0X11] = 0;
	get_base_info(info, buffer, &i);
	info->last_executions[0] = get_execution_time(buffer, &i);
	i += 16;
	info->run_count = read_four_bytes(buffer, &i);
}

void dump_file_info(t_prefetch_info *info, const uint32_t prefetch_version, const int8_t fd) {
	size_t i = 0;
	fprintf(stdout, "#------  INFO  ------#\n");
	
	switch (prefetch_version) {
	case 0x11:
		dump_file_info_0x11(fd, info); 
		break;
	case 0x17:
		dump_file_info_0x17(fd, info);
		break;
	case 0x1A:
	case 0x1E:
		dump_file_info_0x1A(fd, info);
		break;
	}

	fprintf(stdout, "Last execution:\t\t%s", ctime(&info->last_executions[0]));
	for (i = 1; i < 8 && info->last_executions[i] != 0; i++) {
		fprintf(stdout, "\t\t\t%s", ctime(&info->last_executions[i]));
	}
	fprintf(stdout, "Run count:\t\t%" PRIu32 "\n", info->run_count);
}
