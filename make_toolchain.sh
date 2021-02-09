cd limine
make toolchain
make bootloader
make
cd ..
cd echfs
make
cd ..
cd toolchain
chmod +x make_toolchain.sh
./make_toolchain.sh
cd ..
