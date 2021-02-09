KERNEL_HDD = cernel.hdd

.PHONY: compile clean all toolchain cleanToolchain

all: $(KERNEL_HDD)

run: $(KERNEL_HDD)
	qemu-system-x86_64 -m 1G -hda $(KERNEL_HDD) -s

$(KERNEL_HDD): compile
	rm -f $(KERNEL_HDD)
	dd if=/dev/zero bs=1M count=0 seek=64 of=$(KERNEL_HDD)
	parted -s $(KERNEL_HDD) mklabel gpt
	parted -s $(KERNEL_HDD) mkpart primary 2048s 100%
	./echfs/echfs-utils -g -p0 $(KERNEL_HDD) quick-format 512
	./echfs/echfs-utils -g -p0 $(KERNEL_HDD) import kernel/cernel.elf cernel.elf
	./echfs/echfs-utils -g -p0 $(KERNEL_HDD) import limine.cfg limine.cfg
	./limine/limine-install $(KERNEL_HDD)

compile:
	make -C kernel	

clean:
	rm $(KERNEL_HDD)
	make -C kernel clean

toolchain:
	make -C limine toolchain
	make -C limine bootloader
	make -C limine
	make -C echfs
	chmod +x toolchain/make_toolchain.sh
	./toolchain/make_toolchain.sh

cleanToolchain:
	make -C limine clean
	make -C echfs clean
	rm -rf ./toolchain/cross
