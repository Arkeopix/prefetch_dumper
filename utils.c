#define _XOPEN_SOURCE
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <iconv.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include "reader.h"
#include "prefetch.h"
#include "utils.h"

uint16_t bytes_to_uint16(const uint8_t buffer[]) {
  memset(&btoui16, 0, sizeof(btoui16));
  btoui16.byte[0] = buffer[0];
  btoui16.byte[1] = buffer[1];
  return btoui16.nbr;
}

uint32_t read_seq_nbr(const uint8_t buffer[]) {
  memset(&btoui32, 0, sizeof(btoui32));
  btoui32.byte[0] = buffer[0];
  btoui32.byte[1] = buffer[1];
  return btoui32.nbr;
}

uint32_t bytes_to_uint32(const uint8_t buffer[]) {
  memset(&btoui32, 0, sizeof(btoui32));
  btoui32.byte[0] = buffer[0];
  btoui32.byte[1] = buffer[1];
  btoui32.byte[2] = buffer[2];
  btoui32.byte[3] = buffer[3];
  return btoui32.nbr;
}

uint64_t read_file_ref(const uint8_t buffer[]) {
  memset(&btoui64, 0, sizeof(btoui64));
  btoui64.byte[0] = buffer[0];
  btoui64.byte[1] = buffer[1];
  btoui64.byte[2] = buffer[2];
  btoui64.byte[3] = buffer[3];
  btoui64.byte[4] = buffer[4];
  btoui64.byte[5] = buffer[5];
  return btoui64.nbr;
}

uint64_t bytes_to_uint64(const uint8_t buffer[]) {
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

uint8_t *read_bytes(const uint64_t bytes_to_read) {
  uint8_t *bytes = NULL;
  
  bytes = r.in(bytes_to_read);
  if (bytes == NULL) {
    return NULL;
  }
  return bytes;
}

uint32_t read_uint32() {
  uint8_t *bytes = NULL;
  uint32_t ret;
  
  bytes = r.in(4);
  if (bytes == NULL) {
    return -1;
  }
  ret = bytes_to_uint32(bytes);
  free(bytes);
  return ret;
}

char *read_string_utf16_to_utf8(const uint64_t bytes_to_read) {
  uint8_t *buffer = NULL;
  size_t len16 = bytes_to_read, len8 = bytes_to_read / 2;
  char buff8[4096], *_buf8 = buff8, *ret_string = NULL;
  iconv_t cd;
  ssize_t ret;
  
  buffer = r.in(bytes_to_read);
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

time_t read_timestamp_win_to_unix() {
  uint8_t *buffer;
  buffer = r.in(8);
  uint64_t windows_time = bytes_to_uint64(buffer);
  free(buffer);
  return windows_time / WINDOWS_TICK - WINDOWS_TO_UNIX_DIFFERENCE;
}
