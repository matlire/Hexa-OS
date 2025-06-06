# Hexa OS


Hexa OS - is a from-scretch project, which main goal is to create an os capable of running both - linux and windows executables.


To build:

    ./build.sh

    -i Intalls and updates all dependencies

To test iso in vm:

    qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso

Documentation:

    ./docs.sh


Documentation can be found on 127.0.0.1:3000