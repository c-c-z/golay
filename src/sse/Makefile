RM=rm -f
STRIP=strip
MKDIR=mkdir -p
CFLAGS=-Wall -O2 -fopenmp -fomit-frame-pointer -funsigned-char -msse -msse2 $(USERCFLAGS)
#CPPFLAGS=-g
LDFLAGS=-Wl,-O1
INCLUDES=-Iinclude

OBJS= \
	encode.o \
	set_filesize.o \
	encode_file.o \
	glyenc.o

OBJS_1= \
	hammingweight.o \
	decode.o \
	get_filesize.o \
	decode_file.o \
	glydec.o


default: glydec-sse glyenc-sse

all: default

clean:
	$(RM) $(OBJS) $(OBJS_1)

glyenc-sse: $(OBJS)
	$(MKDIR) ../../bin
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $^ -o ../../bin/$@
	$(STRIP) -s ../../bin/$@

glydec-sse: $(OBJS_1)
	$(MKDIR) ../../bin
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $^ -o ../../bin/$@
	$(STRIP) -s ../../bin/$@

distclean: clean
	$(RM) ../../bin/glydec-sse ../../bin/glyenc-sse
