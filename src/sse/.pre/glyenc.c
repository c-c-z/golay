#include "../include/gly.h"
#include "../include/sse.h"
#include <string.h>


inline void process(FILE *infile, FILE *outfile, const uint readSteps, const uint dataVars, const uint codeVars)
{
	for (UINT i=0; i<readSteps; i++) {
		USHORT data[dataVars];
		fread(&data, 2 /* sizeof(ushort) */, dataVars, infile);
		
		USHORT tmp[codeVars];
		UINT j = 0,
			k = 0,
			jk = 0;
		while (j < dataVars) {
			jk = j + k;
			tmp[jk] = data[j] & 0x0FFF;
			tmp[jk+1] = ((data[j] & 0xF000) >> 12) ^ ((data[j+1] & 0x00FF) << 4);
			tmp[jk+2] = ((data[j+1] & 0xFF00) >> 8) ^ ((data[j+2] & 0x000F) << 8);
			tmp[jk+3] = ((data[j+2] & 0xFFF0) >> 4);
			j += 3;
			k++;
		}
		
		for (UINT i=0; i<codeVars; i++) {
			tmp[i] = encode(&tmp[i]);
		}
		
		j = 0;
		k = 0;
		jk = 0;
		while (j < dataVars) {
			jk = j + k;
			data[j] = tmp[jk] ^ ((tmp[jk+1] & 0x000F) << 12);
			data[j+1] = ((tmp[jk+1] & 0x0FF0) >> 4) ^ ((tmp[jk+2] & 0x00FF) << 8);
			data[j+2] = ((tmp[jk+2] & 0x0F00) >> 8) ^ ((tmp[jk+3] & 0x0FFF) << 4);
			j += 3;
			k++;
		}
		
		if (fwrite(&data, 2, dataVars, outfile) <= 0) {
			printf("ERROR: cannot write into outfile (step: %d of %d).\n", i, readSteps);
			exit(EXIT_FAILURE);
		}
	}
	return;
}



int main(int argc, char **argv)
{
	if ((argc < 0) || (strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
		printf("Usage: glyenc <filename>.\n\n");
		exit(EXIT_FAILURE);
	}
	
	CHAR outfileName[1024];
	strcpy(outfileName, argv[1]);
	strcat(outfileName, ".gly");
	FILE *outfile = fopen(outfileName, "wb");
	if (!outfile) {
		printf("ERROR: cannot open outfile %s.\n", outfileName);
		exit(EXIT_FAILURE);
	}
	fseek(outfile, 0, SEEK_SET);
	
	FILE *infile = fopen(argv[1], "rb");
	if (!infile) {
		printf("ERROR: cannot open infile %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	fseek(infile, 0, SEEK_SET);
	
	fseek(infile, 0, SEEK_END);
	ULONG filesize = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	if (fwrite(&filesize, 8, 1, outfile) <= 0) {
		printf("ERROR: cannot write filesize into %s.\n", outfileName);
		exit(EXIT_FAILURE);
	}
	
	
	/* read/encode/write: 3.000.000 bytes */
	UINT steps = filesize / 3000000;
	process(infile, outfile, steps, 1500000, 2000000);
	UINT stepsRest = filesize % 3000000;
	
	/* read/write 300.000 byte */
	steps = stepsRest / 300000;
	process(infile, outfile, steps, 150000, 200000);
	stepsRest %= 300000;
	
	/* read/write 300 byte */
	steps = stepsRest / 300;
	process(infile, outfile, steps, 150, 200);
	stepsRest %= 300;
	
	
	/* final steps */
	
	/* read/write: 3 bytes */
	steps = stepsRest / 3;
	for (UINT i=0; i<steps; i++) {
		UCHAR data[3];
		fread(&data, 1, 3, infile);
		
		USHORT tmp[2];
		tmp[0] = data[0] ^ ((data[1] & 0x0F) << 8);
		tmp[1] = ((data[1] & 0xF0) >> 4) ^ (data[2] << 4);
		
		for (UINT i=0; i<2; i++) {
			tmp[i] = encode(&tmp[i]);
		}
		
		data[0] = tmp[0] & 0x00FF;
		data[1] = ((tmp[0] & 0xF00) >> 8) ^ ((tmp[1] & 0x000F) << 4);
		data[2] = (tmp[1] &0x0FF0) >> 4;
		
		if (fwrite(&data, 1, 3, outfile) <= 0) {
			printf("ERROR: cannot write into %s (step: %d, wordsteps).\n", outfileName, i);
			exit(EXIT_FAILURE);
		}
	}
	
	/* 0-2 times: read 1 byte; write 2 bytes */
	steps = stepsRest % 3;
	for (UINT i=0; i<steps; i++) {
		UCHAR data;
		fread(&data, 1, 1, infile);
		
		USHORT tmp = data;	// it 's also a cast to ushort
		tmp = encode(&tmp);
		
		if (fwrite(&tmp, 2, 1, outfile) <= 0) {
			printf("ERROR: cannot write into %s (step: %d, bytesteps).\n", outfileName, i);
			exit(EXIT_FAILURE);
		}
	}
	
	fflush(outfile);
	
	fclose(infile);
	fclose(outfile);
	
	exit(EXIT_SUCCESS);
}
