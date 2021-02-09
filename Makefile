.PHONY: clean all toolchain

all:
	@echo "all"

clean:
	@echo "clean"

toolchain:
	#make -C limine toolchain
	#make -C limine bootloader
	#make -C limine
	#make -C echfs
	chmod +x toolchain/make_toolchain.sh
	./toolchain/make_toolchain.sh
