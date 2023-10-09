include setup.mk

CFLAGS          = -Os -Wall -Wno-unused-result -pthread -DEXTLIB=212
SOURCE_C       := $(shell find src/* -maxdepth 0 -type f -name '*.c')
SOURCE_O_WIN32 := $(foreach f,$(SOURCE_C:.c=.o),bin/win32/$f)
SOURCE_O_LINUX := $(foreach f,$(SOURCE_C:.c=.o),bin/linux/$f)

.PHONY: clean default linux win32

default: linux

include $(PATH_EXTLIB)ext_lib.mk

# Make build directories
$(shell mkdir -p bin/ $(foreach dir, \
	$(dir $(SOURCE_O_WIN32)) \
	$(dir $(SOURCE_O_LINUX)), $(dir)))

all: linux win32
linux: $(SOURCE_O_LINUX) z64audio
win32: $(SOURCE_O_WIN32) z64audio.exe

clean:
	@rm -f z64audio *.exe
	@rm -f -R bin/*

# # # # # # # # # # # # # # # # # # # #
# LINUX BUILD                         #
# # # # # # # # # # # # # # # # # # # #

-include $(SOURCE_O_LINUX:.o=.d)

bin/linux/%.o: %.c
	@echo "$(PRNT_RSET)$(PRNT_RSET)[$(PRNT_CYAN)$(notdir $@)$(PRNT_RSET)]"
	@gcc -c $(CFLAGS) -o $@ $<
	$(GD_LINUX)

z64audio: $(SOURCE_O_LINUX) $(ExtLib_Linux_O) $(Mp3_Linux_O) $(Audio_Linux_O)
	@echo "$(PRNT_RSET)$(PRNT_RSET)[$(PRNT_CYAN)$(notdir $@)$(PRNT_RSET)] [$(PRNT_CYAN)$(notdir $^)$(PRNT_RSET)]"
	@gcc -o $@ $^ $(CFLAGS) -lm -ldl

# # # # # # # # # # # # # # # # # # # #
# WIN32 BUILD                         #
# # # # # # # # # # # # # # # # # # # #

-include $(SOURCE_O_WIN32:.o=.d)

bin/win32/%.o: %.c
	@echo "$(PRNT_RSET)$(PRNT_RSET)[$(PRNT_CYAN)$(notdir $@)$(PRNT_RSET)]"
	@i686-w64-mingw32.static-gcc -c -o $@ $< $(CFLAGS) -D_WIN32
	$(GD_WIN32)

z64audio.exe: bin/win32/icon.o $(SOURCE_O_WIN32) $(ExtLib_Win32_O) $(Mp3_Win32_O) $(Audio_Win32_O)
	@echo "$(PRNT_RSET)$(PRNT_RSET)[$(PRNT_CYAN)$(notdir $@)$(PRNT_RSET)] [$(PRNT_CYAN)$(notdir $^)$(PRNT_RSET)]"
	@i686-w64-mingw32.static-gcc -o $@ $^ $(CFLAGS) -lm -D_WIN32
