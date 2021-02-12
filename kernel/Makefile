TARGET := cernel.elf

CC = ../toolchain/cross/bin/x86_64-elf-gcc
CFLAGS = -Wall -Wextra -g -O2 -pipe
ASM = ../toolchain/cross/bin/nasm
ASMFLAGS = -g -felf64

LDINTERNALFLAGS := \
	-Tlinker.ld    \
	-static        \
	-nostdlib      \
	-no-pie

INTERNALCFLAGS  :=       \
	-I include           \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic             \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mcmodel=kernel      \
	-mno-red-zone

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
