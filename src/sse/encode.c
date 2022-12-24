/*	sse
*/


#include "include/gly.h"
#include "include/sse.h"


ushort encode(ushort *data)
{
	USHORT code = 0,
		tmp;
	
	/* create and fill buffer */
	USHORT mDataParity[2][8],
		buffer[2][2][8][8];	// ii jj j i
	__m128i mParity[2] = {_mm_load_si128((__m128i *) parityM[0]), _mm_load_si128((__m128i *) parityM[1])},
		mOne = _mm_set1_epi16(0x1),
		mData = _mm_set1_epi16(*data);
	
	_mm_store_si128((__m128i *) mDataParity[0], _mm_and_si128(mData, mParity[0]));
	_mm_store_si128((__m128i *) mDataParity[1], _mm_and_si128(mData, mParity[1]));
	for (UINT j=0; j<6; j++) {
		_mm_store_si128((__m128i *) buffer[0][0][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[0]), j), mOne));
		_mm_store_si128((__m128i *) buffer[0][1][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[0]), add6[j]), mOne));
		_mm_store_si128((__m128i *) buffer[1][0][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[1]), j), mOne));
		_mm_store_si128((__m128i *) buffer[1][1][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[1]), add6[j]), mOne));
	}
	
	for (UINT i=0; i<12; i++) {
		tmp = 0;
		
		for (UINT j=0; j<12; j++) {
			tmp ^= buffer[indexGt6[i]][indexGt6[j]][sub6[j]][sub6[i]];
		}
		
		code <<= 1;
		code ^= tmp;
	}
	
	return(code);
}
