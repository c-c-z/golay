#!/bin/bash


#export USERCFLAGS="-O3 -fno-tree-loop-vectorize -fno-tree-vectorize -march=native -mtune=native"	# when using gcc
export USERCFLAGS="-march=native -mtune=native"

for d in def lut sse mix
do
	( cd ../$d
		make clean
		make -j2
		make clean
	)
done
