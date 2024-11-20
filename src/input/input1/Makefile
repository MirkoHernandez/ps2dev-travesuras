EE_BIN=input1.elf
EE_OBJS=main.o
EE_LIBS=-ldma -lgraph -ldraw  -lpad -lc
EE_CFLAGS += -Wall --std=c99

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

BROWSER=qutebrowser
DOCUMENT=input_2input1_2main_8c.html

all: compile

compile: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)

.PHONY: run
run:
	pcsx2 -elf $(mkfile_dir)/$(EE_BIN)


.PHONY: docs
docs:
	(cd ../../.. && doxygen Doxyfile)

browse: docs
	$(BROWSER) doxygen/html/$(DOCUMENT)


.PHONY: clean
clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.eeglobal
include $(PS2SDK)/samples/Makefile.pref

