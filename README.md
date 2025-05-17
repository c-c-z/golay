#gly

Golay Coder



Golay Coder for the Extended Golay with parity check aka (24, 12, 8).



Build/ Install:

	In general, to build use:

		make clean; make

	You can use your own CFLAGS by setting USERCFLAGS.


	The source is available in 4 versions.

		src/lut
			includes a coder using lookup tables to encode the data and for calculating the hamming weight,
			everthing else comes from the default version.

		src/sse
			includes a coder, using SSE and starting several de-/encode tasks.

		src/mix
			using look up tables for encoding and hamming weight calculation,
			using SSE for decoding,
			starts several decode processes and
			uses a single process to encode.

		src/def
			includes the default coder.


Usage:

	create a golay check file to <filename>:
		"glyenc     <filename> [--gly <outfile>]"

		"glyenc-def <filename> [--gly <outfile>]"
		"glyenc-lut <filename> [--gly <outfile>]"
		"glyenc-sse <filename> [--gly <outfile>]"

		If outfile will not be set, filename.gly is used.


	check/ repair <filename> with the help of <filename>.gly.
		"glydec     <datafile> [--gly <glyfile>] [--bak <bakfile>] [--tmp <tmpfile>]"

		"glydec-def <datafile> [--gly <glyfile>] [--bak <bakfile>] [--tmp <tmpfile>]"
		"glydec-lut <datafile> [--gly <glyfile>] [--bak <bakfile>] [--tmp <tmpfile>]"
		"glydec-sse <datafile> [--gly <glyfile>] [--bak <bakfile>] [--tmp <tmpfile>]"

		The original file will be safed as bakfile.
		If glyfile will not be set, filename.gly is used.
		If bakfile will not be set, filename.bak is used.
		If tmpfile will not be set, filename.tmp is used.


	Additional options:
		-h, --help, --usage	to show a short usage info.



	src/lt/crlt/
		type crlt.sh to create the look up tables
		(no need, they are included)



Description:

	Golay ist in der Lage innerhalb eines 12 Bitblocks
	bis zu 8 Bitfehler zu finden und bis zu 3 Bitfehler zu korrigieren.
	Damit bietet er sich, bis auf Wiederholungscoder, ungeschlagen zur Archivierung an.

	Der Encoder erstellt eine gly Datei, der Größe der originalen Datei
	plus 8-10 Byte bedingt durch die Implementierung.

	Die Ausgangsdatei bleibt unverändert!



Info:

	It bases on a work of Robert H. Morelos-Zaragoza (2002).



TODO:

	Translation
	Stream en-/decoding



Notes:

	Please, report bugs.
