# Nuke built-in rules and variables.
MAKEFLAGS += -rR
.SUFFIXES:

PLATFORM = salernos-x86_64
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard ,$d, $2) $(filter $(subst *, %, $2),$d))

override OUTPUT := fresh

CC := cc
LD := ld
CFLAGS := -g -O2 -pipe
CPPFLAGS :=
LDFLAGS :=

override CFLAGS += \
		-I./ \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
		-fno-stack-protector \
		-fno-stack-check \
    -fno-lto \
    -fno-PIC \
		-mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -m64 \

override CPPFLAGS := \
    $(CPPFLAGS) \
    -MMD \
    -MP
#
override LDFLAGS += \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 

override CFILES = $(call rwildcard, fresh/, *c) $(call rwildcard, sysdeps/$(PLATFORM), *.c)
override ASFILES = $(call rwildcard, sysdeps/$(PLATFORM), *.S)
override OBJ = $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o))
override HEADER_DEPS = $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

# Default target.
.PHONY: all
all: bin/$(OUTPUT)

# Link rules for the final executable.
bin/$(OUTPUT): GNUmakefile $(OBJ)
	@echo [LD] Linking...
	@mkdir -p "$$(dirname $@)"
	@$(LD) $(OBJ) $(LDFLAGS) -o $@

# Include header dependencies.
-include $(HEADER_DEPS)

# Compilation rules for *.c files.
obj/%.c.o: %.c GNUmakefile
	@echo [CC] $<
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
obj/%.S.o: %.S GNUmakefile
	@echo [AS] $<
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf bin obj
