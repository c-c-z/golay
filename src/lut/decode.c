/*	def
*/


#include "include/gly.h"
#include "include/def.h"


void decode(ushort *data, ushort *code)
{
	USHORT tmp,
		   tmpAnd,
		   syndrom = 0;
	UINT i;
	
	for (UINT j=0; j<12; j++) {
		tmp = 0;
		
		tmpAnd = identity[j] & (*data);
		for (i=0; i<12; i++) {
			tmp ^= (tmpAnd >> i) & 0x1;
		}
		
		tmpAnd = parity[j] & (*code);
		for (i=0; i<12; i++) {
			tmp ^= (tmpAnd >> i) & 0x01;
		}
		
		syndrom <<= 1;
		syndrom &= 0x0FFF;
		syndrom ^= tmp;
	}
	
	USHORT decData,
		   decCode;
	
	if (hammingweight(syndrom) <= 3) {
		decData = syndrom;
		decCode = 0;
	}
	else {
		i = 0;
		UCHAR found = 0;
		USHORT tmpSyndrom;
		
		while ((i < 12) && (!found)) {
			tmpSyndrom = syndrom ^ parity[i];
			if (hammingweight(tmpSyndrom) <= 2) {
				decData = tmpSyndrom;
				decCode = identity[i];
				found = 1;
			}
			i++;
		}
		
		if ((i==12) && (!found)) {
			USHORT modifiedSyndrom = 0;
			for (UINT j=0; j<12; j++) {
				tmp = 0;
				
				tmpAnd = parity[j] & syndrom;
				for (i=0; i<12; i++) {
					tmp ^= (tmpAnd >> i) & 0x1;
				}
				
				modifiedSyndrom <<= 1;
				modifiedSyndrom &= 0x0FFF;
				modifiedSyndrom ^= tmp;
			}
			
			if (hammingweight(modifiedSyndrom) <= 3) {
				decData = 0;
				decCode = modifiedSyndrom;
			}
			else {
				i = 0;
				found = 0;
				while ((i < 12) && (!found)) {
					tmpSyndrom = modifiedSyndrom ^ parity[i];
					if (hammingweight(tmpSyndrom) <= 2) {
						decData = identity[i];
						decCode = tmpSyndrom;
						found = 1;
					}
					i++;
				}
				
				if ((i == 12) && (!found)) {
					printf("WARNING: Uncorrectable error found.\n");
					printf("WARNING: Decoded file will still be broken.\n");
					return;
				}
			}
		}
	}
	
	*data ^= decData;
	*code ^= decCode;
	return;
}
