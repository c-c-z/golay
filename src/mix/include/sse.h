#include <smmintrin.h>



/* Memory optimised parity matrix for sse */
static const USHORT parityM[2][8] = {
	{0x7ff, 0xee2, 0xdc5, 0xb8b, 0xf16, 0xe2d, 0, 0},
	{0xc5b, 0x8b7, 0x96e, 0xadc, 0xdb8, 0xb71, 0, 0}};

/* Memory optimised identity matrix for sse */
static const USHORT identityM[2][8] = {
	{0x800, 0x400, 0x200, 0x100, 0x080, 0x040, 0, 0},
	{0x020, 0x010, 0x008, 0x004, 0x002, 0x001, 0, 0}};



/* Lookup table to prevent addition btw. substraction by 6 from 0 btw. 12 */
static const USHORT add6[6] = {6, 7, 8, 9, 10, 11};
static const USHORT sub6[12] = {0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5};
static const USHORT indexGt6[12] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
