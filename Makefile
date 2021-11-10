export

IS_WSL := $(shell uname -a | grep -i 'Microsoft')
IS_MACOS := $(shell uname -a | grep -i 'Darwin')

ifdef IS_MACOS
	DD_FLAGS = bs=1m count=0 seek=64
	XORRISO_FLAGS = -as mkisofs -b /boot/limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot /boot/limine-eltorito-efi.bin -efi-boot-part --efi-boot-image --protective-msdos-label pack 
else
	DD_FLAGS = bs=1M count=0 seek=64
endif

ifeq ($(OS),Windows_NT)
    QEMU = qemu-system-x86_64.exe
else
    QEMU = qemu-system-x86_64
endif

ifdef IS_WSL
	QEMU = qemu-system-x86_64.exe
endif

ifdef RELEASE
	MODE = release
else
	MODE = debug
endif

KERNEL_HDD = cernel.hdd
NM = ./toolchain/cross/bin/x86_64-elf-nm

.PHONY: drun run compile clean all toolchain cleanToolchain

all: $(KERNEL_HDD)

run:
	$(QEMU) -m 1G -debugcon stdio -drive file=$(KERNEL_HDD),format=raw -d int -no-reboot -no-shutdown

drun:
	$(QEMU) -m 1G -debugcon stdio -drive file=$(KERNEL_HDD),format=raw -s -S -d int -no-shutdown

$(KERNEL_HDD): compile
	rm -f $(KERNEL_HDD)
	dd if=/dev/zero $(DD_FLAGS) of=$(KERNEL_HDD)

ifdef IS_MACOS
	mkdir pack
	mkdir pack/boot
	cp kernel/cernel.elf pack/
	cp kernel/symbols.map pack/
	cp limine.cfg pack/boot/
	cp ./limine/limine.sys pack/boot/
	cp ./limine/limine-cd.bin pack/boot/
	cp ./limine/limine-eltorito-efi.bin pack/boot/
	xorriso $(XORRISO_FLAGS) -o $(KERNEL_HDD)
	rm -rf pack
else
	parted -s $(KERNEL_HDD) mklabel gpt
	parted -s $(KERNEL_HDD) mkpart primary 2048s 100%
	./echfs/echfs-utils -g -p0 $(KERNEL_HDD) quick-format 512
	./echfs/echfs-utils -g -p0 $(KERNEL_HDD) import kernel/cernel.elf cernel.elf
	./echfs/echfs-utils -g -p0 $(KERNEL_HDD) import kernel/symbols.map symbols.map
	./echfs/echfs-utils -g -p0 $(KERNEL_HDD) import limine.cfg limine.cfg
	./echfs/echfs-utils -g -p0 $(KERNEL_HDD) import limine/limine.sys limine.sys
endif
	./limine/limine-install $(KERNEL_HDD)

compile:
	make -C kernel
	$(NM) kernel/cernel.elf | grep " T " | cut -d' ' -f 1,3 > kernel/symbols.map

clean:
	rm -f $(KERNEL_HDD)
	make -C kernel clean

toolchain:
	make -C limine
ifndef IS_MACOS
	make -C echfs echfs-utils
	chmod +x toolchain/make_toolchain.sh
	./toolchain/make_toolchain.sh
endif

cleanToolchain:
	make -C limine clean
	make -C echfs clean
	rm -rf ./toolchain/cross
