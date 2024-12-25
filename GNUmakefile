# Nuke built-in rules and variables.
MAKEFLAGS += -rR
.SUFFIXES:

PLATFORM = salernos-x86_64
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard ,$d, $2) $(filter $(subst *, %, $2),$d))

CC := cc
LD := ld
CFLAGS := -g -O2 -pipe
CPPFLAGS :=
LDFLAGS :=

override CFLAGS += \
		-I./ \
		-I./subprojects/ \
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
		-mno-red-zone \
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

override SYSDEPS_CFILES = $(call rwildcard, sysdeps/$(PLATFORM)/, *.c) 
override CRT_CFILES = $(call rwildcard, crt/, *.c)
override CFILES = $(call rwildcard, subprojects/$(OUTPUT)/, *.c)
override ASFILES = $(call rwildcard, sysdeps/$(PLATFORM), *.S)
override OBJ = $(addprefix obj/,$(CFILES:.c=.c.o))
override CRT_OBJ = $(addprefix obj/,$(CRT_CFILES:.c=.c.o))
override SYSDEPS_OBJ = $(addprefix obj/,$(SYSDEPS_CFILES:.c=.c.o) $(ASFILES:.S=.S.o))

override HEADER_DEPS = $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

.PHONY: all
all: sysdeps crt
	@for proj in ./subprojects/* ; \
		do \
			file_name=$$(basename $${proj}) ; \
			echo COMPILING $${file_name} ; \
			make project OUTPUT=$${file_name} ; \
		done 

crt: $(CRT_OBJ)
sysdeps: $(SYSDEPS_OBJ)

# Default target.
.PHONY: project
project: bin/$(OUTPUT)

# Link rules for the final executable.
bin/$(OUTPUT): GNUmakefile $(OBJ)
	@echo [LD] Linking...
	@mkdir -p "$$(dirname $@)"
	@$(LD) $(OBJ) $(SYSDEPS_OBJ) $(CRT_OBJ) $(LDFLAGS) -o $@

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
