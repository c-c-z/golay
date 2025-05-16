/*	sse
*/


#include "include/gly.h"
#include "include/sse.h"


void decode(ushort *data, ushort *code)
{
	USHORT tmp,
		syndrom = 0,
		mDataIdentity[2][8],
		mDataParity[2][8],
		bufferIdentity[2][2][8][8],	// jj ii i j
		bufferParity[2][2][8][8];
	__m128i mIdentity[2] = {_mm_load_si128((__m128i *) identityM[0]), _mm_load_si128((__m128i *) identityM[1])},
		mParity[2] = {_mm_load_si128((__m128i *) parityM[0]), _mm_load_si128((__m128i *) parityM[1])},
		mOne = _mm_set1_epi16(0x1),
		mData = _mm_set1_epi16(*data),
		mCode = _mm_set1_epi16(*code);
	
	_mm_store_si128((__m128i *) mDataIdentity[0], _mm_and_si128(mData, mIdentity[0]));
	_mm_store_si128((__m128i *) mDataIdentity[1], _mm_and_si128(mData, mIdentity[1]));
	_mm_store_si128((__m128i *) mDataParity[0], _mm_and_si128(mCode, mParity[0]));
	_mm_store_si128((__m128i *) mDataParity[1], _mm_and_si128(mCode, mParity[1]));
	for (UINT j=0; j<6; j++) {
		_mm_store_si128((__m128i *) bufferIdentity[0][0][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataIdentity[0]), j), mOne));
		_mm_store_si128((__m128i *) bufferIdentity[0][1][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataIdentity[0]), add6[j]), mOne));
		_mm_store_si128((__m128i *) bufferIdentity[1][0][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataIdentity[1]), j), mOne));
		_mm_store_si128((__m128i *) bufferIdentity[1][1][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataIdentity[1]), add6[j]), mOne));
		_mm_store_si128((__m128i *) bufferParity[0][0][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[0]), j), mOne));
		_mm_store_si128((__m128i *) bufferParity[0][1][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[0]), add6[j]), mOne));
		_mm_store_si128((__m128i *) bufferParity[1][0][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[1]), j), mOne));
		_mm_store_si128((__m128i *) bufferParity[1][1][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[1]), add6[j]), mOne));
	}
	
	for (UINT j=0; j<12; j++) {
		tmp = 0;
		
		for (UINT i=0; i<12; i++) {
			tmp ^= bufferIdentity[indexGt6[j]][indexGt6[i]][sub6[i]][sub6[j]];
		}
		for (UINT i=0; i<12; i++) {
			tmp ^= bufferParity[indexGt6[j]][indexGt6[i]][sub6[i]][sub6[j]];
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
		UINT i = 0;
		UCHAR found = 0;
		USHORT tmpSyndrom;
		
		while ((i < 12) && (!found)) {
			tmpSyndrom = syndrom ^ parityM[indexGt6[i]][sub6[i]];
			if (hammingweight(tmpSyndrom) <= 2) {
				decData = tmpSyndrom;
				decCode = identityM[indexGt6[i]][sub6[i]];
				found = 1;
			}
			i++;
		}
		
		if ((i==12) && (!found)) {
			USHORT modifiedSyndrom = 0;
			mData = _mm_set1_epi16(syndrom);
			
			_mm_store_si128((__m128i *) mDataParity[0], _mm_and_si128(mData, mParity[0]));
			_mm_store_si128((__m128i *) mDataParity[1], _mm_and_si128(mData, mParity[1]));
			for (UINT j=0; j<6; j++) {
				_mm_store_si128((__m128i *) bufferParity[0][0][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[0]), j), mOne));
				_mm_store_si128((__m128i *) bufferParity[0][1][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[0]), add6[j]), mOne));
				_mm_store_si128((__m128i *) bufferParity[1][0][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[1]), j), mOne));
				_mm_store_si128((__m128i *) bufferParity[1][1][j], _mm_and_si128(_mm_srli_epi16(_mm_load_si128((__m128i *) mDataParity[1]), add6[j]), mOne));
			}
			
			for (UINT j=0; j<12; j++) {
				tmp = 0;
				
				for (i=0; i<12; i++) {
					tmp ^= bufferParity[indexGt6[j]][indexGt6[i]][sub6[i]][sub6[j]];
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
					tmpSyndrom = modifiedSyndrom ^ parityM[indexGt6[i]][i];
					if (hammingweight(tmpSyndrom) <= 2) {
						decData = identityM[indexGt6[i]][sub6[i]];
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
