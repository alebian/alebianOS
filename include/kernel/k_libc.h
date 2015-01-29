#ifndef _kc_
#define _kc_

#include "defs.h"
#include "../lib/time.h"

#define FIRSTBYTE(x) x%16
#define SECONDBYTE(x) x-(FIRSTBYTE(x))

typedef void(*klistener)(void);
typedef void(*clicklistener)(int, int);

dword __write(int, unsigned char*, dword);
dword __read(int, unsigned char*, dword);
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
int k_isReadyToShell(void);
void k_shellReady(void);
void k_shellNotReady(void);
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
void k_null(void);
void k_null2(int, int);
void k_randomVGAstyle(void);
void k_setVGAstyle(char, char, char, char, char);
void k_move_cursor_back(void);
void k_move_cursor_forward(void);
void k_set_vga_size(int, int);
void k_beep(void);

#endif
