#include "../../include/kernel/defs.h"
#include "../../include/drivers/keyboard.h"

int shiftedl = 0;
int shiftedr = 0;
int capsLock = 0;

char buffer[KEYBOARD_BUFFER_SIZE];
int buffer_write_cursor = 0;
int buffer_read_cursor = 0;

char scanCodeToAsciiTable[0xFF] =
{    /*  0    1    2    3    4    5    6    7    8    9    A    B    C    D    E     F    */
/*0*/   NOP, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', NOP, '\b', '\t',

/*1*/   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', NOP, '+', '\n', NOP, 'a', 's',

/*2*/   'd', 'f', 'g', 'h', 'j', 'k', 'l', NOP, '{', '}', NOP, '<', 'z', 'x', 'c', 'v',

/*3*/   'b', 'n', 'm', ',', '.', '-', NOP, '*', NOP, ' ', NOP, 0xB3, NOP, NOP, NOP, NOP,

/*4*/   NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, 0xC1, NOP, '-', 0xB4, NOP, 0xC3, '+', NOP,

/*5*/   0xC2, NOP, 0xDC, 0xB2, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP

};

char scanCodeToAsciiShiftedTable[0xFF] =
{
     /*  0    1    2    3    4    5    6    7    8    9    A    B    C    D    E     F    */
/*0*/   NOP, ESC, '!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '?', NOP, '\b', NOP,
    
/*1*/   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', NOP, '*', '\n', NOP, 'A', 'S',
    
/*2*/   'D', 'F', 'G', 'H', 'J', 'K', 'L', NOP, '[', ']', NOP, '>', 'Z', 'X', 'C', 'V',
    
/*3*/   'B', 'N', 'M', ';', ':', '_', NOP, NOP, NOP, ' ', NOP, 0xB3, NOP, NOP, NOP, NOP,
    
/*4*/   NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, 0xC1, NOP, '-', 0xB4, NOP, 0xC3, '+', NOP,
    
/*5*/   0xC2, NOP, 0xDC, 0xB2, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP
    
};

char scanCodeToAsciiCapsLockTable[0xFF] =
{
     /*  0    1    2    3    4    5    6    7    8    9    A    B    C    D    E     F    */
/*0*/   NOP, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', NOP, '\b', '\t',
    
/*1*/   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', NOP, '+', '\n', NOP, 'A', 'S',
    
/*2*/   'D', 'F', 'G', 'H', 'J', 'K', 'L', NOP, '{', '}', NOP, '<', 'Z', 'X', 'C', 'V',
    
/*3*/   'B', 'N', 'M', ',', '.', '-', NOP, '*', NOP, ' ', NOP, 0xB3, NOP, NOP, NOP, NOP,
    
/*4*/   NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, 0xC1, NOP, '-', 0xB4, NOP, 0xC3, '+', NOP,
    
/*5*/   0xC2, NOP, 0xDC, 0xB2, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP
    
};

char scanCodeToAsciiShiftedCapsLockTable[0xFF] =
{
     /*  0    1    2    3    4    5    6    7    8    9    A    B    C    D    E     F    */
/*0*/   NOP, ESC, '!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '?', NOP, '\b', NOP,
    
/*1*/   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', NOP, '*', '\n', NOP, 'a', 's',
    
/*2*/   'd', 'f', 'g', 'h', 'j', 'k', 'l', NOP, '[', ']', NOP, '>', 'z', 'x', 'c', 'v',
    
/*3*/   'b', 'n', 'm', ';', ':', '_', NOP, NOP, NOP, ' ', NOP, 0xB3, NOP, NOP, NOP, NOP,
    
/*4*/   NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, 0xC1, NOP, '-', 0xB4, NOP, 0xC3, '+', NOP,
    
/*5*/   0xC2, NOP, 0xDC, 0xB2, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP
    
};

char scanCodeToAscii(int scanCode) {
    specialKey(scanCode);
    if((shiftedr == 1 || shiftedl == 1) && capsLock == 1)
        return scanCodeToAsciiShiftedCapsLockTable[scanCode];
    else if(shiftedr == 1 || shiftedl == 1)
        return scanCodeToAsciiShiftedTable[scanCode];
    else if(capsLock == 1)
        return scanCodeToAsciiCapsLockTable[scanCode];
    else
        return scanCodeToAsciiTable[scanCode];
}

void specialKey(int scanCode) {
    switch(scanCode){
        case BREAK_SHIFT_L:
            shiftedl = 0;
            break;
        case BREAK_SHIFT_R:
            shiftedr = 0;
            break;
        case MAKE_SHIFT_L:
            shiftedl = 1;
        case MAKE_SHIFT_R:
            shiftedr = 1;
            break;
        case MAKE_CAPS_LOCK:
            capsLock = !capsLock;
            break;
    }
    return;
}

int isPrintable(unsigned char c) {
    if ((c > 1 && c < 14) || (c > 15 && c < 26) || (c > 29 && c < 42)
            || (c > 42 && c < 56) || c == 57 || c == 86) {
        return TRUE;
    }
    return FALSE;
}

unsigned char get_char_from_keyboard_buffer() {
    char ans = buffer[buffer_read_cursor];
    buffer[buffer_read_cursor] = 0;
    if (buffer_read_cursor == KEYBOARD_BUFFER_SIZE - 1) {
        buffer_read_cursor = 0;
    } else {
        buffer_read_cursor++;
    }
    return ans;
}

int keyboard_buffer_can_read() {
    return (buffer[buffer_read_cursor] != 0) ? 1 : 0;
}

void add_to_keyboard_buffer(unsigned char c) {
    buffer[buffer_write_cursor] = c;
    if (buffer_write_cursor == KEYBOARD_BUFFER_SIZE - 1) {
        buffer_write_cursor = 0;
    } else {
        buffer_write_cursor++;
    }
}