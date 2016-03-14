#ifndef		DUMP_FILE_METRICS_H_
# define    DUMP_FILE_METRICS_H_

# include <stdint.h>
# include "dump_info.h"

# define SEC_A_SIZE_0X11 20
# define SEC_A_SIZE_0X17 32

void dump_file_metrics(const int8_t fd, const uint32_t prefetch_version, const t_prefetch_info *info);

#endif		/* !DUMP_FILE_METRICS_H_ */
