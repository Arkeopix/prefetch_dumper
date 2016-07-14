#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <inttypes.h>

#include "reader.h"
#include "prefetch.h"
#include "dump_info.h"
#include "utils.h"

static inline void get_base_info(t_pf *prefetch) {
  prefetch->info.offset_sec_a  = read_uint32();
  prefetch->info.entries_a = read_uint32();
  prefetch->info.offset_sec_b = read_uint32();
  prefetch->info.entries_b = read_uint32();
  prefetch->info.offset_sec_c = read_uint32();
  prefetch->info.len_c = read_uint32();
  prefetch->info.offset_sec_d = read_uint32();
  prefetch->info.entries_d = read_uint32();
  prefetch->info.len_d = read_uint32();
}

/* static void dump_file_info_0x1A(t_pf *prefetch) { */
/* 	/\* needs to be tested *\/ */
/* 	uint8_t buffer[FILE_INFO_LEN_0X1A + 1]; */
/* 	size_t i = 0, j = 0; */
	
/* 	memset(&buffer, 0, FILE_INFO_LEN_0X1A); */
/* 	if (read(fd, &buffer, FILE_INFO_LEN_0X1A) < 0) { */
/* 		fprintf(stderr, "%s\n", strerror(errno)); */
/* 		exit(-1); */
/* 	} */
/* 	buffer[FILE_INFO_LEN_0X1A] = 0; */
/* 	get_base_info(info, buffer, &i); */
/* 	i += 8; */
/* 	while (j < 8) { */
/* 		info->last_executions[j] = get_execution_time(buffer, &i); */
/* 		i += 8; */
/* 		j++; */
/* 	} */
/* 	i += 16; */
/* 	info->run_count = read_four_bytes(buffer, &i); */
/* } */

static void dump_file_info_23(t_pf *prefetch) {
  get_base_info(prefetch);
  prefetch->info.unkown_value_info1 = read_bytes(8);
  prefetch->info.last_executions[0] = read_timestamp_win_to_unix();
  prefetch->info.unkown_value_info2 = read_bytes(16);
  prefetch->info.execution_counter = read_uint32();
  prefetch->info.unkown_value_info3 = read_bytes(4);
  prefetch->info.unkown_value_info4 = read_bytes(80);

  printf("offset a = %"PRIu32"\n", prefetch->info.offset_sec_a);
  printf("entries a = %"PRIu32"\n", prefetch->info.entries_a);
  printf("offset b = %"PRIu32"\n", prefetch->info.offset_sec_b);
  printf("entries b = %"PRIu32"\n", prefetch->info.entries_b);
  printf("offset c = %"PRIu32"\n", prefetch->info.offset_sec_c);
  printf("entries c = %"PRIu32"\n", prefetch->info.len_c);
  printf("offset d = %"PRIu32"\n", prefetch->info.offset_sec_d);
  printf("entries d = %"PRIu32"\n", prefetch->info.len_d);
  printf("last execution = %s", ctime(&prefetch->info.last_executions[0]));
  printf("exec counter = %" PRIu32"\n", prefetch->info.execution_counter);
}

/* static void dump_file_info_0x11(t_pf *prefetch) { */
/* 	/\* needs to be tested *\/ */
/* 	uint8_t buffer[FILE_INFO_LEN_0X11 + 1]; */
/* 	size_t i = 0; */
	
/* 	memset(&buffer, 0, FILE_INFO_LEN_0X11); */
/* 	if (read(fd, &buffer, FILE_INFO_LEN_0X11) < 0) { */
/* 		fprintf(stderr, "%s\n", strerror(errno)); */
/* 		exit(-1); */
/* 	} */
/* 	buffer[FILE_INFO_LEN_0X11] = 0; */
/* 	get_base_info(info, buffer, &i); */
/* 	info->last_executions[0] = get_execution_time(buffer, &i); */
/* 	i += 16; */
/* 	info->run_count = read_four_bytes(buffer, &i); */
/* } */

void dump_file_info(t_pf *prefetch) {
  switch (prefetch->header.format_version) {
  case VER_17:
    //dump_file_info_0x17(prefetch);
    break;
  case VER_23:
    dump_file_info_23(prefetch);
    break;
  case VER_26:
  case VER_30:
    //dump_file_info_0x26(prefetch);
    break;
  }
}
