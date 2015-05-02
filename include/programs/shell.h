#ifndef _shell_
#define _shell_

#define SHELL_BUFFER_SIZE 128
#define SHELL_COMMAND_SIZE 16
#define SHELL_PARAMETER_SIZE 16

#define START_LOGO "aOS"
#define ARROW "->"

#define START_BAR_MENU_ITEM "Reboot"
#define START_BAR_MENU_SIZE 8

typedef struct{
	char buffer[SHELL_BUFFER_SIZE];
	char command[SHELL_COMMAND_SIZE];
	char parameter[SHELL_PARAMETER_SIZE];
	int bpos;
	int max_bpos;
} shell_buffer;

typedef struct{
	char* user;
	char* computername;	
} session_data;

typedef struct{
	char str[VGA_WIDTH+1];
	int menu_opened;
	char* menu_str;
} start_bar;

typedef struct{
	char* init;
	int limit;
} shell_file;

void shell(char*, char*);
void shell_set_screen(void);
void restart_shell_buffer(void);
void prompt(void);
void shell_enter(void);
void shell_backspace(void);
void shell_left(void);
void shell_right(void);
void shell_up(void);
void shell_down(void);
void shell_pageup(void);
void shell_pagedown(void);
void shell_delete(void);
void shell_insert(void);
void shell_home(void);
void shell_end(void);
void shell_print(unsigned char);
void shell_f1(void);
void shell_f2(void);
void shell_f3(void);
void shell_f4(void);
void shell_f5(void);
void shell_f6(void);
void shell_f7(void);
void shell_f8(void);
void shell_f9(void);
void shell_f10(void);
void shell_f11(void);
void shell_f12(void);
void shell_updateStartBar(void);
void setRCM(rclickmenu*);
void openRCM(rclickmenu*, int, int);
void closeRCM(rclickmenu*);
/* Listeners */
void shell_keyboardListener(void);
void shell_lclickListener(int, int);
void shell_rclickListener(int, int);
void shell_mclickListener(int, int);
void openStartMenu(void);
void closeStartMenu(void);
/* Functions to parse commands*/
void parsecommand(char*);
void getCommand(char*);
void getParameter(char*, int);
int s_about(int, char*);
int s_help(int, char*);
int s_checkBIOS(int, char*);
int s_exit(int, char*);
int s_k_reboot(int, char*);
int s_clear(int, char*);
int s_speed(int, char*);
int s_timestyle(int, char*);
int s_bkg(int, char*);
int s_logout(int, char*);

#endif