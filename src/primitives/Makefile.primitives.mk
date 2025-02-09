EE_OBJS=main.o
EE_CFLAGS += -Wall --std=c99
BROWSER=qutebrowser

all: compile run

compile: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)

.PHONY: run
run:
	pcsx2 -elf $(mkfile_dir)$(EE_BIN) -fullscreen -nogui

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
