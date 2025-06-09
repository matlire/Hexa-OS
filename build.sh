#!/bin/bash

set -e

function build_cross_compiler() {
    echo "Building GCC cross-compiler..."

    export TARGET=x86_64-elf
    export PREFIX="$(pwd)/gcc-cross-compiler"
    export PATH="$PREFIX/bin:$PATH"

    mkdir -p build-gcc
    cd build-gcc

    if [ ! -d binutils ]; then
        curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.44.tar.gz
        tar -xvzf binutils-2.44.tar.gz
        mv binutils-2.44 binutils
    fi

    if [ ! -d gcc ]; then
        curl -O https://ftp.gnu.org/gnu/gcc/gcc-15.1.0/gcc-15.1.0.tar.gz
        tar -xvzf gcc-15.1.0.tar.gz
        mv gcc-15.1.0 gcc
        cd gcc
        ./contrib/download_prerequisites
        cd ..
    fi

    mkdir -p build-binutils build-gcc-stage

    cd build-binutils
    ../binutils/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror
    make
    make install
    cd ..

    cd build-gcc-stage
    ../gcc/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
    make all-gcc
    make all-target-libgcc
    make all-target-libstdc++-v3
    make install-gcc
    make install-target-libgcc
    make install-target-libstdc++-v3
    cd ../..
    
    echo "Cross-compiler built at $PREFIX"
}

function install_dependencies() {
    echo "Installing build dependencies..."

    # Detect package manager and install required packages
    if command -v apt-get >/dev/null; then
        sudo apt-get update
        sudo apt-get install -y build-essential nasm xorriso grub-pc-bin grub-common mtools bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev
    elif command -v pacman >/dev/null; then
        sudo pacman -Sy --noconfirm base-devel mtools grub-pc-bin grub-common xorriso nasm bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev
    elseq
        echo "Unsupported package manager. Please install dependencies manually."
        exit 1
    fi

    echo "Dependencies installed."
}

function build_project() {
    echo "Building project..."
    make clean
    make build-x86_64
    echo "Build completed."
}

install=false
start=false
cross_compiler=false

# Parse arguments
for arg in "$@"; do
    case "$arg" in
        -i)
            install=true
            ;;
        -s)
            start=true
            ;;
        -c)
            cross_compiler=true
            ;;
    esac
done


if [ "$install" = true ]; then
    install_dependencies
fi

if [ "$cross_compiler" = true ]; then
    build_cross_compiler
fi

build_project

if [ "$start" = true ]; then
    qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso
fi