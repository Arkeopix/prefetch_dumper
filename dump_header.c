#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "dump_header.h"
#include "utils.h"

static uint32_t dump_version(uint8_t buffer[], size_t *offset) {
	uint32_t version = bytes_to_uint32(buffer, *offset);

	fprintf(stdout, "Prefetch version:\t0x%X\n", version);
	*offset += 4;
	return version;
}

static void dump_signature(uint8_t buffer[], size_t *offset) {
	fprintf(stdout, "Signature:\t\t0x%02X 0x%02X 0x%02X 0x%02X || ",
			buffer[*offset], buffer[*offset + 1], buffer[*offset + 2], buffer[*offset + 3]);
	fprintf(stdout, "%c%c%c%c\n", buffer[*offset], buffer[*offset + 1], buffer[*offset + 2], buffer[*offset + 3]);
	*offset += 8;
}

static void dump_file_length(uint8_t buffer[], size_t *offset) {
	fprintf(stdout, "File length:\t\t%u bytes\n", bytes_to_uint32(buffer, *offset));
	*offset += 4;
}

static uint8_t dump_exec_name(uint8_t buffer[], size_t *offset) {
	/* Ok i have no idea why len8 must be 38 */
	/* in posix a char is 8bits, so utf16 should be 16bits or 2 bytes */
	/* if the input buffer is 60 bytes long, then dividing it by 2 gives us 30 */
	/* which is just enough to old the 29 characters */
	/* but setting len8 to 30 makes iconv set errno to E2BIG... */
	/* Maybe i'm just plain dumb or something but i just dont get it */
	size_t len16 = EXEC_NAME_LEN16, len8 = EXEC_NAME_LEN8;
	char buff8[31], buff16[60], *_buff16, *_buff8 = buff8, *ret;
		
	memcpy(&buff16, &buffer[*offset], 60);
	_buff16 = buff16;
	
	if ((ret = bytes_to_string(_buff16, _buff8, len16, len8)) == NULL) {
		fprintf(stderr, "Failed to recover executable name\n");
		return -1;
	}
	
	fprintf(stdout, "Executable name:\t%s\n", ret);
	*offset += 60;
	free(ret);
	return 0;
}

static void dump_hash_path(uint8_t buffer[], size_t *offset) {
	fprintf(stdout, "Hash path:\t\t%X\n", bytes_to_uint32(buffer, *offset));
	*offset += 8;
}

uint32_t dump_file_header(const int8_t fd) {
	uint8_t buffer[BUFF_SIZE + 1];
	uint32_t version; 
	ssize_t bytes_read;
	size_t i = 0;
	
	memset(&buffer, 0, BUFF_SIZE);
	bytes_read = read(fd, &buffer, HEADER_LEN);
	if (bytes_read < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		return -1;
	}
	buffer[HEADER_LEN] = 0;

        fprintf(stdout, "#------ HEADER ------#\n");
	version = dump_version(buffer, &i);
	dump_signature(buffer, &i);
	dump_file_length(buffer, &i);
	if (dump_exec_name(buffer, &i) == (uint8_t) -1) {
		return -1;
	}
	dump_hash_path(buffer, &i);
	return version;
}
