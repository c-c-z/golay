/*	def
*/


#include "include/gly.h"
#include "include/def.h"


ushort encode(ushort *data)
{
	USHORT code = 0,
		   tmp,
		   tmpAnd;
	
	for (UINT i=0; i<12; i++) {
		tmp = 0;
		
		tmpAnd = (*data) & parity[i];
		for (UINT j=0; j<12; j++) {
			tmp ^= (tmpAnd >> j) & 0x1;
		}
		
		code <<= 1;
		code ^= tmp;
	}
	
	return(code);
}
