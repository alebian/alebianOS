#include "../../include/kernel/defs.h"
#include "../../include/kernel/k_libasm.h"
#include "../../include/drivers/keyboard.h"

static int shifted = 0;
static int capsLock = 0;

/* 0 1 2 3 4 5 6 7 8 9 A B C D E F
 * 0
 * 1
 * 2
 * 3
 * 4
 * 5
 */

static char scanCodeToAsciiTable[] = {
NOP, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE, TAB,
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER, NOP, 'a', 's',
'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', NOP, '\\', 'z', 'x', 'c', 'v',
'b', 'n', 'm', ',', '.', '/', NOP, '*', NOP, ' ', NOP, KF1, KF2, KF3, KF4, KF5,
KF6, KF7, KF8, KF9, KF10, NOP, NOP, KHOME, KUP, KPGUP, '-', KLEFT, NOP, KRIGHT, '+', KEND,
KDOWN, KPGDN, KINS, KDEL, NOP, NOP, NOP, KF11, KF12 };

static char scanCodeToAsciiShiftedTable[] = {
NOP, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BACKSPACE, NOP,
'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', ENTER, NOP, 'A', 'S',
'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', NOP, '|', 'Z', 'X', 'C', 'V',
'B', 'N', 'M', '<', '>', '?', NOP, NOP, NOP, ' ', NOP, KF1, KF2, KF3, KF4, KF5,
KF6, KF7, KF8, KF9, KF10, NOP, NOP, KHOME, KUP, KPGUP, '-', KLEFT, NOP, KRIGHT, '+', KEND,
KDOWN, KPGDN, KINS, KDEL, NOP, NOP, NOP, KF11, KF12 };

static char scanCodeToAsciiCapsLockTable[] = {
NOP, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE, TAB,
'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', ENTER, NOP, 'A', 'S',
'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', NOP, '\\', 'Z', 'X', 'C', 'V',
'B', 'N', 'M', ',', '.', '/', NOP, '*', NOP, ' ', NOP, KF1, KF2, KF3, KF4, KF5,
KF6, KF7, KF8, KF9, KF10, NOP, NOP, KHOME, KUP, KPGUP, '-', KLEFT, NOP, KRIGHT, '+', KEND,
KDOWN, KPGDN, KINS, KDEL, NOP, NOP, NOP, KF11, KF12 };

static char scanCodeToAsciiShiftedCapsLockTable[] = {
NOP, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BACKSPACE, NOP,
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', ENTER, NOP, 'a', 's',
'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', NOP, '|', 'z', 'x', 'c', 'v',
'b', 'n', 'm', '<', '>', '?', NOP, NOP, NOP, ' ', NOP, KF1, KF2, KF3, KF4, KF5,
KF6, KF7, KF8, KF9, KF10, NOP, NOP, KHOME, KUP, KPGUP, '-', KLEFT, NOP, KRIGHT, '+', KEND,
KDOWN, KPGDN, KINS, KDEL, NOP, NOP, NOP, KF11, KF12 };

char scanCodeToAscii(int scanCode){
    /* Check if a key is pressed or released */
    if (scanCode & 0x80){
        /* If released */
        switch(scanCode){
            case BREAK_SHIFT_L:
                shifted = 0;
                return 0;
            case BREAK_SHIFT_R:
                shifted = 0;
                return 0;
        }
    } else {
        /* If pressed */
        /* If shift keys pressed */
        switch(scanCode){
            case MAKE_SHIFT_L:
                shifted = 1;
                return 0;
            case MAKE_SHIFT_R:
                shifted = 1;
                return 0;
            case MAKE_CAPS_LOCK:
                capsLock = !capsLock;
                if(capsLock){
                    turn_on_capslock();
                }else{
                    turn_off_leds();
                }
                return 0;
        }
        /* If arrow keys pressed */
        switch(scanCode){
            case 0x4B: //scancode for left arrow key
                return KLEFT;
            case 0x4D: //scancode for right arrow key
                return KRIGHT;
            case 0x48: //scancode for up arrow key
                return KUP;
            case 0x50: //scancode for down arrow key
                return KDOWN;
        }
        /* Any other key pressed */
        if (capsLock){
            if(shifted){
                return scanCodeToAsciiShiftedCapsLockTable[scanCode];
            } else {
                return scanCodeToAsciiCapsLockTable[scanCode];
            }
        } else {
            if(shifted){
                return scanCodeToAsciiShiftedTable[scanCode];
            } else {
                return scanCodeToAsciiTable[scanCode];
            }
        }
    }
    return 0;
}

int isPrintable(unsigned char c){
    if ((c > 1 && c < 14) || (c > 15 && c < 26) || (c > 29 && c < 42)
            || (c > 42 && c < 56) || c == 57 || c == 86){
        return TRUE;
    }
    return FALSE;
}

/* Functions to control keyboard LEDs */

void turn_on_leds(){
    _outb(0x60, SET_LED);
    _outb(0x60, 0x07);
    return;
}

void turn_off_leds(){
    _outb(0x60, SET_LED);
    _outb(0x60, 0x00);
    return;
}

void turn_on_capslock(){
    _outb(0x60, SET_LED);
    _outb(0x60, 0x04);
    return;
}