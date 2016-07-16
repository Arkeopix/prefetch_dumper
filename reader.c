#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iconv.h>

#include "reader.h"

t_reader r;

static uint8_t *_t_reader_in(uint64_t bytes_to_read) {
  uint8_t *ret = NULL;
  
  memset(r._buffer, 0, BUFFSIZE);
  r.bytes_read = read(r._fd, &r._buffer, bytes_to_read);
  if (r.bytes_read < 0) {
    perror("Could not read from file");
    return NULL;
  }
  r._buffer[BUFFSIZE] = 0;
  ret = calloc(sizeof(uint8_t), bytes_to_read);
  if (ret == NULL) {
    perror("Could not allocate memory");
    return NULL;
  }
  memcpy(ret, r._buffer, r.bytes_read);
  r.offset += r.bytes_read;
  return ret;
}

static uint64_t _t_reader_bytes_to_uint64(const uint8_t buffer[]) {
  memset(&btoui64, 0, sizeof(btoui64));
  btoui64.byte[0] = buffer[0];
  btoui64.byte[1] = buffer[1];
  btoui64.byte[2] = buffer[2];
  btoui64.byte[3] = buffer[3];
  btoui64.byte[4] = buffer[4];
  btoui64.byte[5] = buffer[5];
  btoui64.byte[6] = buffer[6];
  btoui64.byte[7] = buffer[7];
  return btoui64.nbr;
}

static uint8_t *_t_reader_read_bytes(const uint64_t bytes_to_read) {
  uint8_t *bytes = NULL;
  
  bytes = _t_reader_in(bytes_to_read);
  if (bytes == NULL) {
    return NULL;
  }
  return bytes;
}

static uint64_t _t_reader_read_mft_entry() {
  uint8_t *bytes = NULL;

  bytes = _t_reader_in(6);
  if (bytes == NULL) {
    return -1;
  }
  memset(&btoui64, 0, sizeof(btoui64));
  btoui64.byte[0] = bytes[0];
  btoui64.byte[1] = bytes[1];
  btoui64.byte[2] = bytes[2];
  btoui64.byte[3] = bytes[3];
  btoui64.byte[4] = bytes[4];
  btoui64.byte[5] = bytes[5];
  free(bytes);
  return btoui64.nbr;
}

static uint16_t _t_reader_read_uint16() {
  uint8_t *bytes = NULL;

  bytes = _t_reader_in(2);
  if (bytes == NULL) {
    return -1;
  }
  memset(&btoui16, 0, sizeof(btoui16));
  btoui16.byte[0] = bytes[0];
  btoui16.byte[1] = bytes[1];
  free(bytes);
  return btoui16.nbr;
}

static uint32_t _t_reader_read_uint32() {
  uint8_t *bytes = NULL;
  
  bytes = _t_reader_in(4);
  if (bytes == NULL) {
    return -1;
  }
  memset(&btoui32, 0, sizeof(btoui32));
  btoui32.byte[0] = bytes[0];
  btoui32.byte[1] = bytes[1];
  btoui32.byte[2] = bytes[2];
  btoui32.byte[3] = bytes[3];  
  free(bytes);
  return btoui32.nbr;
}

static uint64_t _t_reader_read_uint64() {
  uint8_t *bytes = NULL;
  
  bytes = _t_reader_in(8);
  if (bytes == NULL) {
    return -1;
  }
  memset(&btoui64, 0, sizeof(btoui64));
  btoui64.byte[0] = bytes[0];
  btoui64.byte[1] = bytes[1];
  btoui64.byte[2] = bytes[2];
  btoui64.byte[3] = bytes[3];
  btoui64.byte[4] = bytes[4];
  btoui64.byte[5] = bytes[5];
  btoui64.byte[6] = bytes[6];
  btoui64.byte[7] = bytes[7];
  free(bytes);
  return btoui64.nbr;
}

static char *_t_reader_read_string_utf16_to_utf8(const uint64_t bytes_to_read) {
  uint8_t *buffer = NULL;
  size_t len16 = bytes_to_read, len8 = bytes_to_read / 2;
  char buff8[4096], *_buf8 = buff8, *ret_string = NULL;
  iconv_t cd;
  ssize_t ret;
  
  buffer = _t_reader_in(bytes_to_read);
  memset(&buff8, 0, 4096);
  cd = iconv_open("UTF-8", "UTF-16LE");
  if (cd == (iconv_t) -1) {
    perror("Could not open iconv");
    return NULL;
  }
  ret = iconv(cd, (char **)&buffer, &len16, &_buf8, &len8 + 1);
  if (ret == -1) {
    perror("Could not convert string from utf16 to utf8");
    return NULL;
  }
  iconv_close(cd);
  ret_string = calloc(strlen(buff8), sizeof(char));
  strncpy(ret_string, buff8, strlen(buff8));
  return ret_string;
}

static time_t _t_reader_read_timestamp_win_to_unix() {
  uint8_t *buffer;
  buffer = _t_reader_in(8);
  uint64_t windows_time = _t_reader_bytes_to_uint64(buffer);
  free(buffer);
  return windows_time / WINDOWS_TICK - WINDOWS_TO_UNIX_DIFFERENCE;
}

static int64_t _t_reader_set(off_t offset) {
  off_t ret;
  ret = lseek(r._fd, offset, SEEK_SET);
  if (ret == (off_t)-1) {
    perror("Could not set file offset");
    return -1;
  }
  return ret;
}

int8_t t_reader_new(const char *filename) {
  r._fd = open(filename, O_RDONLY);
  if (r._fd < 0) {
    perror("Could not open file");
    return -1;
  }
  r.offset = 0;
  r.bytes_read = 0;
  memset(r._buffer, 0, BUFFSIZE);
  //  r._in = &_t_reader_in;
  r.set = &_t_reader_set;
  r.read_bytes = &_t_reader_read_bytes;
  r.read_mft_entry = &_t_reader_read_mft_entry;
  r.read_string_utf16_to_utf8 = &_t_reader_read_string_utf16_to_utf8;
  r.read_timestamp_win_to_unix = &_t_reader_read_timestamp_win_to_unix;
  r.read_uint16 = &_t_reader_read_uint16;
  r.read_uint32 = &_t_reader_read_uint32;
  r.read_uint64 = &_t_reader_read_uint64;
  return 0;
}

void t_reader_delete() {
  close(r._fd);
}
