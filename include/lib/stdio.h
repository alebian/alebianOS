#ifndef _stdio_
#define _stdio_

#define MAX_BUFFER 50
#define MAX_FILES 10
#define MAX_STRING_LENGTH 80

/* Input */
unsigned char getchar(void);
int scanf(char*, ...);

/* Output */
int putchar(unsigned char);
int printf(char*, ...);
int puts(char*);

#endif
