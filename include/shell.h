#ifndef _shell_
#define _shell_

#define SHELL_BUFFER_SIZE 150
#define SHELL_PROMPT_SIZE 3
#define SHELL_COMMAND_SIZE 20
#define SHELL_PARAMETER_SIZE 20

void shell(void);
void shell_keyboardListener(void);
void restart_shell_buffer(void);
void shellLineBreak(void);
void prompt(void);
/* Functions to parse commands*/
void parsecommand(char *);
void getCommand(char *);
void getParameter(char *, int);
int s_about(int, char *);
int s_help(int, char *);
int s_checkBIOS(int, char *);
int s_exit(int, char *);
int s_k_reboot(int, char *);
int s_clear(int, char *);
int s_speed(int, char *);
int s_time(int, char *);
int s_timestyle(int, char *);
int s_bkg(int, char *);

#endif