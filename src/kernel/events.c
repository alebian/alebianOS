#include "../../include/kernel/events.h"

static klistener keyboard_listener = &k_OSkblistener;
static clicklistener lclick_listener = &k_OSlclcklistener;
static clicklistener rclick_listener = &k_OSrclcklistener;
static clicklistener mclick_listener = &k_OSmclcklistener;

void k_KeyboardListener(){
	keyboard_listener();
	return;
}

void k_setKeyboardListener(klistener listener){
	keyboard_listener = listener;
	return;
}

void k_mouseRightClick(int x, int y){
	rclick_listener(x,y);
	return;
}

void k_setRclickListener(clicklistener listener){
	rclick_listener = listener;
	return;
}

void k_mouseLeftClick(int x, int y){
	lclick_listener(x,y);
	return;
}

void k_setLclickListener(clicklistener listener){
	lclick_listener = listener;
	return;
}

void k_mouseMidClick(int x, int y){
	mclick_listener(x,y);
	return;
}

void k_setMclickListener(clicklistener listener){
	mclick_listener = listener;
	return;
}

void k_OSkblistener(){
	return;
}

void k_OSlclcklistener(int x, int y){
	return;
}

void k_OSrclcklistener(int x, int y){
	return;
}

void k_OSmclcklistener(int x, int y){
	return;
}