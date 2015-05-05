#ifndef _kernel_
#define _kernel_

#include "../lib/time.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define FIRSTBYTE(x) x%16
#define SECONDBYTE(x) x-(FIRSTBYTE(x))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define OS_VERSION "0.1"
#define OS_NAME "alebianOS"

#define TRUE    1
#define FALSE   0
#define EOF     -1
#define NULL    0

typedef unsigned char  byte;
typedef short int 	   word;
typedef int 		   dword;
typedef long 		   qword;
typedef char 		   s8int;
typedef unsigned char  u8int;
typedef short		   s16int;
typedef unsigned short u16int;
typedef int 		   s32int;
typedef unsigned int   u32int;
typedef long 		   s64int;
typedef unsigned long  u64int;

/* Segment access rights flags */
#define ACS_PRESENT     0x80            /* segment present */
#define ACS_CSEG        0x18            /* code segment */
#define ACS_DSEG        0x10            /* data segment */
#define ACS_READ        0x02            /* read segment */
#define ACS_WRITE       0x02            /* write segment */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386     0x0E    		/* Interrupt GATE 32 bits */
#define ACS_EXC_386     0x0F            /* Exception GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )
#define ACS_EXC         ( ACS_PRESENT | ACS_EXC_386 )
#define ACS_USED        0x10            /* Interrupt programed */
#define ACS_CODE        ( ACS_PRESENT | ACS_CSEG | ACS_READ )
#define ACS_DATA        ( ACS_PRESENT | ACS_DSEG | ACS_WRITE )
#define ACS_STACK       ( ACS_PRESENT | ACS_DSEG | ACS_WRITE )

#define KEYBOARD_BUFFER_SIZE  32

#pragma pack (1)    /* Allign structures to 1 byte */

/* Segment descriptor */
typedef struct {
	word limit;
	word base_l;
	byte base_m;
	byte access;
	byte attribs;
	byte base_h;
} __attribute__ ((packed)) DESCR_SEG;

typedef void(*klistener)(void);
typedef void(*clicklistener)(int, int);

void k_showLoadingScreen(void);
void k_checkBIOSinfo(void);
void k_checkSystemSpeed(void);
void k_time(tm*);
char* k_stime(void);
void k_setTimeStyle(int);
int k_getTimeStyle(void);
void k_exitScreen(void);
void k_clearScreen(void);
void k_clearFullScreen(void);
void k_scrolldown(void);
void k_scrollup(void);
char k_getBackgroundColor(void);
void k_setBackgroundColor(char);
void k_setFullBackgroundColor(char);
char k_getCharacterColor(void);
void k_setCharacterColor(char);
void k_setAllCharacterColor(char);
char k_getMouseColor(void);
void k_setMouseColor(char, char);
void k_printStartBar(char*);
void k_setStartBarColor(char);
void k_sleep(int);
void k_reboot(void);
void k_set_frecuency(int);
int k_isShellEnabled(void);
void k_enableShell(void);
void k_disableShell(void);
void k_panic(char*);
void k_KeyboardListener(void);
void k_setKeyboardListener(void(*listener)(void));
void k_printWithColor(char*, char);
void k_printwarning(char*);
void k_printerror(char*);
void k_printalert(char*);
void k_printosmsg(char*);
void k_drawMouse(void);
void k_eraseMouse(void);
void k_mouseRightClick(int, int);
void k_mouseLeftClick(int, int);
void k_mouseMidClick(int, int);
void k_setLclickListener(clicklistener);
void k_setRclickListener(clicklistener);
void k_setMclickListener(clicklistener);
void k_OSkblistener(void);
void k_OSlclcklistener(int, int);
void k_OSrclcklistener(int, int);
void k_OSmclcklistener(int, int);
void k_setStartMenu(void);
void k_printStartMenu(void);
void k_clearStartMenu(void);
void k_randomVGAstyle(void);
void k_setVGAstyle(char, char, char, char, char);
void k_move_cursor_back(void);
void k_move_cursor_forward(void);
void k_set_vga_size(int, int);
void k_beep(void);
void k_init_shell_file(void);
void k_deleteKey(void);

#endif
