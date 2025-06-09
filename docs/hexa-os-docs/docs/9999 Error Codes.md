
---

## Error codes

All error codes documentation.

---

### Bootloader

Error code are 1 symbol long, representing first letter of error.

`M` - multiboot not supported

`C` - cpu id not supported

`L` - long mode not supported

### Kernel

Error code format: `XXXX.XXXX.0000.0000.C`, where first 4 symbols are letters, representing path to source file, where error occurred, second 4 symbols are letters, representing some file info:
- X - NOP, nothing

- H - header

- C - c file

- A - asm file

- D - definition/declaration

- V - value assignation

- M - compiler error

next 4 symbols are digits, representing global error code, next 4 symbols are digits, representing local error code, last symbol is letter, representing error level:

- I - info

- W - warning

- E - not critical error

- C - critical

---

`XXUP.XHDM.0000.0001.C` - utils/print, header file, definition section, compiler problem, critical - not using cross-compiler
