#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "dump_info.h"
#include "dump_trace_chains.h"
#include "utils.h"


void dump_trace_chains_0x11(const int8_t fd, t_prefetch_info *info) {
        uint8_t buffer[SEC_B_SIZE_0X11 + 1];
        uint32_t ctr = 0, tmp_index = 0;
        size_t i = 0;

        while (ctr < (size_t)((info->entries_in_section_B <= 5) ? : 5)) {
          fprintf(stdout, "12 bytes array: entrie %d at offset %d\n", ctr, info->entries_in_section_B + tmp_index);
          lseek(fd, (off_t)info->offset_to_section_B + tmp_index, SEEK_SET);
          memset(&buffer, 0, SEC_B_SIZE_0X11);
          if (read(fd, &buffer, SEC_B_SIZE_0X11) < 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(-1);
          }
          buffer[SEC_B_SIZE_0X11] = 0;

          fprintf(stdout, "\tNext array entry index:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
          i += 4;
          fprintf(stdout, "\tTotal block load count:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
          tmp_index += SEC_B_SIZE_0X11;
          i = 0;
          ctr += 1;
        }
}


void dump_trace_chains_0x1E(const int8_t fd, t_prefetch_info *info) {

}

void dump_trace_chains(const int8_t fd, const uint32_t prefetch_version, t_prefetch_info *info) {
        fprintf(stdout, "#------ TRACE CHAIN ARRAY ------#\n");
  	switch (prefetch_version) {
	case 0x11:
	case 0x17:
	case 0x1A:
                dump_trace_chains_0x11(fd, info);
	case 0x1E:
		dump_trace_chains_0x1E(fd, info);
		break;
	}
}
