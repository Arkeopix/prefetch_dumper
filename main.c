#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#include "reader.h"
#include "prefetch.h"

static void dump_volume_info_23(t_pf *prefetch) {
  uint32_t idx = 0, file_ref_count = 0, string_count = 0;
  uint32_t string_len = 0;
  
  prefetch->volume_info = calloc(prefetch->info.entries_d, sizeof(t_pf_volume_info));
  while (idx < prefetch->info.entries_d) {
    prefetch->volume_info[idx].offset_to_volume_device_path = r.read_uint32();
    prefetch->volume_info[idx].len_volume_device_path = r.read_uint32();
    prefetch->volume_info[idx].volume_creation_time = r.read_timestamp_win_to_unix();
    prefetch->volume_info[idx].volume_serial_number = r.read_uint32();
    prefetch->volume_info[idx].offset_to_subsec_e = r.read_uint32();
    prefetch->volume_info[idx].len_subsec_e = r.read_uint32();
    prefetch->volume_info[idx].offset_to_subsec_f = r.read_uint32();
    prefetch->volume_info[idx].entries_subsec_f = r.read_uint32();
    prefetch->volume_info[idx].unknown_value_volumeinfo1 = r.read_bytes(4);
    prefetch->volume_info[idx].unknown_value_volumeinfo2 = r.read_bytes(28);
    prefetch->volume_info[idx].unknown_value_volumeinfo3 = r.read_bytes(4);
    prefetch->volume_info[idx].unknown_value_volumeinfo4 = r.read_bytes(28);
    prefetch->volume_info[idx].unknown_value_volumeinfo5 = r.read_bytes(4);

    r.set(prefetch->info.offset_sec_d + prefetch->volume_info[idx].offset_to_volume_device_path);
    prefetch->volume_info[idx].volume_name = r.read_string_utf16_to_utf8(prefetch->volume_info[idx].len_volume_device_path * 2);

    prefetch->volume_info[idx].file_refs = calloc(prefetch->volume_info[idx].len_subsec_e / 8, sizeof(t_pf_file_ref));
    r.set(prefetch->info.offset_sec_d + prefetch->volume_info[idx].offset_to_subsec_e);
    while (file_ref_count < prefetch->volume_info[idx].len_subsec_e / 8) {
      prefetch->volume_info[idx].file_refs[file_ref_count].mft_entry_index = r.read_mft_entry();
      prefetch->volume_info[idx].file_refs[file_ref_count].seq_number = r.read_uint16();
      file_ref_count++;
    }

    prefetch->volume_info[idx].directory_strings = calloc(prefetch->volume_info[idx].entries_subsec_f, sizeof(char *));
    r.set(prefetch->info.offset_sec_d + prefetch->volume_info[idx].offset_to_subsec_f);
    while (string_count < prefetch->volume_info[idx].entries_subsec_f) {
      string_len = r.read_uint16();
      prefetch->volume_info[idx].directory_strings[string_count] = calloc(string_len, sizeof(char));
      prefetch->volume_info[idx].directory_strings[string_count] = r.read_string_utf16_to_utf8(string_len * 2 + 2);
      string_count++;
    }
    idx++;
  }
}

void dump_volume_info(t_pf *prefetch) {
  r.set(prefetch->info.offset_sec_d);

  switch (prefetch->header.format_version) {
  case VER_17:
    //dump_volume_info_17(prefetch);
  case VER_23:
  case VER_26:
    dump_volume_info_23(prefetch);
  case VER_30:
    break;
  }

}

static void dump_filename_strings_17(t_pf *prefetch) {
  uint32_t idx = 0;
  off_t filename_offset = r.offset;
  
  prefetch->filename_strings = calloc(prefetch->info.entries_a, sizeof(char *));
  while (idx < prefetch->info.entries_a) {
    prefetch->filename_strings[idx] = calloc(prefetch->metrics[idx].filename_string_len, sizeof(char));
    r.set(filename_offset + prefetch->metrics[idx].filename_string_offset);
    prefetch->filename_strings[idx] = r.read_string_utf16_to_utf8(prefetch->metrics[idx].filename_string_len * 2);

    printf("%s\n", prefetch->filename_strings[idx]);
    idx++;
  }
}

void dump_filename_strings(t_pf *prefetch) {
  r.set(prefetch->info.offset_sec_c);
  switch (prefetch->header.format_version) {
  case VER_17:
  case VER_23:
  case VER_26:
  case VER_30:
    dump_filename_strings_17(prefetch);
    break;
  }
}


void dump_trace_chains_17(t_pf *prefetch) {
  uint32_t idx = 0;

  prefetch->trace_chains = calloc(prefetch->info.entries_b, sizeof(t_pf_trace_chains));
  while (idx < prefetch->info.entries_b) {
    prefetch->trace_chains[idx].next_array_entry_index = r.read_uint32();
    prefetch->trace_chains[idx].total_block_load_count = r.read_uint32();
    prefetch->trace_chains[idx].unknown_value_trace_chain1 = r.read_bytes(1);
    prefetch->trace_chains[idx].unknown_value_trace_chain2 = r.read_bytes(1);
    prefetch->trace_chains[idx].unknown_value_trace_chain3 = r.read_bytes(2);

    printf("next entry = %" PRIu32 "\n", prefetch->trace_chains[idx].next_array_entry_index);
    printf("load count = %" PRIu32 "\n", prefetch->trace_chains[idx].total_block_load_count);
    printf("unknown1 = %02X\n", *prefetch->trace_chains[idx].unknown_value_trace_chain1);
    printf("unknown2 = %02X\n", *prefetch->trace_chains[idx].unknown_value_trace_chain2);
    printf("unknown3 = %02X %02X\n", prefetch->trace_chains[idx].unknown_value_trace_chain3[0],
           prefetch->trace_chains[idx].unknown_value_trace_chain3[1]);
    idx++;
  }
}

void dump_trace_chains(t_pf *prefetch) {
  r.set(prefetch->info.offset_sec_b);
  switch (prefetch->header.format_version) {
  case VER_17:
  case VER_23:
  case VER_26:
    dump_trace_chains_17(prefetch);
  case VER_30:
    //dump_trace_chains_0x1E(fd, info);
    break;
  }
}

static void dump_file_metrics_23(t_pf *prefetch) {
  uint32_t idx = 0;

  prefetch->metrics = calloc(prefetch->info.entries_a, sizeof(t_pf_metrics));
  while (idx < prefetch->info.entries_a) {
    prefetch->metrics[idx].start_time = r.read_uint32();
    prefetch->metrics[idx].duration = r.read_uint32();
    prefetch->metrics[idx].average_duration = r.read_uint32();
    prefetch->metrics[idx].filename_string_offset = r.read_uint32();
    prefetch->metrics[idx].filename_string_len = r.read_uint32();
    prefetch->metrics[idx].unknown_value_metrics1 = r.read_bytes(4);
    prefetch->metrics[idx].file_ref.mft_entry_index = r.read_mft_entry();
    prefetch->metrics[idx].file_ref.seq_number = r.read_uint16();
    
    printf("start time %" PRIu32 "\n", prefetch->metrics[idx].start_time);
    printf("duration %" PRIu32 "\n", prefetch->metrics[idx].duration);
    printf("average duration %" PRIu32 "\n", prefetch->metrics[idx].average_duration);
    printf("filename string offset %" PRIu32 "\n", prefetch->metrics[idx].filename_string_offset);
    printf("filename string len %" PRIu32 "\n", prefetch->metrics[idx].filename_string_len);
    printf("mft_entry %"PRIu64"\n", prefetch->metrics[idx].file_ref.mft_entry_index);
    printf("seq number %"PRIu16"\n", prefetch->metrics[idx].file_ref.seq_number);
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

static inline void get_base_info(t_pf *prefetch) {
  prefetch->info.offset_sec_a = r.read_uint32();
  prefetch->info.entries_a = r.read_uint32();
  prefetch->info.offset_sec_b = r.read_uint32();
  prefetch->info.entries_b = r.read_uint32();
  prefetch->info.offset_sec_c = r.read_uint32();
  prefetch->info.len_c = r.read_uint32();
  prefetch->info.offset_sec_d = r.read_uint32();
  prefetch->info.entries_d = r.read_uint32();
  prefetch->info.len_d = r.read_uint32();
}

static void dump_file_info_23(t_pf *prefetch) {
  get_base_info(prefetch);
  prefetch->info.unkown_value_info1 = r.read_bytes(8);
  prefetch->info.last_executions[0] = r.read_timestamp_win_to_unix();
  prefetch->info.unkown_value_info2 = r.read_bytes(16);
  prefetch->info.execution_counter = r.read_uint32();
  prefetch->info.unkown_value_info3 = r.read_bytes(4);
  prefetch->info.unkown_value_info4 = r.read_bytes(80);

  printf("offset a = %"PRIu32"\n", prefetch->info.offset_sec_a);
  printf("entries a = %"PRIu32"\n", prefetch->info.entries_a);
  printf("offset b = %"PRIu32"\n", prefetch->info.offset_sec_b);
  printf("entries b = %"PRIu32"\n", prefetch->info.entries_b);
  printf("offset c = %"PRIu32"\n", prefetch->info.offset_sec_c);
  printf("entries c = %"PRIu32"\n", prefetch->info.len_c);
  printf("offset d = %"PRIu32"\n", prefetch->info.offset_sec_d);
  printf("entries d = %"PRIu32"\n", prefetch->info.entries_d);
  printf("last execution = %s", ctime(&prefetch->info.last_executions[0]));
  printf("exec counter = %" PRIu32"\n", prefetch->info.execution_counter);
}

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

static void dump_file_header(t_pf *prefetch) {
  prefetch->header.format_version = r.read_uint32();
  prefetch->header.signature = r.read_bytes(4);
  prefetch->header.unknown_value_header1 = r.read_uint32();
  prefetch->header.file_size = r.read_uint32();
  prefetch->header.executable_name = r.read_string_utf16_to_utf8(30 * 2);
  prefetch->header.pf_hash = r.read_uint32();
  prefetch->header.unknown_value_header2 = r.read_bytes(4);

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
  dump_volume_info(&prefetch);
  
  t_reader_delete();
  return 0;
}
