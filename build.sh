#!/bin/bash

set -e

function install_dependencies() {
    echo "Installing build dependencies..."

    # Detect package manager and install required packages
    if command -v apt-get >/dev/null; then
        sudo apt-get update
        sudo apt-get install -y build-essential nasm xorriso grub-pc-bin grub-common mtools
    elif command -v pacman >/dev/null; then
        sudo pacman -Sy --noconfirm base-devel mtools grub-pc-bin grub-common xorriso nasm
    elif command -v brew >/dev/null; then
        brew install grub-pc-bin grub-common xorriso nasm mtools
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

if [[ "$1" == "-i" ]]; then
    install_dependencies
fi

build_project