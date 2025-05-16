/*	gnrc
*/


#include "include/gly.h"


ulong setFilesize(FILE *infile, FILE *outfile)
{
	fseek(infile, 0, SEEK_END);
	ULONG filesize = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	
	
	if (fwrite(&filesize, 8, 1, outfile) <= 0) {
		printf("ERROR: cannot write filesize into file.\n");
		exit(EXIT_FAILURE);
	}
	
	fflush(outfile);
	
	
	return filesize;
}
