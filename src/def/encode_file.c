/*	def
*/


#include "include/gly.h"


void encodeFile(FILE *infile, FILE *outfile, const ulong filesize)
{
	/* read/write: 3 bytes */
	ULONG steps = filesize / 3;
	for (ULONG i=0; i<steps; i++) {
		UCHAR data[3];
		fread(&data, 1, 3, infile);
		
		USHORT tmp[2];
		tmp[0] = data[0] ^ ((data[1] & 0x0F) << 8);
		tmp[1] = ((data[1] & 0xF0) >> 4) ^ (data[2] << 4);
		
		for (UINT i=0; i<2; i++) {
			tmp[i] = encode(&tmp[i]);
		}
		
		data[0] = tmp[0] & 0x00FF;
		data[1] = ((tmp[0] & 0x0F00) >> 8) ^ ((tmp[1] & 0x000F) << 4);
		data[2] = (tmp[1] &0x0FF0) >> 4;
		
		if (fwrite(&data, 1, 3, outfile) <= 0) {
			printf("ERROR: cannot write into outfile (step: %lu, wordsteps).\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	/* 0-2 times: read 1 byte; write 2 bytes */
	steps = filesize % 3;
	for (UINT i=0; i<steps; i++) {
		UCHAR data;
		fread(&data, 1, 1, infile);
		
		USHORT tmp = data;	// it 's also a cast to ushort
		tmp = encode(&tmp);
		
		if (fwrite(&tmp, 2, 1, outfile) <= 0) {
			printf("ERROR: cannot write into outfile (step: %d, bytesteps).\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	fflush(outfile);
	
	return;
}
