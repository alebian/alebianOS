#ifndef _stdio_
#define _stdio_

#define MAX_BUFFER 50
#define MAX_FILES 10
#define MAX_STRING_LENGTH 80

/* Input */
char getchar(void);
int scanf(char *fmt, ...);

/* Output */
int putchar(char c);
int printf(char *fmt, ...);
int puts(char * stream);

#endif
