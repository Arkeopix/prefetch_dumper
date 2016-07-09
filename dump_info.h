#ifndef		DUMP_INFO_H_
# define	DUMP_INFO_H_

# include <stdint.h>
# include <time.h>

# define FILE_INFO_LEN_0X11 68
# define FILE_INFO_LEN_0X17 156
# define FILE_INFO_LEN_0X1A 224

typedef struct {
	uint32_t offset_to_section_A;
	uint32_t entries_in_section_A;
	uint32_t offset_to_section_B;
	uint32_t entries_in_section_B;
	uint32_t offset_to_section_C;
	uint32_t section_C_len;
	uint32_t offset_to_section_D;
	uint32_t entries_in_section_D;
	uint32_t section_D_len;
	time_t   last_executions[8];
        uint32_t run_count;
        /* as we dump the first 5 entries of section A we only need to store 5 of each */
        uint32_t filename_string_offset_in_section_C[5];
        uint32_t filename_string_len_in_section_C[5];
        uint64_t file_ref[5];
} t_prefetch_info;

void dump_file_info(t_prefetch_info *info, const uint32_t prefetch_version, const int8_t fd);

#endif		/* !DUMP_INFO_H_ */
