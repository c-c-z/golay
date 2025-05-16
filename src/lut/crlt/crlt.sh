#!/bin/bash


set -e


FLAGS="-m64 -march=native -mtune=native"


gcc \
	-Wall \
	-O3 -funroll-loops -fomit-frame-pointer \
	-funsigned-char -funsigned-bitfields \
	-mtune=generic \
	$FLAGS \
	../../std/hammingweight.c \
	-msse -msse2 ../../sse/encode.c \
	crlt.c -o crlt
strip --strip-debug --strip-unneeded crlt


./crlt > ../include/lt.h

sed -i 's|0x  |0x00|g' ../include/lt.h
sed -i 's|0x |0x0|g' ../include/lt.h

rm ./crlt


exit 0
