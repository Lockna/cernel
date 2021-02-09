.PHONY: clean all toolchain cleanToolchain

all:
	@echo "all"

clean:
	@echo "clean"

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
