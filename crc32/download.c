#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc32.h"

#define BIN_OFFSET 		(12) /*12 byte = header(4 byte "inno") + spl_len(4 byte) + crc(4 byte)*/
#define PRNIT_HEADER	1

void message_print(unsigned int size)
{
	printf("x6_a55 spl bin add header --->\"inno + spl len + crc\" \r\n");
	printf("\tDate:2022/2/28\r\n");
	printf("\tVersion:V1.1\r\n");
}

int main(int argc, char *argv[])
{
	FILE *fp;
	unsigned char *buf;
	int nbytes, filelen;
	int i = 0, j = 0;
	unsigned int crc = 0;
	if (argc != 2) {
		printf("Error Usage! Reference Below:\r\n");
		printf("\t./download  <source_bin> \r\n");
		return -1;
	}

	fp = fopen(argv[1], "rb");
	if (fp == NULL) {
		printf("Can't, Open file %s\r\n",argv[1]);
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
	filelen = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	if (PRNIT_HEADER) {
		message_print(filelen);
	} else {
		printf("file %s size = %dByte\r\n", argv[1],filelen);
	}

	buf = malloc(BIN_OFFSET + filelen);
	if (buf == NULL) {
		printf("Mem Malloc Failed!\r\n");
		return -1;
	}

	memset(buf, 0, BIN_OFFSET + filelen);
	fread(buf + BIN_OFFSET, 1, filelen, fp);
	fclose(fp);

	crc = crc32(0, buf + BIN_OFFSET, filelen);
	printf("\tcrc: %u\r\n", crc);
	
	buf[0] = 'i'; buf[1] = 'n';
	buf[2] = 'n'; buf[3] = 'o';
	for (i = 0; i < 4; i++) {
		buf[4 + i] = ((filelen >> (8 * i) ) & 0xff);
		buf[8 + i] = ((crc >> (8 * i) ) & 0xff);
	}

	system("rm -rf spl_bin");
	system("touch spl_bin");
	fp = fopen("spl_bin", "wb");
	if (fp == NULL) {
		printf("Can't Open file spl_bin \r\n");
		return -1;
	}

	nbytes = fwrite(buf, 1, filelen + BIN_OFFSET, fp);
	if (nbytes != filelen + BIN_OFFSET) {
		printf("File Write Error!\r\n");
		free(buf);
		fclose(fp);
		return -1;
	}
	
	free(buf);
	fclose(fp);
	return 0;
}
