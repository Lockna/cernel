#!/bin/sh
set -e
set -x

cd toolchain
mkdir -p cross
cd cross

PREFIX="$(pwd)"
TARGET=x86_64-elf

BINUTILSVERSION=2.35.1
GCCVERSION=10.2.0
NASMVERSION=2.15.05

export PATH="$PREFIX/bin:$PATH"

# downloading binutils/gcc/nasm
if [ ! -f binutils-$BINUTILSVERSION.tar.gz ]; then
    wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILSVERSION.tar.gz
fi
if [ ! -f gcc-$GCCVERSION.tar.gz ]; then
    wget https://ftp.gnu.org/gnu/gcc/gcc-$GCCVERSION/gcc-$GCCVERSION.tar.gz
fi
if [ ! -f nasm-$NASMVERSION.tar.gz ]; then
    wget https://www.nasm.us/pub/nasm/releasebuilds/$NASMVERSION/nasm-$NASMVERSION.tar.gz
fi

rm -rf build
mkdir build
cd build

# Extracting
tar -xf ../binutils-$BINUTILSVERSION.tar.gz
tar -xf ../gcc-$GCCVERSION.tar.gz
tar -xf ../nasm-$NASMVERSION.tar.gz

# Building binutils
mkdir build-binutils
cd build-binutils
../binutils-$BINUTILSVERSION/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j $(nproc)
make install
cd ..

# configuring gcc to build libgcc without red-zone
echo "# Add libgcc multilib variant without red-zone requirement4
MULTILIB_OPTIONS += mno-red-zone
MULTILIB_DIRNAMES += no-red-zone
" >> gcc-$GCCVERSION/gcc/config/i386/t-x86_64-elf

sed '/x86_64-\*-elf\*)/a tmake_file="${tmake_file} i386/t-x86_64-elf"' gcc-$GCCVERSION/gcc/config.gcc > gcc-$GCCVERSION/gcc/config2.gcc
cat gcc-$GCCVERSION/gcc/config2.gcc > gcc-$GCCVERSION/gcc/config.gcc
rm gcc-$GCCVERSION/gcc/config2.gcc

# building gcc
mkdir build-gcc
cd build-gcc
../gcc-$GCCVERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
make -j $(nproc) all-gcc
make -j $(nproc) all-target-libgcc
make install-gcc
make install-target-libgcc
cd ..

mkdir build-nasm
cd build-nasm
../nasm-$NASMVERSION/configure --prefix="$PREFIX"
make -j$(nproc)
make install -j$(nproc)
cd ..
