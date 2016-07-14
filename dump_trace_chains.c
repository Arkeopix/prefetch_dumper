#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "reader.h"
#include "dump_info.h"
#include "dump_trace_chains.h"
#include "utils.h"


void dump_trace_chains_0x11(t_pf *prefetch) {
  uint32_t idx = 0;

  prefetch->trace_chains = calloc(prefetch->info.entries_b, sizeof(t_pf_trace_chains));
  while (idx < prefetch->info.entries_b) {
    prefetch->trace_chains[idx].next_array_entry_index = read_uint32();
    prefetch->trace_chains[idx].total_block_load_count = read_uint32();
    prefetch->trace_chains[idx].unknown_value_trace_chain1 = read_bytes(1);
    prefetch->trace_chains[idx].unknown_value_trace_chain2 = read_bytes(1);
    prefetch->trace_chains[idx].unknown_value_trace_chain3 = read_bytes(2);

    printf("next entry = %" PRIu32 "\n", prefetch->trace_chains[idx].next_array_entry_index);
    printf("load count = %" PRIu32 "\n", prefetch->trace_chains[idx].total_block_load_count);
    printf("unknown1 = %02X\n", *prefetch->trace_chains[idx].unknown_value_trace_chain1);
    printf("unknown2 = %02X\n", *prefetch->trace_chains[idx].unknown_value_trace_chain2);
    printf("unknown3 = %02X %02X\n", prefetch->trace_chains[idx].unknown_value_trace_chain3[0],
           prefetch->trace_chains[idx].unknown_value_trace_chain3[1]);
    idx++;
  }
}

/* void dump_trace_chains_0x1E(const int8_t fd, t_prefetch_info *info) { */

/* } */

void dump_trace_chains(t_pf *prefetch) {
  r.set(prefetch->info.offset_sec_b);
  switch (prefetch->header.format_version) {
  case VER_17:
  case VER_23:
  case VER_26:
    dump_trace_chains_0x11(prefetch);
  case VER_30:
    //dump_trace_chains_0x1E(fd, info);
    break;
  }
}
