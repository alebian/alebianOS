#include "../../include/system.h"

static int shell_ready = 0;
static int time_style = 0;
static int vga_style = 0;
static char current_time[14];
static klistener keyboard_listener = &k_null;
static clicklistener lclick_listener = &k_null2;
static clicklistener rclick_listener = &k_null2;
static clicklistener mclick_listener = &k_null2;

static char* months[] = { "000", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
"Aug", "Sep", "Oct", "Nov", "Dec" };

dword __write(int fd, unsigned char* buffer, dword count){
	dword c;
	int i;
	switch(fd){
		/* 1: Video output */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++){
	    		print(buffer[i], getCharacterColor());
	    		c++;
	    	}
	    	break;
	    }
	    /* Keyboard */
	    case 2:
	    	c = 0;
	    	for (i = 0; i < count; i++){
	    		add_to_keyboard_buffer(buffer[c]);
	    		c++;
	    	}	
	    default: {
	    	break;
	    }
	}
	return c;
}

dword __read(int fd, unsigned char* buffer, dword count){
	dword c;
	int i;
	switch(fd){
		/* 1: Keyboard input */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++){
	    		buffer[i] = get_char_from_keyboard_buffer();
	    		c++;
	    	}
	    	break;
	    }
	    default: {
	    	break;
	    }
	}
	return c;
}

void k_showLoadingScreen(){
	k_printalert("*******************************************************************************\n");
	k_printalert("*                                                                             *\n");
	k_printalert("*   $$$$$$$$ $$           $$       $$                      $$$$$$   $$$$$$    *\n");
	k_printalert("*   $$    $$ $$           $$                              $$    $$ $$    $$   *\n");
	k_printalert("*   $$    $$ $$   $$$$$   $$       $$  $$$$$$  $$ $$$     $$    $$ $$         *\n");
	k_printalert("*   $$$$$$$$ $$  $$   $$  $$$$$$$  $$       $$ $$$  $$$   $$    $$  $$$$$$    *\n");
	k_printalert("*   $$    $$ $$  $$$$$$   $$    $$ $$   $$$$$$ $$    $$   $$    $$       $$   *\n");
	k_printalert("*   $$    $$ $$  $$       $$    $$ $$  $$   $$ $$    $$   $$    $$ $$    $$   *\n");
	k_printalert("*   $$    $$  $$  $$$$$$  $$$$$$$  $$   $$$$$$ $$    $$    $$$$$$   $$$$$$    *\n");
	k_printalert("*                                                                             *\n");
	k_printalert("*******************************************************************************\n\n");
	return;
}

void k_checkBIOSinfo(){
	smb_BIOSinfo();
	return;
}

void k_checkSystemSpeed(){
	/*k_findSMBIOS();
	word * mem = (word*) SMBIOS->TableAddress + SMBIOShead->Length;
	printf("%s\n", "System speed:");
	printf("\t%s", "External clock frequency: ");
	printf("%d", mem[0x12]);
	printf("%s\n", "Mhz");
	printf("\t%s", "Maximum CPU speed: ");
	printf("%d", mem[0x14]);
	printf("%s\n", "Mhz");
	printf("\t%s", "Current CPU speed: ");
	printf("%d", mem[0x16]);
	printf("%s\n", "Mhz");*/
	return;
}
/*
 * Loads the time structure given with data from the RTC
 */
void k_time(tm * tp){
	_Cli();
	// Read year
	_outb(0x70, 0x09);
    tp->tm_year = _inb(0x71);
    // Read month
	_outb(0x70, 0x08);
    tp->tm_mon = _inb(0x71);
    // Read day of the month
	_outb(0x70, 0x07);
    tp->tm_mday = _inb(0x71);
    // Read day of the week
	_outb(0x70, 0x06);
    tp->tm_wday = _inb(0x71);
    // Read hour
    _outb(0x70, 0x04);
    tp->tm_hour = _inb(0x71);
    // Read minutes
    _outb(0x70, 0x02);
    tp->tm_min = _inb(0x71);
    // Read seconds
    _outb(0x70, 0x00);
    tp->tm_sec = _inb(0x71);
    _Sti();
    return;
}

char* k_stime(){
	tm tp;
	k_time(&tp);
	char* mon = months[tp.tm_mon];
	//char* year = years[tp.tm_year];
	switch(time_style){
		case 0:
			current_time[0] = ((tp.tm_mday & 0xF0) >> 4) + '0';
			current_time[1] = ((tp.tm_mday & 0x0F)) + '0';
			current_time[2] = ' ';
			current_time[3] = mon[0];
			current_time[4] = mon[1];
			current_time[5] = mon[2];
			current_time[6] = ',';
			current_time[7] = ' ';
			current_time[8] = ((tp.tm_hour & 0xF0) >> 4) + '0';
			current_time[9] = ((tp.tm_hour & 0x0F)) + '0';
			current_time[10] = ':';
			current_time[11] = ((tp.tm_min & 0xF0) >> 4) + '0';
			current_time[12] = ((tp.tm_min & 0x0F)) + '0';
			current_time[13] = 0;
			break;
		case 1:
			current_time[0] = ' ';
			current_time[1] = ' ';
			current_time[2] = ' ';
			current_time[3] = ' ';
			current_time[4] = ' ';
			current_time[5] = ((tp.tm_hour & 0xF0) >> 4) + '0';
			current_time[6] = ((tp.tm_hour & 0x0F)) + '0';
			current_time[7] = ':';
			current_time[8] = ((tp.tm_min & 0xF0) >> 4) + '0';
			current_time[9] = ((tp.tm_min & 0x0F)) + '0';
			current_time[10] = ':';
			current_time[11] = ((tp.tm_sec & 0xF0) >> 4) + '0';
			current_time[12] = ((tp.tm_sec & 0x0F)) + '0';
			current_time[13] = 0;
			break;
		case 2:
			current_time[0] = ' ';
			current_time[1] = ' ';
			current_time[2] = ((tp.tm_mday & 0xF0) >> 4) + '0';
			current_time[3] = ((tp.tm_mday & 0x0F)) + '0';
			current_time[4] = '/';
			current_time[5] = mon[0];
			current_time[6] = mon[1];
			current_time[7] = mon[2];
			current_time[8] = '/';
			current_time[9] = '2';
			current_time[10] = '0';
			current_time[11] = ((tp.tm_year & 0xF0) >> 4) + '0';
			current_time[12] = ((tp.tm_year & 0x0F)) + '0';
			current_time[13] = 0;
			break;
		default:
			break;
	}
	return current_time;
}

void k_setTimeStyle(int sty){
	time_style = sty;
	shell_updateStartBar();
	return;
}

int k_getTimeStyle(){
	return time_style;
}

void k_exitScreen(){
	k_shellNotReady();
	k_disableMouse();
	MARIO();
	set_vga_size(1,25);
	k_printalert("                         Please turn off your computer...");
	song_pacman();
	_Cli();
	/*
	k_clearFullScreen();
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	set_vga_size(10, 25);
	k_printalert("                            Goodbye! See you soon!\n\n\n");
	k_printalert("                         Please turn off your computer");
	*/
	k_turnOff();
	return;
}

void k_clearScreen(){
	clearScreen();
	return;
}

void k_clearFullScreen(){
	clearFullScreen();
	return;
}

void k_scrolldown(){
	scrolldown();
	return;
}

char k_getBackgroundColor(){
	return getBackgroundColor();
}

void k_setBackgroundColor(char color){
	setBackgroundColor(SECONDBYTE(color));
	return;
}

void k_setFullBackgroundColor(char color){
	setFullBackgroundColor(SECONDBYTE(color));
	return;
}

char k_getCharacterColor(){
	return getCharacterColor();
}

void k_setCharacterColor(char color){
	setCharacterColor(FIRSTBYTE(color));
	return;
}

void k_setAllCharacterColor(char color){
	setAllCharacterColor(FIRSTBYTE(color));
	return;
}

char k_getMouseColor(){
	return getMouseColor();
}

void k_setMouseColor(char back, char arrow){
	setMouseColor(SECONDBYTE(back), FIRSTBYTE(arrow));
	return;
}

void k_printStartBar(char* start){
	printStartBar(start);
	return;
}

void k_setStartBarColor(char color){
	setStartBarColor(SECONDBYTE(color));
	shell_updateStartBar();
	return;
}

void k_sleep(int ticks){
    unsigned long aux;
    aux = getTicks() + ticks;
    while(getTicks() < aux){}
    return;
}

void k_reboot(){
	int ticks = 10;
	k_shellNotReady();
	k_disableMouse();
	_Sti();
	k_clearFullScreen();
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLUE);
	k_setCharacterColor(CHAR_COLOR_WHITE);
	printf("\n\n\n\n\n\n%s", "                                  Rebooting in...");
	set_vga_size(9, 25);
	printf("%s\n", "                                   $$$$$$$$$$");
	printf("%s\n", "                                   $$");
	printf("%s\n", "                                   $$");
	printf("%s\n", "                                   $$$$$$$$$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s", "                                   $$$$$$$$$");
	k_sleep(ticks);
	k_clearScreen();
	printf("%s\n", "                                   $$      $$");
	printf("%s\n", "                                   $$      $$");
	printf("%s\n", "                                   $$      $$");
	printf("%s\n", "                                   $$      $$");
	printf("%s\n", "                                   $$$$$$$$$$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s", "                                           $$");
	k_sleep(ticks);
	k_clearScreen();
	printf("%s\n", "                                    $$$$$$$$$");
	printf("%s\n", "                                          $$");
	printf("%s\n", "                                         $$");
	printf("%s\n", "                                      $$$$$$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s", "                                    $$$$$$$$");
	k_sleep(ticks);
	k_clearScreen();
	printf("%s\n", "                                   $$$$$$$$$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                          $$");
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                      $$");
	printf("%s\n", "                                    $$");
	printf("%s", "                                   $$$$$$$$$$");
	k_sleep(ticks);
	k_clearScreen();
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                      $$$$");
	printf("%s\n", "                                     $$ $$");
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                        $$");
	printf("%s", "                                      $$$$$$");
	k_sleep(ticks);
	k_clearScreen();
	_outb(0x64, 0xFE);
	return;
}

/*
 * Changes the frequency of PIC's Channel 0
 */
void k_set_frecuency(int frequency){
  // The value we send to the PIT is the value to divide it's input clock
  // (1193180 Hz) by, to get our required frequency. Important to note is
  // that the divisor must be small enough to fit into 16-bits.
  int divisor = 1193180 / frequency;
  // Send the command byte.
  _outb(0x43, 0x36);
  // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
  char l = (char)(divisor & 0xFF);
  char h = (char)( (divisor>>8) & 0xFF );
  // Send the frequency divisor.
  _outb(0x40, l);
  _outb(0x40, h);
  return;
}

int k_isReadyToShell(){
	return shell_ready;
}

void k_shellReady(){
	shell_ready = 1;
	return;
}

void k_shellNotReady(){
	shell_ready = 0;
	return;
}

void k_panic(char* message){
	k_shellNotReady();
	k_disableMouse();
	k_clearFullScreen();
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLUE);
	set_vga_size(1, 25);
	k_setCharacterColor(CHAR_COLOR_WHITE);
	printf("\n%s\n", message);
	printf("\n%s\n", "Halting");
	k_beep();
	for (;;){
		turn_off_leds();
		k_sleep(10);
		turn_on_leds();
		k_sleep(10);
	}
	return;
}

void k_KeyboardListener(){
	keyboard_listener();
	return;
}

void k_setKeyboardListener(klistener listener){
	keyboard_listener = listener;
	return;
}

void k_printWithColor(char* s, char color){
	char aux = getCharacterColor();
	k_setCharacterColor(color);
	printf("%s", s);
	k_setCharacterColor(aux);
	return;
}

void k_printwarning(char* s){
	k_printWithColor(s, CHAR_COLOR_GREEN);
	return;
}

void k_printerror(char* s){
	k_printWithColor(s, CHAR_COLOR_RED);
	return;
}

void k_printalert(char* s){
	k_printWithColor(s, CHAR_COLOR_LIGHT_BROWN);
	return;
}

void k_printosmsg(char* s){
	k_printWithColor(s, CHAR_COLOR_BLUE);
	return;
}

void k_drawMouse(){
	MousePosition mouse = getMousePosition();
	drawMouse(mouse.actual_x, mouse.actual_y);
	return;
}

void k_eraseMouse(){
	MousePosition mouse = getMousePosition();
	eraseMouse(mouse.actual_x, mouse.actual_y);
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

void k_null(){
	return;
}

void k_null2(int x, int y){
	return;
}

void k_randomVGAstyle(){
	switch(vga_style){
		case 0:
			k_setVGAstyle(BACKGROUND_COLOR_LIGHT_GREY, CHAR_COLOR_BLACK, BACKGROUND_COLOR_BLACK, CHAR_COLOR_LIGHT_GREY, BACKGROUND_COLOR_LIGHT_GREY);
			vga_style++;
			break;
		case 1:
			k_setVGAstyle(BACKGROUND_COLOR_BLUE, CHAR_COLOR_WHITE, BACKGROUND_COLOR_BLACK, CHAR_COLOR_RED, BACKGROUND_COLOR_BLUE);
			vga_style++;
			break;
		case 2:
			k_setVGAstyle(BACKGROUND_COLOR_RED, CHAR_COLOR_WHITE, BACKGROUND_COLOR_BLACK, CHAR_COLOR_RED, BACKGROUND_COLOR_RED);
			vga_style++;
			break;
		default:
			k_setVGAstyle(BACKGROUND_COLOR_BLACK, CHAR_COLOR_LIGHT_GREY, BACKGROUND_COLOR_RED, CHAR_COLOR_WHITE, BACKGROUND_COLOR_LIGHT_GREY);
			vga_style = 0;
			break;
	}
	return;
}

void k_setVGAstyle(char background, char character, char mouseback, char mousechar, char startbar){
	k_setBackgroundColor(SECONDBYTE(background));
	setAllCharacterColor(FIRSTBYTE(character));
	k_eraseMouse();
	setMouseColor(SECONDBYTE(mouseback), FIRSTBYTE(mousechar));
	k_drawMouse();
	k_setStartBarColor(SECONDBYTE(startbar));
	return;
}

void k_move_cursor_back(){
	move_cursor_back();
	return;
}

void k_move_cursor_forward(){
	move_cursor_forward();
	return;
}

void k_set_vga_size(int min, int max){
	set_vga_size(min, max);
	return;
}

void k_beep(){
	beep();
	return;
}