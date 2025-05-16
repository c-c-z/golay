/*	gnrc
*/


#include "include/gly.h"


void _msgShortUsage(void)
{
	printf("Usage:\tglydec <datafile> [--gly <glyfile>]  [--bak <bakfile>]  [--tmp <tmpfile>].\n\n");
	return;
}


void _msgUsage(void)
{
	_msgShortUsage();
	printf("\tIf glyfile will be unset, using datafile.gly.\n");
	printf("\tIf bakfile will be unset, using datafile.bak.\n");
	printf("\tIf tmpfile will be unset, using datafile.tmp.\n\n");
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
	
	char datafileName[1024],
		codefileName[1024],
		tmpfileName[1024],
		bakfileName[1024];
	
	for (SINT i=1; i<argc; i++) {
		if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
			_msgUsage();
			exit(EXIT_SUCCESS);
		}
		else if (strcmp(argv[i], "--usage") == 0) {
			_msgShortUsage();
			exit(EXIT_SUCCESS);
		}
		else if (strcmp(argv[i], "--gly") == 0) {
			if (strlen(argv[++i]) != 0) {
				strcpy(codefileName, argv[i]);
			}
			else {
				_msgUsage();
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(argv[i], "--bak") == 0) {
			if (strlen(argv[++i]) != 0) {
				strcpy(bakfileName, argv[i]);
			}
			else {
				_msgUsage();
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(argv[i], "--tmp") == 0) {
			if (strlen(argv[++i]) != 0) {
				strcpy(tmpfileName, argv[i]);
			}
			else {
				_msgUsage();
				exit(EXIT_FAILURE);
			}
		}
		else {
			strcpy(datafileName, argv[1]);
		}
	}
	
	if (strstr(datafileName, charNull) == NULL) {
		_msgUsage();
		exit(EXIT_FAILURE);
	}
	
	if (strlen(codefileName) == 0) {
		strcpy(codefileName, datafileName);
	}
	if (strstr(codefileName, fileEnding) == NULL) {
		strcat(codefileName, fileEnding);
	}
	
	if (strlen(tmpfileName) == 0) {
		strcpy(tmpfileName, datafileName);
		strcat(tmpfileName, ".tmp");
	}
	
	if (strlen(bakfileName) == 0) {
		strcpy(bakfileName, datafileName);
		strcat(bakfileName, ".bak");
	}
	
	
	FILE *datafile = fopen(datafileName, "rb");
	if (!datafile) {
		printf("ERROR: cannot open datafile %s.\n", datafileName);
		_msgShortUsage();
		exit(EXIT_FAILURE);
	}
	fseek(datafile, 0, SEEK_SET);
	
	FILE *codefile = fopen(codefileName, "rb");
	if (!codefile) {
		printf("ERROR: cannot open codefile %s.\n", codefileName);
		_msgShortUsage();
		exit(EXIT_FAILURE);
	}
	fseek(codefile, 0, SEEK_SET);
	
	
	FILE *tmpfile = fopen(tmpfileName, "wb");
	if (!tmpfile) {
		printf("ERROR: cannot open tmpfile %s.\n", tmpfileName);
		exit(EXIT_FAILURE);
	}
	fseek(tmpfile, 0, SEEK_SET);
	
	
	decodeFile(datafile, codefile, tmpfile, getFilesize(datafile, codefile));
	
	
	fflush(tmpfile);
	
	fclose(datafile);
	fclose(codefile);
	fclose(tmpfile);
	
	
	/* Linux only code? */
	char cmd0[2560] = "mv ";
	strcat(cmd0, datafileName);
	strcat(cmd0, " ");
	strcat(cmd0, bakfileName);
	if (system(cmd0) < 0) {
		printf("ERROR: cannot mv %s to %s.\n", datafileName, bakfileName);
		exit(EXIT_FAILURE);
	}
	
	/* Linux only code? */
	char cmd1[2560] = "mv ";
	strcat(cmd1, tmpfileName);
	strcat(cmd1, " ");
	strcat(cmd1, datafileName);
	if (system(cmd1) < 0) {
		printf("ERROR: cannot mv %s to %s.\n", tmpfileName, datafileName);
		exit(EXIT_FAILURE);
	}
	
	
	exit(EXIT_SUCCESS);
}
