EE_BIN=sprite.elf
EE_OBJS=main.o
EE_LIBS=-ldma -lgraph -ldraw -lpacket -lmath3d 
EE_CFLAGS += -Wall --std=c99

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

BROWSER=qutebrowser

all: compile run

compile: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)

.PHONY: run
run:
	pcsx2 --elf=$(mkfile_dir)/$(EE_BIN) --fullscreen --nogui

.PHONY: docs
docs: 
	(cd ../../.. && doxygen Doxyfile && $(BROWSER) doxygen/html/main_8c.html)

.PHONY: clean
clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.eeglobal
include $(PS2SDK)/samples/Makefile.pref
