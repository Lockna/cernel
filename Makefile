all:
	echo "all"

clean:
	echo "clean"

toolchain:
	cd limine
	make toolchain
	make bootloader
	make
	cd ..
	cd echfs
	make
	cd ..
	cd toolchain
	./make_toolchain.sh
	cd ..
