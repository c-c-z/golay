/*	sse
*/


#include "include/gly.h"
#include "include/sse.h"


ushort hammingweight(const ushort a)
{
	USHORT count = 0;
	
	__m128i mmAndO = _mm_set1_epi16(0x1),
		mmA[2] = {_mm_setr_epi16(a, a >> 1, a >> 2, a >> 3, a >> 4, a >> 5, 0, 0), _mm_setr_epi16(a >> 6, a >> 7, a >> 8, a >> 9, a >> 10, a >> 11, 0, 0)};
	USHORT mAnd[2][8];
	
	for (UINT i=0; i<2; i++) {
		_mm_store_si128((__m128i *) mAnd[i], _mm_and_si128(mmA[i], mmAndO));
	}
	
	for (UINT i=0; i<12; i++) {
		if (mAnd[indexGt6[i]][sub6[i]]) {
			count++;
		}
	}
	
	return(count);
}
