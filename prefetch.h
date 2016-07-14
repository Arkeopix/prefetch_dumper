#ifndef  PREFETCH_H_
# define PREFETCH_H_

typedef struct s_pf_header {
  enum {
    VER_17 = 17,
    VER_23 = 23,
    VER_26 = 26,
    VER_30 = 30
  } format_version;
  uint8_t *signature;
  uint32_t unknown_value_header1;
  uint32_t file_size;
  char *executable_name;
  uint32_t pf_hash;
  uint8_t *unknown_value_header2;
} t_pf_header;

typedef struct s_pf_info {
  uint32_t offset_sec_a;
  uint32_t entries_a;
  uint32_t offset_sec_b;
  uint32_t entries_b;
  uint32_t offset_sec_c;
  uint32_t len_c;
  uint32_t offset_sec_d;
  uint32_t entries_d;
  uint32_t len_d;
  uint8_t *unkown_value_info1;
  time_t last_executions[8];
  uint8_t *unkown_value_info2;
  uint32_t execution_counter;
  uint8_t *unkown_value_info3;
  uint8_t *unkown_value_info4;
} t_pf_info;

typedef struct s_pf_metrics {
  uint32_t start_time;
  uint32_t duration;
  uint32_t average_duration;
  uint32_t filename_string_offset;
  uint32_t filename_string_len;
  uint8_t *unknown_value_metrics1;
  uint64_t mft_entry_index;
  uint16_t seq_number;
} t_pf_metrics;

typedef struct s_pf_trace_chains {
  uint32_t next_array_entry_index;
  uint32_t total_block_load_count;
  uint8_t *unknown_value_trace_chain1;
  uint8_t *unknown_value_trace_chain2;
  uint8_t *unknown_value_trace_chain3;
} t_pf_trace_chains;

typedef struct s_pf {
  t_pf_header header;
  t_pf_info info;
  t_pf_metrics *metrics;
  t_pf_trace_chains *trace_chains;
  char *filename_strings[];
} t_pf;

#endif   /* !PREFETCH_H_ */
