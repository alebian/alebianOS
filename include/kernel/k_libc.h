#ifndef _kc_
#define _kc_

#include "defs.h"
#include "../lib/time.h"

#define FIRSTBYTE(x) x%16
#define SECONDBYTE(x) x-(FIRSTBYTE(x))

typedef void(*klistener)(void);

dword __write(int, char *, dword);
dword __read(int, char *, dword);
void k_showLoadingScreen(void);
void k_checkBIOSinfo(void);
void k_checkSystemSpeed(void);
void k_time(tm *);
char * k_stime(void);
char * k_currentTime(void);
void k_setTimeStyle(int);
void k_turnOff(void);
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
void k_setMouseColor(char);
void k_drawMouse(int, int);
void k_eraseMouse(int, int);
void k_updateStartBar(void);
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
void k_printwarning(char*);
void k_printerror(char*);
void k_printalert(char*);
void k_printosmsg(char*);
void k_mouseRightClick(int, int);
void k_mouseLeftClick(int, int);
void k_mouseMidClick(int, int);
void openStartMenu(void);
void closeStartMenu(void);
void k_null(void);
void k_randomVGAstyle(void);
void k_setVGAstyle(char, char, char, char, char);

#endif
