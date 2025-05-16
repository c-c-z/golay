/*	gnrc
*/


#include "include/gly.h"


void _msgShortUsage(void)
{
	printf("Usage:\tglyenc <infile> [--gly <outfile>].\n\n");
	return;
}


void _msgUsage(void)
{
	_msgShortUsage();
	printf("\tIf outfile will be unset, using infile.gly.\n\n");
	printf("\t-h and --help print this message.\n");
	printf("\t--usage shows an usage info.\n");
	printf("\tVersion: %s.\n\n", _glyvers);
	return;
}


sint main(sint argc, char **argv)
{
	if (argc < 0) {
		_msgUsage();
		exit(EXIT_FAILURE);
	}
	
	char infileName[1024],
		outfileName[1024];
	
	for (SINT i=1; i<argc; i++) {
		if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
			_msgUsage();
			exit(EXIT_SUCCESS);
		}
		else {
		if (strcmp(argv[i], "--usage") == 0) {
			_msgShortUsage();
			exit(EXIT_SUCCESS);
		}
		else {
		if (strcmp(argv[i], "--gly") == 0) {
			UINT ii = i + 1;
			if (strlen(argv[ii]) != 0) {
				strcpy(outfileName, argv[++i]);
			}
			else {
				_msgUsage();
				exit(EXIT_FAILURE);
			}
		}
		else {
			strcpy(infileName, argv[1]);
		}
		}
		}
	}
	
	if (strstr(infileName, charNull) == NULL) {
		_msgUsage();
		exit(EXIT_FAILURE);
	}
	
	if (strlen(outfileName) == 0) {
		strcpy(outfileName, infileName);
	}
	if (strstr(outfileName, fileEnding) == NULL) {
		strcat(outfileName, fileEnding);
	}
	
	
	FILE *infile = fopen(infileName, "rb");
	if (!infile) {
		printf("ERROR: cannot open infile %s.\n", infileName);
		_msgShortUsage();
		exit(EXIT_FAILURE);
	}
	fseek(infile, 0, SEEK_SET);
	
	FILE *outfile = fopen(outfileName, "wb");
	if (!outfile) {
		printf("ERROR: cannot open outfile %s.\n", outfileName);
		exit(EXIT_FAILURE);
	}
	fseek(outfile, 0, SEEK_SET);
	
	
	encodeFile(infile, outfile, setFilesize(infile, outfile));
	
	
	fflush(outfile);
	
	fclose(infile);
	fclose(outfile);
	
	
	exit(EXIT_SUCCESS);
}
