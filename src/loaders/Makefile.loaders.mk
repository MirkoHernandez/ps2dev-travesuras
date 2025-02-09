EE_OBJS=main.o  $(PS2DEV)/gsKit/lib/libgskit.a
EE_CFLAGS += -Wall --std=c99 $(DVARS) 

BROWSER=qutebrowser

all:
	$(MAKE) compile || exit 1 
	$(MAKE) run

compile: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN) 

.PHONY: run
run:
	pcsx2 -elf $(mkfile_dir)$(EE_BIN) 

.PHONY: docs

docs: 
	(cd ../../.. && doxygen Doxyfile)

browse: docs
	(cd ../../.. &&  $(BROWSER) doxygen/html/$(DOCUMENT))

.PHONY: clean
clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.eeglobal
include $(PS2SDK)/samples/Makefile.pref
