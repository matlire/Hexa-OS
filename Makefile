x86_64_c_source_files    := $(shell find src/x86_64/ -type f -name '*.c')
x86_64_c_object_files    := $(patsubst src/x86_64/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

x86_64_asm_source_files  := $(shell find src/x86_64/ -type f -name '*.asm')
x86_64_asm_object_files  := $(patsubst src/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

all_object_files         := \
    $(x86_64_c_object_files) \
    $(x86_64_asm_object_files)

GCC_FLAGS   := -ffreestanding -fno-stack-protector -fno-builtin -O2 -mno-red-zone -mno-sse -mno-mmx -m64 -Wall -Wextra \
               -I src/x86_64        \
               -I src/x86_64/utils  \
			   -I src/x86_64/utils/print  \
               -I src/x86_64/kernel \
               -I src/x86_64/interrupts \
               -I src/x86_64/drivers/ports \
               -I src/x86_64/drivers/ps2 \
               -I src/x86_64/PIC

NASM_FLAGS  := -f elf64

LD_SCRIPT   := targets/x86_64/linker.ld
LD_FLAGS    := -n -T $(LD_SCRIPT)

CC=./gcc-cross-compiler/bin/x86_64-elf-gcc
LD=./gcc-cross-compiler/bin/x86_64-elf-ld

.PHONY: files
files:
	@echo "x86_64 C sources:    $(x86_64_c_source_files)"
	@echo "x86_64 ASM sources:  $(x86_64_asm_source_files)"

build/kernel/%.o: src/kernel/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCC_FLAGS) -c $< -o $@

build/x86_64/%.o: src/x86_64/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCC_FLAGS) -c $< -o $@

build/x86_64/%.o: src/x86_64/%.asm
	@mkdir -p $(dir $@)
	nasm $(NASM_FLAGS) $< -o $@

.PHONY: build-x86_64
build-x86_64: $(all_object_files)
	@echo "[LD] Linking all objects → dist/x86_64/kernel.bin"
	@mkdir -p dist/x86_64
	ld $(LD_FLAGS) -o dist/x86_64/kernel.bin $(all_object_files)

	@echo "[COPY] Putting kernel.bin into GRUB ISO tree"
	@cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin

	@echo "[GRUB] Generating bootable ISO"
	grub-mkrescue /usr/lib/grub/i386-pc \
	              -o dist/x86_64/kernel.iso \
	              targets/x86_64/iso

.PHONY: clean
clean:
	rm -rf build    \
	       dist	\
	       targets/x86_64/iso/boot/kernel.bin

.DEFAULT_GOAL := build-x86_64
