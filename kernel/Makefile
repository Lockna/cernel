TARGET := cernel.elf

IS_MACOS := $(shell uname -a | grep -i 'Darwin')

ifdef IS_MACOS
	CC = x86_64-elf-gcc
	ASM = nasm
else
	CC = ../toolchain/cross/bin/x86_64-elf-gcc
  	ASM = ../toolchain/cross/bin/nasm
endif

CFLAGS = -Wall -Wextra -g -O2 -pipe
ASMFLAGS = -g -felf64

LDINTERNALFLAGS := \
	-Tlinker.ld    \
	-static        \
	-nostdlib      \
	-no-pie

INTERNALCFLAGS  :=        \
	-I include            \
	-ffreestanding        \
	-fstack-protector-all \
	-fno-pic              \
	-mno-80387            \
	-mno-mmx              \
	-mno-sse			  \
	-mno-sse2			  \
	-mno-3dnow            \
	-mcmodel=kernel       \
	-mno-red-zone		  \
	-fno-builtin          \
	-Wall

CFILES   := $(shell find ./ -type f -name '*.c')
OBJ    	 := $(addprefix ./bin/,$(CFILES:.c=.o))
ASMFILES := $(shell find ./ -type f -name '*.asm')
ASMOBJ 	 := $(addprefix ./bin/,$(ASMFILES:.asm=.o))
OBJDIR 	 := bin
DIRS 	 := $(sort $(addprefix ./bin/,$(patsubst %/,%,$(dir $(CFILES)))))
ASMDIRS  := $(sort $(addprefix ./bin/,$(patsubst %/,%,$(dir $(ASMFILES)))))

.PHONY: all dir clean

all: dir | $(TARGET)

dir:
	mkdir -p $(OBJDIR)
	mkdir -p $(DIRS)
	mkdir -p $(ASMDIRS)

$(TARGET): $(OBJ) $(ASMOBJ)
	$(CC) $(LDINTERNALFLAGS) $(OBJ) $(ASMOBJ) -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf $(TARGET) bin
