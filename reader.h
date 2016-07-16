/* reader.h provides a structure t_reader to handle reads and offsets */
/* method in() returns an allocated buffer that the caller is responsible of freeing */
/* method skip() allows to set the file offset */
#ifndef		READER_H_
# define	READER_H_

#include <sys/types.h>

/* I don't believe i will ever need to read more to 4096 bytes in one */
/* time, so i figure i can just avoid the overhead of allocatin memory here. */
# define BUFFSIZE 4096
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


typedef struct s_reader {
  int8_t _fd;
  int8_t _buffer[BUFFSIZE + 1];
  //uint8_t *(*_in)(uint64_t bytes_to_read);

  int64_t (*set)(off_t offset);
  uint16_t (*read_uint16)();
  uint32_t (*read_uint32)();
  uint64_t (*read_uint64)();
  uint64_t (*read_mft_entry)();
  uint8_t *(*read_bytes)(const uint64_t bytes_to_read);
  char *(*read_string_utf16_to_utf8)(const uint64_t bytes_to_read);
  time_t (*read_timestamp_win_to_unix)();
  ssize_t bytes_read;
  off_t offset;
} t_reader;

/* We declare an extern reader that will be used throughout the files that need it */
/* This is only a declaration, and the variable stiil needs to be instanciated */
extern t_reader r;

int8_t t_reader_new(const char *filename);
void t_reader_delete();

#endif		/* !READER_H_ */
