#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>



static const char _glyvers[6] = "0.9.0";



typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef signed long slong;
typedef signed int sint;
typedef signed short sshort;
typedef signed char schar;



#ifdef __X86_64__
typedef __uint64_t ulong;
#endif
typedef __uint32_t uint;
typedef __uint16_t ushort;
typedef __uint8_t uchar;
typedef __int32_t sint;



#define ULONG \
    __attribute__((aligned(8))) ulong
#define UINT \
    __attribute__((aligned(4))) uint
#define USHORT \
    __attribute__((aligned(2))) ushort
#define UCHAR \
    uchar
#define SINT \
    __attribute__((aligned(4))) sint



static const char fileEnding[5] = ".gly";
static const char charNull[1] = "\0";



ushort hammingweight(const ushort a);
void decode(ushort *data, ushort *code);
ulong getFilesize(FILE *datafile, FILE *codefile);
void decodeFile(FILE *datafile, FILE *codefile, FILE *outfile, const ulong filesize);


ushort encode(ushort *data);
ulong setFilesize(FILE *infile, FILE *outfile);
void encodeFile(FILE *infile, FILE *outfile, const ulong filesize);
