#include "../../include/gly.h"


int main (void)
{
	printf("static const USHORT d2c[4096] = {\n");
	USHORT i;
	printf("\t");
	for (i=0; i<4095; i++) {
		printf("0x%3x, ", encode(&i));
	}
	i = 4095;
	printf("0x%3x\n};\n\n", encode(&i));
	

	printf("static const USHORT hammingWeight[4096] = {\n");
	printf("\t");
	for (i=0; i<4095; i++) {
		printf("0x%3x, ", hammingweight(i));
	}
	i = 4095;
	printf("0x%3x\n};\n", hammingweight(i));
	
	
	return (EXIT_SUCCESS);
}
