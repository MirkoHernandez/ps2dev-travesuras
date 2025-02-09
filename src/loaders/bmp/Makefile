EE_BIN=loader.elf
DOCUMENT=group__loaders.html
EE_LIBS=-ldma -lgraph -ldraw -lpacket -lmath3d -lkernel

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

include ../Makefile.loaders.mk
