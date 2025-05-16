RM=rm -f
STRIP=strip
MKDIR=mkdir -p
#-Wno-aggressive-loop-optimizations
CFLAGS=-Wall -O2 -funroll-loops -fomit-frame-pointer -funsigned-char $(USERCFLAGS)
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

default: glydec-lut glyenc-lut

all: default

clean:
	$(RM) $(OBJS) $(OBJS_1)

glyenc-lut: $(OBJS)
	$(MKDIR) ../../bin
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $^ -o ../../bin/$@
	$(STRIP) -s ../../bin/$@

glydec-lut: $(OBJS_1)
	$(MKDIR) ../../bin
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $^ -o ../../bin/$@
	$(STRIP) -s ../../bin/$@

distclean: clean
	$(RM) ../../bin/glydec-lut ../../bin/glyenc-lut
