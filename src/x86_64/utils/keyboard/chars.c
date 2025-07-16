#include "chars.h"

const char *not_printable[] = {
    "ERROR",
    "Esc",
    "Backspace",
    "Tab",
    "LCtrl",
    "RCtrl",
    "LShift",
    "RShift",
    "LAlt",
    "RAlt",
    "CapsLock",
    "NumLock",
    "ScrollLock",
    "Kp*",
    "Kp/",
    "Kp-",
    "Kp+",
    "Kp.",
    "F1", "F2", "F3", "F4",
    "F5", "F6", "F7", "F8",
    "F9", "F10", "F11", "F12",
    "Home", "End", "Delete"
};

const char ctrl_break[] = {
    '\n',
    ' ',
    '-',
    '_',
    '(',
    ')',
    '*',
    '&',
    '?',
    '/',
    '\\',
    '|',
    '^',
    '%',
    '$',
    '@',
    '!',
    '~',
    '\'',
    '"',
    '<',
    '>',
    '.',
    ','
};

const char *ascii_names[] = {
    "ERROR",       // 0x00
    "Esc",         // 0x01
    "1",           // 0x02
    "2",           // 0x03
    "3",           // 0x04
    "4",           // 0x05
    "5",           // 0x06
    "6",           // 0x07
    "7",           // 0x08
    "8",           // 0x09
    "9",           // 0x0A
    "0",           // 0x0B
    "-",           // 0x0C
    "=",           // 0x0D
    "Backspace",   // 0x0E
    "Tab",         // 0x0F
    "Q",           // 0x10
    "W",           // 0x11
    "E",           // 0x12
    "R",           // 0x13
    "T",           // 0x14
    "Y",           // 0x15
    "U",           // 0x16
    "I",           // 0x17
    "O",           // 0x18
    "P",           // 0x19
    "[",           // 0x1A
    "]",           // 0x1B
    "Enter",       // 0x1C
    "LCtrl",       // 0x1D
    "A",           // 0x1E
    "S",           // 0x1F
    "D",           // 0x20
    "F",           // 0x21
    "G",           // 0x22
    "H",           // 0x23
    "J",           // 0x24
    "K",           // 0x25
    "L",           // 0x26
    ";",           // 0x27
    "'",           // 0x28
    "`",           // 0x29
    "LShift",      // 0x2A
    "\\",          // 0x2B
    "Z",           // 0x2C
    "X",           // 0x2D
    "C",           // 0x2E
    "V",           // 0x2F
    "B",           // 0x30
    "N",           // 0x31
    "M",           // 0x32
    ",",           // 0x33
    ".",           // 0x34
    "/",           // 0x35
    "RShift",      // 0x36
    "Kp*",         // 0x37
    "LAlt",        // 0x38
    "Space",       // 0x39
    "CapsLock",    // 0x3A
    "F1",          // 0x3B
    "F2",          // 0x3C
    "F3",          // 0x3D
    "F4",          // 0x3E
    "F5",          // 0x3F
    "F6",          // 0x40
    "F7",          // 0x41
    "F8",          // 0x42
    "F9",          // 0x43
    "F10",         // 0x44
    "NumLock",     // 0x45
    "ScrollLock",  // 0x46
    "Kp7",         // 0x47
    "Kp8",         // 0x48
    "Kp9",         // 0x49
    "Kp-",         // 0x4A
    "Kp4",         // 0x4B
    "Kp5",         // 0x4C
    "Kp6",         // 0x4D
    "Kp+",         // 0x4E
    "Kp1",         // 0x4F
    "Kp2",         // 0x50
    "Kp3",         // 0x51
    "Kp0",         // 0x52
    "Kp.",         // 0x53
    "ERROR",       // 0x54
    "ERROR",       // 0x55
    "ERROR",       // 0x56
    "F11",         // 0x57
    "F12"          // 0x58
};

const char ascii_normal[] = {
    '?',  // 0x00
    '?',  // 0x01 Esc
    '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  // 0x02–0x0B
    '-', '=', '?',  // 0x0C–0x0E Backspace
    '\t', // 0x0F
    'q','w','e','r','t','y','u','i','o','p',  // 0x10–0x19
    '[',']','\n','?',  // 0x1A–0x1D LCtrl -> '?'
    'a','s','d','f','g','h','j','k','l',';',  // 0x1E–0x27
    '\'','`','?',  // 0x28–0x29 LShift -> '?'
    '\\','z','x','c','v','b','n','m',',','.','/',  // 0x2B–0x35
    '?','*','?',  // 0x36 RShift, Kp*, LAlt
    ' ',  // 0x39 Space
    '?',  // 0x3A CapsLock
    // F‑keys and locks
    '?','?','?','?','?','?','?','?','?','?',  // 0x3B–0x44
    '?','?',  // 0x45 NumLock, ScrollLock
    // keypad digits/arrows
    '?','?','?','?','?','?','?','?','?','?','?',  // 0x47–0x51
    '?',  // 0x52 Kp0
    '?',  // 0x53 Kp.
    '?','?','?',  // 0x54–0x56
    '?','?'   // 0x57–0x58 F11, F12
};

const char ascii_shifted[] = {
    '?', // 0x00
    '?',  // Esc
    '!','@','#','$','%','^','&','*','(',')',  // '1'..'0'
    '_','+','?',   // '-' '=' Backspace
    '\t',          // Tab
    'Q','W','E','R','T','Y','U','I','O','P',
    '{','}','\n','?',  // [,],Enter
    'A','S','D','F','G','H','J','K','L',':',
    '"','~','?',  // '`' LShift
    '|','Z','X','C','V','B','N','M','<','>','?',
    '?','*','?', // RShift,Kp*,LAlt
    ' ', // Space
    '?', // CapsLock
    // F‑keys and locks
    '?','?','?','?','?','?','?','?','?','?',
    '?','?',
    // keypad
    '?','?','?','?','?','?','?','?','?','?','?',
    '?',
    '?','?','?',
    '?','?'   // F11, F12
};

const char ascii_caps[] = {
    '?',
    '?',
    '1','2','3','4','5','6','7','8','9','0',
    '-','=', '?',
    '\t',
    'Q','W','E','R','T','Y','U','I','O','P',
    '[',']','\n','?',
    'A','S','D','F','G','H','J','K','L',';',
    '\'','`','?','\\','Z','X','C','V','B','N','M',',','.','/',
    '?','*','?',
    ' ', '?',
    '?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?'
};

const char ascii_shifted_caps[] = {
    '?',
    '?',
    '!','@','#','$','%','^','&','*','(',')',
    '_','+','?',
    '\t',
    'q','w','e','r','t','y','u','i','o','p',
    '{','}','\n','?',
    '?','a','s','d','f','g','h','j','k','l',':',
    '"','~','?','|','z','x','c','v','b','n','m','<','>','?',
    '?','*','?',
    ' ', '?',
    '?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?'
};

const char *ext_names[] = {
    "ERROR",        // 0xE0,00
    "Insert",       // 0xE0,52
    "Delete",       // 0xE0,53
    "Home",         // 0xE0,47
    "End",          // 0xE0,4F
    "PageUp",       // 0xE0,49
    "PageDown",     // 0xE0,51
    "ArrowUp",      // 0xE0,48
    "ArrowDown",    // 0xE0,50
    "ArrowLeft",    // 0xE0,4B
    "ArrowRight",   // 0xE0,4D
    "Keypad /",     // 0xE0,35
    "Keypad Enter", // 0xE0,1C
    "RCtrl",        // 0xE0,1D
    "RAlt",         // 0xE0,38
    "WinLeft",      // 0xE0,5B
    "WinRight",     // 0xE0,5C
    "Menu"          // 0xE0,5D
};

const char ext_normal[] = {
    '?','?','?','?','?','?','?','?','?','?','?', 
    '/',    // Keypad /
    '\n',   // Keypad Enter
    '?','?','?','?','?','?'  // the rest (Win/Menu etc)
};

const char ext_numed[] = {
    '?',   // 0xE0,00
    '0',   // Insert(0xE0,52)
    '.',   // Delete(0xE0,53)
    '7',   // Home
    '1',   // End
    '9',   // PageUp
    '3',   // PageDown
    '8',   // ArrowUp
    '2',   // ArrowDown
    '4',   // ArrowLeft
    '6',   // ArrowRight
    '/',   // Keypad /
    '\n',  // Keypad Enter
    '?','?','?','?','5'  // the rest; 0xE0,5D(Menu) → '5'
};