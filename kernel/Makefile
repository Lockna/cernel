TARGET := cernel.elf

CC = ../toolchain/cross/bin/x86_64-elf-gcc
CFLAGS = -Wall -Wextra -g -O2 -pipe

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

CFILES := $(shell find ./ -type f -name '*.c')
OBJ    := $(addprefix ./bin/,$(CFILES:.c=.o))
OBJDIR := bin
DIRS := $(sort $(addprefix ./bin/,$(patsubst %/,%,$(dir $(CFILES)))))

.PHONY: all dir clean

all: dir | $(TARGET)

dir:
	mkdir -p $(OBJDIR)
	mkdir -p $(DIRS)

$(TARGET): $(OBJ)
	$(CC) $(LDINTERNALFLAGS) $(OBJ) -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) bin
