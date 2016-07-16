#ifndef		UTILS_H_
# define	UTILS_H_

# include <stdint.h>
# include <unistd.h>
# include <time.h>

# define WINDOWS_TICK 10000000
# define WINDOWS_TO_UNIX_DIFFERENCE 11644473600LL

union {
	uint8_t byte[2];
	uint32_t nbr;
} btoui16;

union {
	uint8_t byte[4];
	uint32_t nbr;
} btoui32;

union {
	uint8_t byte[8];
	uint64_t nbr;
} btoui64;

//int32_t read_four_bytes(uint8_t buffer[], size_t *offset);
//uint64_t read_eight_bytes(uint8_t buffer[], size_t *offset);
uint16_t bytes_to_uint16(const uint8_t buffer[]);
uint32_t bytes_to_uint32(const uint8_t buffer[]);
uint64_t bytes_to_uint64(const uint8_t buffer[]);
uint64_t read_file_ref(const uint8_t buffer[]);
uint32_t read_seq_nbr(const uint8_t buffer[]);
uint32_t read_uint32();
uint8_t *read_bytes(const uint64_t bytes_to_read);
char *read_string_utf16_to_utf8(const uint64_t bytes_to_read);
time_t read_timestamp_win_to_unix();
char *bytes_to_string(char *buff16, char *buff8, size_t buff16_len, size_t buff8_len);

#endif		/* !UTILS_H_ */
