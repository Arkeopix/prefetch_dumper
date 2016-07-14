#ifndef		DUMP_INFO_H_
# define	DUMP_INFO_H_

# include <stdint.h>
# include <time.h>
# include "prefetch.h"

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
        uint32_t *filename_string_offset_in_section_C;
        uint32_t *filename_string_len_in_section_C;
        uint64_t *file_ref;
} t_prefetch_info;

void dump_file_info(t_pf *prefetch);

#endif		/* !DUMP_INFO_H_ */
