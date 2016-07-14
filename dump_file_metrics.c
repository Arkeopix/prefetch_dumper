#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "reader.h"
#include "dump_file_metrics.h"
#include "dump_info.h"
#include "utils.h"

/* static void dump_file_metrics_0x11(const int8_t fd, t_prefetch_info *info) { */
/* 	/\* needs to be tested *\/ */
/* 	uint8_t buffer[SEC_A_SIZE_0X11 + 1]; */
/* 	size_t i = 0; */

/* 	lseek(fd, (off_t)info->offset_to_section_A, SEEK_SET); */
/* 	memset(&buffer, 0, SEC_A_SIZE_0X11); */
/* 	if (read(fd, &buffer, SEC_A_SIZE_0X11) < 0) { */
/* 		fprintf(stderr, "%s\n", strerror(errno)); */
/* 		exit(-1); */
/* 	} */
/* 	buffer[SEC_A_SIZE_0X11] = 0; */
	
/* 	fprintf(stdout, "Start time:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i)); */
/* 	i += 4; */
/* 	fprintf(stdout, "Duration:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i)); */
/* } */

static void dump_file_metrics_23(t_pf *prefetch) {
  uint32_t idx = 0;

  prefetch->metrics = calloc(prefetch->info.entries_a, sizeof(t_pf_metrics));
  while (idx < prefetch->info.entries_a) {
    prefetch->metrics[idx].start_time = read_uint32();
    prefetch->metrics[idx].duration = read_uint32();
    prefetch->metrics[idx].average_duration = read_uint32();
    prefetch->metrics[idx].filename_string_offset = read_uint32();
    prefetch->metrics[idx].filename_string_len = read_uint32();
    prefetch->metrics[idx].unknown_value_metrics1 = read_bytes(4);
    prefetch->metrics[idx].mft_entry_index = bytes_to_uint64(read_bytes(6));
    prefetch->metrics[idx].seq_number = bytes_to_uint16(read_bytes(2));
    
    printf("start time %" PRIu32 "\n", prefetch->metrics[idx].start_time);
    printf("duration %" PRIu32 "\n", prefetch->metrics[idx].duration);
    printf("average duration %" PRIu32 "\n", prefetch->metrics[idx].average_duration);
    printf("filename string offset %" PRIu32 "\n", prefetch->metrics[idx].filename_string_offset);
    printf("filename string len %" PRIu32 "\n", prefetch->metrics[idx].filename_string_len);
    printf("mft_entry %"PRIu64"\n", prefetch->metrics[idx].mft_entry_index);
    printf("seq number %"PRIu16"\n", prefetch->metrics[idx].seq_number);
    idx++;
  }
}

void dump_file_metrics(t_pf *prefetch) {
  r.set(prefetch->info.offset_sec_a);
  switch (prefetch->header.format_version) {
  case VER_17:
    //dump_file_metrics_17(prefetch);
    break;
  case VER_23:
  case VER_26:
  case VER_30:
    dump_file_metrics_23(prefetch);
    break;
  }
}
