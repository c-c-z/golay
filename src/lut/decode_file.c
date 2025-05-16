/*	def
*/


#include "include/gly.h"


void decodeFile(FILE *datafile, FILE *codefile, FILE *outfile, const ulong filesize)
{
	ULONG steps = filesize / 3;
	for (ULONG i=0; i<steps; i++) {
		UCHAR data[3];
		fread(&data, 1, 3, datafile);

		USHORT dataTmp[2];
		dataTmp[0] = data[0] ^ ((data[1] & 0x0F) << 8);
		dataTmp[1] = ((data[1] & 0xF0) >> 4) ^ (data[2] << 4);
		
		UCHAR code[3];
		fread(&code, 1, 3, codefile);
		
		USHORT codeTmp[2];
		codeTmp[0] = code[0] ^ ((code[1] & 0x0F) << 8);
		codeTmp[1] = ((code[1] & 0xF0) >> 4) ^ (code[2] << 4);
		
		for (UINT i=0; i<2; i++) {
			decode(&dataTmp[i], &codeTmp[i]);
		}
		
		data[0] = dataTmp[0] & 0x00FF;
		data[1] = ((dataTmp[0] & 0xF00) >> 8) ^ ((dataTmp[1] & 0x000F) << 4);
		data[2] = (dataTmp[1] & 0x0FF0) >> 4;
		
		if (fwrite(&data, 1, 3, outfile) <= 0) {
			printf("ERROR: cannot write into outfile (step: %lu, wordsteps).\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	steps = filesize % 3;
	for (UINT i=0; i<steps; i++) {
		UCHAR data;
		fread(&data, 1, 1, datafile);
		
		USHORT code;
		fread(&code, 2, 1, codefile);
		
		USHORT dataCast = data;
		decode(&dataCast, &code);
		data = dataCast;
		
		if (fwrite(&data, 1, 1, outfile) <= 0) {
			printf("ERROR: cannot write into outfile (step: %d, bytesteps).\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	fflush(outfile);
	
	return;
}
