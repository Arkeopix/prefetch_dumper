#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include "prefetch.h"
#include "reader.h"
#include "utils.h"
#include "dump_info.h"
#include "dump_file_metrics.h"
#include "dump_trace_chains.h"
#include "dump_filename_strings.h"

static void dump_file_header(t_pf *prefetch) {
  prefetch->header.format_version = read_uint32();
  prefetch->header.signature = read_bytes(4);
  prefetch->header.unknown_value_header1 = read_uint32();
  prefetch->header.file_size = read_uint32();
  prefetch->header.executable_name = read_string_utf16_to_utf8(60);
  prefetch->header.pf_hash = read_uint32();
  prefetch->header.unknown_value_header2 = read_bytes(4);

  /* This is for test purpose only, the output must be dealt with in the end */
  printf("Version: %" PRIu32 "\n", prefetch->header.format_version);
  printf("Signature: %s\n", prefetch->header.signature);
  printf("Header unkown value 1: %#02x" PRIu32"\n", prefetch->header.unknown_value_header1);
  printf("file size: %" PRIu32 "\n", prefetch->header.file_size);
  printf("executable name: %s\n", prefetch->header.executable_name);
  printf("pf hash: %X" PRIu32 "\n", prefetch->header.pf_hash);
  printf("Header unkown value 2: %02X %02X %02X %02X\n",
         prefetch->header.unknown_value_header2[0],
         prefetch->header.unknown_value_header2[1],
         prefetch->header.unknown_value_header2[2],
         prefetch->header.unknown_value_header2[3]);
}

int main(int argc, char *argv[]) {
        t_pf prefetch;
        
	if (argc < 2) {
		fprintf(stderr, "usage here\n");
		return -1;
	}

        t_reader_new(argv[1]);
        
        dump_file_header(&prefetch);
	dump_file_info(&prefetch);
        dump_file_metrics(&prefetch);
        dump_trace_chains(&prefetch);
        dump_filename_strings(&prefetch);
        
	/* if (close(fd) < 0) { */
	/* 	fprintf(stderr, "%s\n", strerror(errno)); */
	/* 	return -1; */
	/* } */
	return 0;
}
