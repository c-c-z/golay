/*	sse
*/


#include "include/gly.h"
#include "include/sse.h"
#ifndef _USE_HAMMINGWEIGHTLT
#include <omp.h>
#endif


void _encodeFile(FILE *infile, FILE *outfile, const ulong readSteps, const uint dataVars, const uint codeVars)
{
	for (ULONG i=0; i<readSteps; i++) {
		USHORT data[dataVars];
		fread(&data, 2 /* sizeof(ushort) */, dataVars, infile);
		
		USHORT tmp[codeVars],
			mXorShiftAnd[8];
		UINT j = 0,
			k = 0,
			jk = 0,
			mAdd[2][4];
		__m128i mmAddO[2] = {_mm_setr_epi32(1, 1, 2, 3), _mm_setr_epi32(0, 1, 2, 3)};
		while (j < dataVars) {
			jk = j + k;
			_mm_store_si128((__m128i *) mAdd[0], _mm_add_epi32(_mm_setr_epi32(k, jk, jk, jk), mmAddO[0]));
			_mm_store_si128((__m128i *) mAdd[1], _mm_add_epi32(_mm_set1_epi32(j), mmAddO[1]));
			_mm_store_si128((__m128i *) mXorShiftAnd, _mm_and_si128(_mm_setr_epi16(data[j], data[j], data[mAdd[1][1]], data[mAdd[1][1]], data[mAdd[1][2]], data[mAdd[1][2]], 0, 0), _mm_setr_epi16(0x0FFF, 0xF000, 0x00FF, 0xFF00, 0x000F, 0xFFF0, 0, 0)));
			mXorShiftAnd[1] >>= 12;
			mXorShiftAnd[2] <<= 4;
			mXorShiftAnd[3] >>= 8;
			mXorShiftAnd[4] <<= 8;
			mXorShiftAnd[5] >>= 4;
			tmp[jk] = mXorShiftAnd[0];
			tmp[mAdd[0][3]] = mXorShiftAnd[5];
			_mm_store_si128((__m128i *) mXorShiftAnd, _mm_xor_si128(_mm_setr_epi16(mXorShiftAnd[1], mXorShiftAnd[3], 0, 0, 0, 0, 0, 0), _mm_setr_epi16(mXorShiftAnd[2], mXorShiftAnd[4], 0, 0, 0, 0, 0, 0)));
			tmp[mAdd[0][1]] = mXorShiftAnd[0];
			tmp[mAdd[0][2]] = mXorShiftAnd[1];
			j = mAdd[1][3];
			k = mAdd[0][0];
		}
		
#ifndef _USE_HAMMINGWEIGHTLT
		#pragma omp parallel for schedule(dynamic) shared(tmp)
#endif
		for (UINT l=0; l<codeVars; l++) {
			tmp[l] = encode(&tmp[l]);
		}
		
		j = 0;
		k = 0;
		jk = 0;
		while (j < dataVars) {
			jk = j + k;
			_mm_store_si128((__m128i *) mAdd[0], _mm_add_epi32(_mm_setr_epi32(k, j, j, j), mmAddO[0]));
			_mm_store_si128((__m128i *) mAdd[1], _mm_add_epi32(_mm_set1_epi32(jk), mmAddO[1]));
			_mm_store_si128((__m128i *) mXorShiftAnd, _mm_and_si128(_mm_setr_epi16(tmp[mAdd[1][1]], tmp[mAdd[1][1]], tmp[mAdd[1][2]], tmp[mAdd[1][2]], tmp[mAdd[1][3]], 0, 0, 0), _mm_setr_epi16(0x000F, 0x0FF0, 0x00FF, 0x0F00, 0x0FFF, 0, 0, 0)));
			mXorShiftAnd[0] <<= 12;
			mXorShiftAnd[1] >>= 4;
			mXorShiftAnd[2] <<= 8;
			mXorShiftAnd[3] >>= 8;
			mXorShiftAnd[4] <<= 4;
			_mm_store_si128((__m128i *) mXorShiftAnd, _mm_xor_si128(_mm_setr_epi16(tmp[mAdd[1][0]], mXorShiftAnd[1], mXorShiftAnd[3], 0, 0, 0, 0, 0), _mm_setr_epi16(mXorShiftAnd[0], mXorShiftAnd[2], mXorShiftAnd[4], 0, 0, 0, 0, 0)));
			data[j] = mXorShiftAnd[0];
			data[mAdd[0][1]] = mXorShiftAnd[1];
			data[mAdd[0][2]] = mXorShiftAnd[2];
			j = mAdd[0][3];
			k = mAdd[0][0];
		}
		
		if (fwrite(&data, 2, dataVars, outfile) <= 0) {
			printf("ERROR: cannot write into outfile (step: %lu of %lu).\n", i, readSteps);
			exit(EXIT_FAILURE);
		}
	}
	return;
}


void encodeFile(FILE *infile, FILE *outfile, const ulong filesize)
{
	/* read/encode/write: 3.000.000 bytes */
	ULONG steps = filesize / 3000000;
	_encodeFile(infile, outfile, steps, 1500000, 2000000);
	ULONG stepsRest = filesize % 3000000;
	
	/* read/write 300.000 bytes */
	steps = stepsRest / 300000;
	_encodeFile(infile, outfile, steps, 150000, 200000);
	stepsRest %= 300000;
	
	/* read/write 300 bytes */
	steps = stepsRest / 300;
	_encodeFile(infile, outfile, steps, 150, 200);
	stepsRest %= 300;
	
	
	/* final steps */
	
	/* read/write: 3 bytes */
	steps = stepsRest / 3;
	for (ULONG i=0; i<steps; i++) {
		UCHAR data[3];
		fread(&data, 1, 3, infile);
		
		USHORT tmp[2],
			mXorAnd[8];
		__m128i mmAnd = _mm_setr_epi16(0x0F, 0xF0, 0, 0, 0, 0, 0, 0);
		_mm_store_si128((__m128i *) mXorAnd, _mm_and_si128(_mm_setr_epi16(data[1], data[1], 0, 0, 0, 0, 0, 0), mmAnd));
		mXorAnd[0] <<= 8;
		mXorAnd[1] >>= 4;
		mXorAnd[2] = data[2] << 4;
		_mm_store_si128((__m128i *) mXorAnd, _mm_xor_si128(_mm_setr_epi16(data[0], mXorAnd[1], 0, 0, 0, 0, 0, 0), _mm_setr_epi16(mXorAnd[0], mXorAnd[2], 0, 0, 0, 0, 0, 0)));
		for (UINT j=0; j<2; j++) {
			tmp[j] = mXorAnd[j];
		}
		
		for (UINT j=0; j<2; j++) {
			tmp[j] = encode(&tmp[j]);
		}
		
		mmAnd = _mm_setr_epi16(0x00FF, 0x0F00, 0x000F, 0x0FF0, 0, 0, 0, 0);
		_mm_store_si128((__m128i *) mXorAnd, _mm_and_si128(_mm_setr_epi16(tmp[0], tmp[0], tmp[1], tmp[1], 0, 0, 0, 0), mmAnd));
		data[0] = mXorAnd[0];
		data[1] = (mXorAnd[1] >> 8) ^ (mXorAnd[2] << 4);
		data[2] = mXorAnd[3] >> 4;
		
		if (fwrite(&data, 1, 3, outfile) <= 0) {
			printf("ERROR: cannot write into outfile (step: %lu, wordsteps).\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	/* 0-2 times: read 1 byte; write 2 bytes */
	steps = stepsRest % 3;
	for (UINT i=0; i<steps; i++) {
		UCHAR data;
		fread(&data, 1, 1, infile);
		
		USHORT tmp = data;	// it 's also a cast to ushort
		tmp = encode(&tmp);
		
		if (fwrite(&tmp, 2, 1, outfile) <= 0) {
			printf("ERROR: cannot write into outfile (step: %d, bytesteps).\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	fflush(outfile);
	
	return;
}
