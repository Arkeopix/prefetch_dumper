#include <unistd.h>
#include <stdint.h>
#include <iconv.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "prefetch.h"
#include "utils.h"

uint32_t bytes_to_uint32(const uint8_t buffer[], const size_t offset) {
	btoui32.byte[0] = buffer[offset];
	btoui32.byte[1] = buffer[offset + 1];
	btoui32.byte[2] = buffer[offset + 2];
	btoui32.byte[3] = buffer[offset + 3];
	return btoui32.nbr;
}

uint64_t bytes_to_uint64(const uint8_t buffer[], const size_t offset) {
	btoui64.byte[0] = buffer[offset];
	btoui64.byte[1] = buffer[offset + 1];
	btoui64.byte[2] = buffer[offset + 2];
	btoui64.byte[3] = buffer[offset + 3];
	btoui64.byte[4] = buffer[offset + 4];
	btoui64.byte[5] = buffer[offset + 5];
	btoui64.byte[6] = buffer[offset + 6];
	btoui64.byte[7] = buffer[offset + 7];
	return btoui64.nbr;
}

char *bytes_to_string(char *buff16, char *buff8, size_t buff16_len, size_t buff8_len) {
	iconv_t cd;
	/* Ok i have no idea why len8 must be 38 */
	/* in posix a char is 8bits, so utf16 should be 16bits or 2 bytes */
	/* if the input buffer is 60 bytes long, then dividing it by 2 gives us 30 */
	/* but setting len8 to 30 makes iconv set errno to E2BIG... */
	/* Maybe i'm just plain dumb or something but i just dont get it */
	size_t len16 = buff16_len, len8 = buff8_len;
	char *_buff16 = buff16, *_buff8 = buff8;
	
	if ((cd = iconv_open("UTF-8", "UTF-16LE")) == (iconv_t) -1) {
		fprintf(stderr, "%s\n", strerror(errno));
		return NULL;
	}
	if ((iconv(cd, &_buff16, &len16, &_buff8, &len8)) == (size_t) -1) {
		fprintf(stderr, "%s\n", strerror(errno));
		return NULL;
	}
	iconv_close(cd);
	return strdup(buff8);
}

time_t bytes_to_unix_timestamp(const uint8_t buffer[], const size_t offset) {
	uint64_t windows_time = bytes_to_uint64(buffer, offset);
	return windows_time / WINDOWS_TICK - WINDOWS_TO_UNIX_DIFFERENCE;
}
