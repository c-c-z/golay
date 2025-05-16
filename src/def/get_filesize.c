/*	gnrc
*/


#include "include/gly.h"


void _msg(char *line1, char *line2)
{
	printf("%s\n", line1);
	printf("%s\n", line2);
	return;
}


void _msgStoredIncorrect(void)
{
	_msg("INFO: Stored file size isn't correct anymore.", "INFO: Please, re-encode the original file.");
	return;
}


void _msgRealIncorrect(void)
{
	_msg("WARNING: File isn't correctly sized anymore", "INFO: It may be unrepairable.");
	return;
}


void _msgUsingGly(void)
{
	_msg("INFO: Calculating file size by gly file.", "INFO: The file may be unrepairable.");
	return;
}


/* soft decision
	0	real file size
	1	stored file size
	2	file size calculated by gly file size
	x	assuming the lowest filesize as correct
*/
ulong getFilesize(FILE *datafile, FILE *codefile)
{
	ULONG fileSize[3];
	fread(&fileSize[0], 8, 1, codefile);
	
	fseek(datafile, 0, SEEK_END);
	fileSize[1] = ftell(datafile);
	fseek(datafile, 0, SEEK_SET);
	
	if (fileSize[0] == fileSize[1]) {
		return(fileSize[0]);
	}
	else {
		fseek(codefile, 0, SEEK_END);
		fileSize[2] = ftell(codefile);
		fseek(codefile, 0, SEEK_SET);
		ULONG tmp = fileSize[2] - 8;
		if (tmp == fileSize[1]) {
			_msgStoredIncorrect();
			return(fileSize[1]);
		}
		else if (tmp == fileSize[0]) {
			_msgRealIncorrect();
			return(fileSize[0]);
		}
		else if ((tmp % 3) == 0) {
			_msgUsingGly();
			return(tmp);
		}
		else {
			tmp--;
			if (tmp == fileSize[1]) {
				_msgStoredIncorrect();
				return(fileSize[1]);
			}
			else if (tmp == fileSize[0]) {
				_msgRealIncorrect();
				return(fileSize[0]);
			}
			else if ((tmp % 3) == 0) {
				_msgUsingGly();
				return(tmp);
			}
			else {
				tmp--;
				if (tmp == fileSize[1]) {
					_msgStoredIncorrect();
					return(fileSize[1]);
				}
				else if (tmp == fileSize[0]) {
					_msgRealIncorrect();
					return(fileSize[0]);
				}
				else if ((tmp % 3) == 0) {
					_msgUsingGly();
					return(tmp);
				}
				else {
					printf("WARNING: Assuming lowest real file size as correct.\n");
					printf("INFO: The file may be unrepairable.\n");
					if (fileSize[1] < fileSize[2]) {
						return(fileSize[1]);
					}
					else {
						return(fileSize[2]);
					}
				}
			}
		}
	}
}
