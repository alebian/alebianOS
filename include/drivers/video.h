#ifndef _video_
#define _video_

#define VGA_PORT 0xB8000
#define VGA_BACKUP 0x200000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define TOTAL_CHARS 4000 // VGA_WIDTH*VGA_HEIGHT*2

#define V_TAB 6

#define CHAR_BLANK ' '

#define CHAR_COLOR_BLACK 0x00
#define CHAR_COLOR_BLUE 0x01
#define CHAR_COLOR_GREEN 0x02
#define CHAR_COLOR_CYAN 0x03
#define CHAR_COLOR_RED 0x04
#define CHAR_COLOR_MAGENTA 0x05
#define CHAR_COLOR_BROWN 0x06
#define CHAR_COLOR_LIGHT_GREY 0x07
#define CHAR_COLOR_DARK_GREY 0x08
#define CHAR_COLOR_LIGHT_BLUE 0x09
#define CHAR_COLOR_LIGHT_GREEN 0x0A
#define CHAR_COLOR_LIGHT_CYAN 0x0B
#define CHAR_COLOR_LIGHT_RED 0x0C
#define CHAR_COLOR_LIGHT_MAGENTA 0x0D
#define CHAR_COLOR_LIGHT_BROWN 0x0E
#define CHAR_COLOR_WHITE 0x0F

#define BACKGROUND_COLOR_BLACK 0x00
#define BACKGROUND_COLOR_BLUE 0x10
#define BACKGROUND_COLOR_GREEN 0x20
#define BACKGROUND_COLOR_CYAN 0x30
#define BACKGROUND_COLOR_RED 0x40
#define BACKGROUND_COLOR_MAGENTA 0x50
#define BACKGROUND_COLOR_BROWN 0x60
#define BACKGROUND_COLOR_LIGHT_GREY 0x70
#define BACKGROUND_COLOR_DARK_GREY 0x80
#define BACKGROUND_COLOR_LIGHT_BLUE 0x90
#define BACKGROUND_COLOR_LIGHT_GREEN 0xA0
#define BACKGROUND_COLOR_LIGHT_CYAN 0xB0
#define BACKGROUND_COLOR_LIGHT_RED 0xC0
#define BACKGROUND_COLOR_LIGHT_MAGENTA 0xD0
#define BACKGROUND_COLOR_LIGHT_BROWN 0xE0
#define BACKGROUND_COLOR_WHITE 0xF0

#define START_MENU_SIZE 24

typedef struct{
	/* Shutdown   | */
	/* Reboot     | */
	/* Logout     | */
	/* Theme: 0000| */
	/* ------------ */
	char firstline[START_MENU_SIZE];
	char secondline[START_MENU_SIZE];
	char thirdline[START_MENU_SIZE];
	char fourthline[START_MENU_SIZE];
	char fifthline[START_MENU_SIZE];
	char savedfirstline[START_MENU_SIZE];
	char savedsecondline[START_MENU_SIZE];
	char savedthirdline[START_MENU_SIZE];
	char savedfourthline[START_MENU_SIZE];
	char savedfifthline[START_MENU_SIZE];
} startmenu;

/* Functions*/
void clearScreen(void); // Clears chars from screen between rows set by set_vga_size
void clearLine(int); // Clears row selected by parameter (1 to 25)
void clearCurrentLine(void);
void clearFullScreen(void); // Clears all chars from screen
char getBackgroundColor(void); // Returns the current background color between rows set by set_vga_size
void setBackgroundColor(char); // Changes the background between rows set by set_vga_size
void setFullBackgroundColor(char); // Changes the entire screen background
char getCharacterColor(void); // Returns the current char color between rows set by set_vga_size
void setCharacterColor(char); // Changes the color that print will use next
void setAllCharacterColor(char); // Changes all chars color between rows set by set_vga_size
void setStartBarColor(char); // Changes the background color of the start bar
char getMouseColor(void);
void setMouseColor(char, char);
void set_vga_size(int, int); // Changes the screen size that other functions can affect
void printxy(char, int, int);
void printxyc(char, char, int, int);
void print(char, char);
void printFromFile(char*, int, int);
void move_cursor_back(void);
void move_cursor_forward(void);
void update_cursor(int);
void scrolldown(void);
void scrollup(void);
void printStartBar(char*);
void draw_mouse();
void erase_mouse();
void printSquare(int, int, char);
void set_start_menu(void);
void set_start_menuColor(char);
void print_start_menu(void);
void clear_start_menu(void);
void switch_VGA_style(int);
void set_VGA_style(char, char, char, char, char);

#endif