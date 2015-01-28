#include "../../include/system.h"

static int shell_ready = 0;
static int time_style = 0;
static int vga_style = 0;
static char str_start_bar[VGA_WIDTH+1];
static int start_menu = 0;
static char current_time[9];
static klistener keyboard_listener = &k_null;
static char start_menu_str[] = "Reboot";

// static char* days[] = { "00", "01", "02", "03", "04", "05", "06", "07", "08",
// "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
// "22", "23", "24", "25", "26", "27", "28", "29", "30", "31" };

static char* months[] = { "000", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
"Aug", "Sep", "Oct", "Nov", "Dec" };

// static char* years[] = { "1994", "1995", "1996", "1997", "1998", "1999", "2000", 
// "2001", "2002", "2003", "2004", "2005", "2006", "2007", "2008", "2009", "2010", 
// "2011", "2012", "2013", "2014", "2015", "2016", "2017", "2018", "2019", "2020" };

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
	memset(&str_start_bar[67], ' ', 13);
	switch(time_style){
		case 0:
			str_start_bar[67] = ((tp.tm_mday & 0xF0) >> 4) + '0';
			str_start_bar[68] = ((tp.tm_mday & 0x0F)) + '0';
			str_start_bar[70] = mon[0];
			str_start_bar[71] = mon[1];
			str_start_bar[72] = mon[2];
			str_start_bar[73] = ',';
			memcpy(&str_start_bar[75], k_currentTime(), 5);
			break;
		case 1:
			memcpy(&str_start_bar[72], k_currentTime(), 8);
			break;
		case 2:
			str_start_bar[69] = ((tp.tm_mday & 0xF0) >> 4) + '0';
			str_start_bar[70] = ((tp.tm_mday & 0x0F)) + '0';
			str_start_bar[71] = '/';
			str_start_bar[72] = mon[0];
			str_start_bar[73] = mon[1];
			str_start_bar[74] = mon[2];
			str_start_bar[75] = '/';
			str_start_bar[76] = '2';
			str_start_bar[77] = '0';
			str_start_bar[78] = ((tp.tm_year & 0xF0) >> 4) + '0';
			str_start_bar[79] = ((tp.tm_year & 0x0F)) + '0';
			break;
		default:
			break;
	}
	return str_start_bar;
}

char* k_currentTime(){
	tm tp;
	k_time(&tp);
	current_time[0] = ((tp.tm_hour & 0xF0) >> 4) + '0';
	current_time[1] = ((tp.tm_hour & 0x0F)) + '0';
	current_time[2] = ':';
	current_time[3] = ((tp.tm_min & 0xF0) >> 4) + '0';
	current_time[4] = ((tp.tm_min & 0x0F)) + '0';
	current_time[5] = ':';
	current_time[6] = ((tp.tm_sec & 0xF0) >> 4) + '0';
	current_time[7] = ((tp.tm_sec & 0x0F)) + '0';
	current_time[8] = 0;
	return current_time;
}

void k_setTimeStyle(int sty){
	time_style = sty;
	k_updateStartBar();
	return;
}

void k_turnOff(){
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
	while(1){}
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

void k_scrollup(){
	scrollup();
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

void k_updateStartBar(){
	k_stime();
	memcpy(&str_start_bar, &START_LOGO, strlen(START_LOGO));
	printStartBar(str_start_bar);
	return;
}

void k_setStartBarColor(char color){
	setStartBarColor(SECONDBYTE(color));
	k_updateStartBar();
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
	beep();
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
	k_mouseLeftClick(x, y);
	return;
}

void k_mouseLeftClick(int x, int y){
	if(y==0){
		if(x>=0 && x<strlen(START_LOGO)){
			if(!start_menu){
				openStartMenu();
				return;
			}else{
				closeStartMenu();
				return;
			}
		}
		else if( x>=(strlen(START_LOGO)+strlen(ARROW)) && x<(strlen(START_LOGO)+strlen(ARROW)+strlen(start_menu_str)) ){
			if(start_menu){
				k_reboot();
			}
		}
		else if(x>=72){
			int aux = time_style;
			switch(aux){
				case 2:
					aux=0;
					break;
				default:
					aux++;
					break;
			}
			k_setTimeStyle(aux);
			return;
		}else{
			closeStartMenu();
			return;
		}
	}else{
		if(start_menu){
			closeStartMenu();
			return;
		}
	}
	return;
}

void openStartMenu(){
	start_menu = 1;
	memcpy(&str_start_bar[strlen(START_LOGO)], &ARROW, strlen(ARROW));
	memcpy(&str_start_bar[strlen(START_LOGO)+strlen(ARROW)], &start_menu_str, strlen(start_menu_str));
	k_updateStartBar();
	return;
}

void closeStartMenu(){
	start_menu = 0;
	memset(&str_start_bar[strlen(START_LOGO)], ' ', strlen(ARROW)+strlen(start_menu_str));
	k_updateStartBar();
	return;
}

void k_mouseMidClick(int x, int y){
	k_randomVGAstyle();
	return;
}

void k_null(){
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
