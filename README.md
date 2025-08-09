# Hexa OS


Hexa OS - is a from-scratch project of os.


To build:

    ./build.sh

    -i      Intalls and updates all dependencies
    -s      Start VM after building
    -c      Build GCC cross compiler

To test iso in vm:

    qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso
