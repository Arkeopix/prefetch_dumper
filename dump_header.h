#ifndef		DUMP_HEADER_H_
# define	DUMP_HEADER_H_

# include <stdint.h>
# include <unistd.h>

# define BUFF_SIZE 88
# define HEADER_LEN 84
# define EXEC_NAME_LEN16 60
# define EXEC_NAME_LEN8 38

uint32_t dump_file_header(const int8_t fd);

#endif		/* !DUMP_HEADER_H_ */
