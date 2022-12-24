#include "../include/gly.h"
#include "../include/sse.h"


ushort encode(ushort *data)
{
	USHORT code = 0,
		   tmp;
	
	/* create and fill buffer */
	USHORT dataParity[2][8];
	USHORT dataParityShiftAnd[2][2][8][8];	// ii jj j i
	
	for (UINT i=0; i<6; i++) {
		dataParity[0][i] = (*data) & parity[i];
	}
	for (UINT i=0; i<6; i++) {
		dataParity[1][i] = (*data) & parity[add6[i]];
	}
	for (UINT i=0; i<6; i++) {
		for (UINT j=0; j<6; j++) {
			dataParityShiftAnd[0][0][j][i] = (dataParity[0][i] >> j) & 0x1;
		}
		for (UINT j=0; j<6; j++) {
			dataParityShiftAnd[0][1][j][i] = (dataParity[0][i] >> add6[j]) & 0x1;
		}
	}
	for (UINT i=0; i<6; i++) {
		for (UINT j=0; j<6; j++) {
			dataParityShiftAnd[1][0][j][i] = (dataParity[1][i] >> j) & 0x1;
		}
		for (UINT j=0; j<6; j++) {
			dataParityShiftAnd[1][1][j][i] = (dataParity[1][i] >> add6[j]) & 0x1;
		}
	}
	
	
	for (UINT i=0; i<12; i++) {
		tmp = 0;
		
		if (i < 6) {
			for (UINT j=0; j<6; j++) {
				tmp ^= dataParityShiftAnd[0][0][j][i];
			}
			for (UINT j=0; j<6; j++) {
				tmp ^= dataParityShiftAnd[0][1][j][i];
			}
		}
		else {
			for (UINT j=0; j<6; j++) {
				tmp ^= dataParityShiftAnd[1][0][j][sub6[i]];
			}
			for (UINT j=0; j<6; j++) {
				tmp ^= dataParityShiftAnd[1][1][j][sub6[i]];
			}
		}
		
		code <<= 1;
		code ^= tmp;
	}
	
	return(code);
}
