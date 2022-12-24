/*	def
*/


#include "include/gly.h"
#include "include/def.h"


ushort hammingweight(const ushort a)
{
	USHORT count = 0;
	
	for (UINT i=0; i<12; i++) {
		if ((a >> i) & 0x1) {
			count++;
		}
	}
	
	return(count);
}
