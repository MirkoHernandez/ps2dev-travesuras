EE_BIN=wav.elf
DOCUMENT= group__audio.html
EE_LIBS=-ldma -lgraph -ldraw -lpacket -lkernel -laudsrv

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

include ../Makefile.audio.mk

# Se necesita copiar este archivo.
irx:
	cp $(PS2SDK)/iop/irx/audsrv.irx audsrv.irx


