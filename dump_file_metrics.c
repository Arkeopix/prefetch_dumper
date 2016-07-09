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

static void dump_file_metrics_0x11(const int8_t fd, t_prefetch_info *info) {
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

static void dump_file_metrics_0x17(const int8_t fd, t_prefetch_info *info) {
        uint8_t buffer[SEC_A_SIZE_0X17 + 1], mft_bytes[6];
	size_t i = 0;
        uint32_t tmp_index = 0;
        uint32_t ctr = 0;
        uint64_t *mft_entry = NULL;
        uint16_t *mft_seq = NULL;

        /* I supose the first 5 entries are somewhat nice enough*/
        while (ctr < (size_t)((info->entries_in_section_A <= 5) ? : 5)) {
          fprintf(stdout, "32 bytes array: entrie %d at offset %d\n", ctr, info->entries_in_section_A + tmp_index);
          lseek(fd, (off_t)info->offset_to_section_A + tmp_index, SEEK_SET);
          memset(&buffer, 0, SEC_A_SIZE_0X17);
          if (read(fd, &buffer, SEC_A_SIZE_0X17) < 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(-1);
          }
          buffer[SEC_A_SIZE_0X17] = 0;
          
	  fprintf(stdout, "\tStart time:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
          i += 4;
          fprintf(stdout, "\tDuration:\t\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
          i += 4;
          fprintf(stdout, "\tAverage duration:\t%" PRIu32 "\n", bytes_to_uint32(buffer, i));
          i += 4;
          info->filename_string_offset_in_section_C[ctr] = read_four_bytes(buffer, &i);
          info->filename_string_len_in_section_C[ctr] = read_four_bytes(buffer, &i);
          fprintf(stdout, "\tfilename_string_offset\t%" PRIu32 "\n", info->filename_string_offset_in_section_C[ctr]);
          fprintf(stdout, "\tfilename_string_len\t%" PRIu32 "\n", info->filename_string_len_in_section_C[ctr]);
          i += 4; /* We skip 4 bytes because we don't know shit about them */
          
          /* read in the file ref */
          if (buffer[i] != 0) {
            memcpy(mft_bytes, buffer + i, 6);
            mft_entry = (uint64_t *)&mft_bytes[0];
            fprintf(stdout, "\tMft entry\t\t%" PRIu64 "\n", *mft_entry);
            i+= 6; /* We just read 6 bytes from the buffer */
            memset(mft_bytes, 0, 6);
            memcpy(mft_bytes, buffer + i, 2);
            mft_seq = (uint16_t *)&mft_bytes[0];
            /* If a lot of seq numbers qre the same: https://jmharkness.wordpress.com/2011/01/27/mft-file-reference-number/ */
            /* ntfs seems to be all about reusing sesquence numbers... But i may be totally wrong */
            fprintf(stdout, "\tMft sequence number\t%" PRIu16 "\n", *mft_seq);
          }
          
          tmp_index += SEC_A_SIZE_0X17;
          i = 0;
          ctr += 1;
        }
}

void dump_file_metrics(const int8_t fd, const uint32_t prefetch_version, t_prefetch_info *info) {
	fprintf(stdout, "#------ FILE METRICS ARRAY ------#\n");
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
