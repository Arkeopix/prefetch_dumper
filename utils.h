#ifndef		UTILS_H_
# define	UTILS_H_

# include <stdint.h>
# include <unistd.h>
# include <time.h>

# define WINDOWS_TICK 10000000
# define WINDOWS_TO_UNIX_DIFFERENCE 11644473600LL

union {
	uint8_t byte[4];
	uint32_t nbr;
} btoui32;

union {
	uint8_t byte[8];
	uint64_t nbr;
} btoui64;

uint32_t bytes_to_uint32(const uint8_t buffer[], const size_t offset);
time_t bytes_to_unix_timestamp(const uint8_t buffer[], const size_t offset);
char *bytes_to_string(char *buff16, char *buff8, size_t buff16_len, size_t buff8_len);

#endif		/* !UTILS_H_ */
