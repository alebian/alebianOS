#ifndef _events_
#define _events_

typedef void(*klistener)(void);
typedef void(*clicklistener)(int, int);

void k_KeyboardListener(void);
void k_setKeyboardListener(void(*listener)(void));
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

#endif