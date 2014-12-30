#ifndef _shell_
#define _shell_

#define SHELL_PROMPT_SIZE 3

void shell();
void prompt();
void parsecommand(char * s);
void about();
void help();

#endif