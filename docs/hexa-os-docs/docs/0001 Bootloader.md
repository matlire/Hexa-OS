
---

## Bootloader

Bootloader is an important component of system and first thing that starts working after OS getting control.

BIOS ⟶ GRUB ⟶ Bootloader

So as we are developing system from scratch we need custom GRUB config and own bootloader.

---

### Linker

But first of all we need `targets/x86_64/linker.ld` to link all needed files and tell grub where he can find essential sections.

`ENTRY(_start)` ⟶ tells that entrypoint of bootloader is _start (we'll see it later)

`SECTIONS { ... }` ⟶ defines how the sections from input object files (like `.text`, `.data`, etc.) are mapped to addresses in the output binary.

`.  = 1M;` ⟶ sets location counter (i.e., the current address in memory layout) to 1 megabyte (usually 1M is default for kernels; now everything after this line will be placed starting at address `0x00100000`)

`.boot : { ... } .text : { ... }` ⟶ defines sections

`KEEP(*(.multiboot_header))` ⟶ keep from all input files from `.multiboot_header`. `KEEP` is used to tell linker not to discard this section even if it seems unused.

`*(.text)` ⟶ from all input files from `.text` (program code)

---

### GRUB cfg

First let's see `targets/x86_64/iso/boot/grub/grub.cfg`:

`set timeout=0` ⟶ delay to give control to bootloader

`set default=0` ⟶ Sets the default menuentry index to boot

`menuentry "Hexa OS" { ... }` ⟶ Defines a boot menu entry named `"Hexa OS"`

`multiboot2 /boot/kernel.bin` ⟶ `kernel.bin` is Multiboot2-compliant, which grub loads into memory

`boot` ⟶ Give control to bootloader

---

After grub loads `kernel.bin` into memory, it 1) checks `.multiboot_header` section 2) If no problems, jumps to `_start` function. As `.multiboot_header` is well commented, I'll jump to `_start`.

---

### Boot.asm

`bits 32` ⟶ assemble in 32-bit protected-mode instructions

`mov esp, stack_top` ⟶ initialize the stack pointer to the top of a 16 KB (`resb  4096*4`) stack reserved in `.bss`

`check_multiboot` ⟶ verify GRUB passed the correct Multiboot2 magic number in EAX

`check_cpuid` ⟶ test CPU for CPUID support by toggling ID flag, see  more [here](https://www.felixcloutier.com/x86/cpuid).

`check_long_mode` ⟶ test CPU for long mode support 

`setup_page_tables` ⟶ Setup paging

`enable_paging` ⟶ Enable paging

`lgdt [gdt64.pointer]` ⟶ loads new GDT descriptor (pointer to a 64-bit GDT) from our `.rodata` block

`jmp gdt64.code_segment:_long_mode_start` ⟶ make far jump to _long_mode_start



### Boot64.asm

`bits 64` ⟶ assemble in 64-bit protected-mode instructions

`_long_mode_start` ⟶ clears registers and give control to kernel via `call kernel_main`. See Kernel, `main.c`.	