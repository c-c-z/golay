#!/bin/bash


export USERCFLAGS="-m32 -march=i686 -mtune=generic"
for d in def lut
do
	( cd ../$d
		make clean
		make -j2
		make clean
	)
done

export USERCFLAGS="-m32 -march=pentium4 -mtune=generic"
for d in sse mix
do
	( cd ../$d
		make clean
		make -j2
		make clean
	)
done

mkdir -p ../../bin32
mv -v ../../bin/* ../../bin32/


export USERCFLAGS="-fPIC -m64 -march=nocona -mtune=generic"
for d in def lut sse mix
do
	( cd ../$d
		make clean
		make -j2
		make clean
	)
done

mkdir -p ../../bin64
mv -v ../../bin/* ../../bin64/
