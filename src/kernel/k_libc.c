#include "../../include/system.h"

static int shell_ready = 0;
static int time_style = 0;
static int vga_style = 0;
static char str_start_bar[VGA_WIDTH+1];
static int start_menu = 0;
static char current_time[9];
static klistener keyboard_listener = &k_null;

static char * days[] = { "00", "01", "02", "03", "04", "05", "06", "07", "08",
"09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
"22", "23", "24", "25", "26", "27", "28", "29", "30", "31" };

static char * months[] = { "000", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
"Aug", "Sep", "Oct", "Nov", "Dec" };

static char * years[] = { "1994", "1995", "1996", "1997", "1998", "1999", "2000", 
"2001", "2002", "2003", "2004", "2005", "2006", "2007", "2008", "2009", "2010", 
"2011", "2012", "2013", "2014", "2015", "2016", "2017", "2018", "2019", "2020" };

/***************************************************************
* setup_IDT_entry
* starts an IDT Descriptor
*
* Arguments: 
*	IDT's element pointer
*	Descriptor's selector
*	Handler pointer	
*	Access level
*	Zero
****************************************************************/

dword __write(int fd, char* buffer, dword count){
	dword c;
	int i;
	switch(fd){
		/* 1: Video output */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++)
	    	{
	    		print(buffer[i], getCharacterColor());
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

dword __read(int fd, char* buffer, dword count){
	dword c;
	int i;
	switch(fd){
		/* 1: Keyboard input */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++)
	    	{
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
	/*
	k_findSMBIOS();
	word * mem = (word*) SMBIOS->TableAddress + SMBIOShead->Length;
	printf("%s\n", "System speed:");
	printf("\t%s", "External clock frequency: ");
	printf("%d\n", mem[0x12]);
	printf("\t%s", "Maximum CPU speed: ");
	printf("%d\n", mem[0x14]);
	printf("\t%s", "Current CPU speed: ");
	printf("%d\n", mem[0x16]);
	*/
	return;
}

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

char * k_stime(){
	tm tp;
	k_time(&tp);
	char * day = days[tp.tm_mday];
	char * mon = months[tp.tm_mon];
	char * year = years[tp.tm_year];
	switch(time_style){
		case 0:
			str_start_bar[67] = day[0];
			str_start_bar[68] = day[1];
			str_start_bar[69] = ' ';
			str_start_bar[70] = mon[0];
			str_start_bar[71] = mon[1];
			str_start_bar[72] = mon[2];
			str_start_bar[73] = ',';
			str_start_bar[74] = ' ';
			str_start_bar[75] = ((tp.tm_hour & 0xF0) >> 4) + '0';
			str_start_bar[76] = ((tp.tm_hour & 0x0F)) + '0';
			str_start_bar[77] = ':';
			str_start_bar[78] = ((tp.tm_min & 0xF0) >> 4) + '0';
			str_start_bar[79] = ((tp.tm_min & 0x0F)) + '0';
			str_start_bar[80] = 0;
			break;
		case 1:
			str_start_bar[67] = ' ';
			str_start_bar[68] = ' ';
			str_start_bar[69] = ' ';
			str_start_bar[70] = ' ';
			str_start_bar[71] = ' ';
			str_start_bar[72] = ((tp.tm_hour & 0xF0) >> 4) + '0';
			str_start_bar[73] = ((tp.tm_hour & 0x0F)) + '0';
			str_start_bar[74] = ':';
			str_start_bar[75] = ((tp.tm_min & 0xF0) >> 4) + '0';
			str_start_bar[76] = ((tp.tm_min & 0x0F)) + '0';
			str_start_bar[77] = ':';
			str_start_bar[78] = ((tp.tm_sec & 0xF0) >> 4) + '0';
    		str_start_bar[79] = ((tp.tm_sec & 0x0F)) + '0';
			str_start_bar[80] = 0;
			break;
		case 2:
			str_start_bar[67] = ' ';
			str_start_bar[68] = ' ';
			str_start_bar[69] = day[0];
			str_start_bar[70] = day[1];
			str_start_bar[71] = '/';
			str_start_bar[72] = mon[0];
			str_start_bar[73] = mon[1];
			str_start_bar[74] = mon[2];
			str_start_bar[75] = '/';
			str_start_bar[76] = year[0];
			str_start_bar[77] = year[1];
			str_start_bar[78] = year[2];
    		str_start_bar[79] = year[3];
			str_start_bar[80] = 0;
			break;
		default:
			break;
	}
	return str_start_bar;
}

char * k_currentTime(){
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
	_Cli();
	k_clearFullScreen();
	set_vga_size(1, 25);
	k_setBackgroundColor(BACKGROUND_COLOR_BLACK);
	k_printalert("\n\n\n\n\n\n\n\n\n                            Goodbye! See you soon!");
	k_printalert("\n\n\n                         Please turn off your computer");
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
	str_start_bar[0] = 'a';
	str_start_bar[1] = 'O';
	str_start_bar[2] = 'S';
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
	set_vga_size(1, 25);
	k_setBackgroundColor(BACKGROUND_COLOR_BLUE);
	k_setCharacterColor(CHAR_COLOR_WHITE);
	set_vga_size(1, 25);
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

void k_printwarning(char* s){
	char aux = getCharacterColor();
	k_setCharacterColor(CHAR_COLOR_GREEN);
	printf("%s", s);
	k_setCharacterColor(aux);
	return;
}

void k_printerror(char* s){
	char aux = getCharacterColor();
	k_setCharacterColor(CHAR_COLOR_RED);
	printf("%s", s);
	k_setCharacterColor(aux);
	return;
}

void k_printalert(char* s){
	char aux = getCharacterColor();
	k_setCharacterColor(CHAR_COLOR_LIGHT_BROWN);
	printf("%s", s);
	k_setCharacterColor(aux);
	return;
}

void k_printosmsg(char* s){
	char aux = getCharacterColor();
	k_setCharacterColor(CHAR_COLOR_BLUE);
	printf("%s", s);
	k_setCharacterColor(aux);
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
		if(x>=0 && x<=2){
			if(!start_menu){
				openStartMenu();
				return;
			}else{
				closeStartMenu();
				return;
			}
		}
		else if(x>=5 && x<=10){
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
	str_start_bar[3]='-';
	str_start_bar[4]='>';
	str_start_bar[5]='R';
	str_start_bar[6]='e';
	str_start_bar[7]='b';
	str_start_bar[8]='o';
	str_start_bar[9]='o';
	str_start_bar[10]='t';
	k_updateStartBar();
	return;
}

void closeStartMenu(){
	start_menu = 0;
	str_start_bar[3]=' ';
	str_start_bar[4]=' ';
	str_start_bar[5]=' ';
	str_start_bar[6]=' ';
	str_start_bar[7]=' ';
	str_start_bar[8]=' ';
	str_start_bar[9]=' ';
	str_start_bar[10]=' ';
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
