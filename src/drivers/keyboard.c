#include "../../include/kernel/events.h"
#include "../../include/arch/x86/x86.h"
#include "../../include/drivers/keyboard.h"

static int shifted = 0;
static int capsLock = 0;
static keyboard_buffer k_buffer;

int init_keyboard(){
    int j;
    reinit_keyboard();
    for (j = 0; j < KEYBOARD_BUFFER_SIZE; j++){
        k_buffer.vec[j] = 0;
    }
    return 0;
}

void reinit_keyboard(){
    k_buffer.read=0;
    k_buffer.write=0;
    return;
}

void add_to_keyboard_buffer(unsigned char c){
    k_buffer.vec[k_buffer.write++]=c;
    if(k_buffer.write>=KEYBOARD_BUFFER_SIZE){
        k_buffer.write=0;
    }
    return;
}

unsigned char get_char_from_keyboard_buffer(){
    unsigned char c;
    if(k_buffer.read==k_buffer.write){
        //reinit_keyboard();
        return 0;
    }
    c = k_buffer.vec[k_buffer.read++];
    if(k_buffer.read>=KEYBOARD_BUFFER_SIZE){
        k_buffer.read=0;
    }
    return c;
}

/* 0 1 2 3 4 5 6 7 8 9 A B C D E F
 * 0
 * 1
 * 2
 * 3
 * 4
 * 5
 */

static char scanCodeToAsciiTable[] = {
0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE, TAB,
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER, 0, 'a', 's',
'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, KF1, KF2, KF3, KF4, KF5,
KF6, KF7, KF8, KF9, KF10, 0, 0, KHOME, KUP, KPGUP, '-', KLEFT, 0, KRIGHT, '+', KEND,
KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };

static char scanCodeToAsciiShiftedTable[] = {
0, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BACKSPACE, 0,
'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', ENTER, 0, 'A', 'S',
'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0, KF1, KF2, KF3, KF4, KF5,
KF6, KF7, KF8, KF9, KF10, 0, 0, KHOME, KUP, KPGUP, '-', KLEFT, 0, KRIGHT, '+', KEND,
KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };

static char scanCodeToAsciiCapsLockTable[] = {
0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE, TAB,
'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', ENTER, 0, 'A', 'S',
'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\', 'Z', 'X', 'C', 'V',
'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0, KF1, KF2, KF3, KF4, KF5,
KF6, KF7, KF8, KF9, KF10, 0, 0, KHOME, KUP, KPGUP, '-', KLEFT, 0, KRIGHT, '+', KEND,
KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };

static char scanCodeToAsciiShiftedCapsLockTable[] = {
0, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BACKSPACE, 0,
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', ENTER, 0, 'a', 's',
'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', 0, '|', 'z', 'x', 'c', 'v',
'b', 'n', 'm', '<', '>', '?', 0, 0, 0, ' ', 0, KF1, KF2, KF3, KF4, KF5,
KF6, KF7, KF8, KF9, KF10, 0, 0, KHOME, KUP, KPGUP, '-', KLEFT, 0, KRIGHT, '+', KEND,
KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };

/*
 * Transforms the ScanCode readed from the keyboard into ASCII
 */
 unsigned char scanCodeToAscii(int scanCode){
    /* Check if a key is pressed or released */
    if (scanCode & 0x80){
        /* If released */
        switch(scanCode){
            case BREAK_SHIFT_L:
                shifted = 0;
                break;
            case BREAK_SHIFT_R:
                shifted = 0;
                break;
        }
    } else {
        /* If pressed */
        switch(scanCode){
            /* If shift keys pressed */
            case MAKE_SHIFT_L: //scancode for left arrow key
                shifted = 1;
                break;
            case MAKE_SHIFT_R:
                shifted = 1;
                break;
            case MAKE_CAPS_LOCK:
                capsLock = !capsLock;
                if(capsLock){
                    turn_on_capsLock();
                }else{
                    turn_off_leds();
                }
                break;
            case KLEFT: //scancode for left arrow key
                return LEFT;
            case KRIGHT: //scancode for right arrow key
                return RIGHT;
            case KUP: //scancode for up arrow key
                return UP;
            case KDOWN: //scancode for down arrow key
                return DOWN;
            case KPGUP: //scancode for page up key
                return PGUP;
            case KPGDN: //scancode for page down key
                return PGDN;
            case KF1:
                return F1;
            case KF2:
                return F2;
            case KF3:
                return F3;
            case KF4:
                return F4;
            case KF5:
                return F5;
            case KF6:
                return F6;
            case KF7:
                return F7;
            case KF8:
                return F8;
            case KF9:
                return F9;
            case KF10:
                return F10;
            case KF11:
                return F11;
            case KF12:
                return F12;
            default:
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

/* ####################################################################
 * #                Functions to control keyboard LEDs                #
 * ####################################################################
 */
 
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

void turn_on_capsLock(){
    _outb(0x60, SET_LED);
    _outb(0x60, 0x04);
    return;
}

void turn_on_numLock(){
    _outb(0x60, SET_LED);
    _outb(0x60, 0x02);
    return;
}

void turn_on_scrollLock(){
    _outb(0x60, SET_LED);
    _outb(0x60, 0x01);
    return;
}
