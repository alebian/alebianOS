#ifndef _defs_
#define _defs_

#define OS_VERSION "1.1.1"

#define TRUE    1
#define FALSE   0
#define EOF     -1
#define NULL    0

typedef char s8int;
typedef unsigned char u8int;
typedef unsigned char byte;
typedef short int word;
typedef short int s16int;
typedef unsigned short int u16int;
typedef int dword;
typedef int s32int;
typedef unsigned int u32int;
typedef long qword;
typedef long s64int;
typedef unsigned long u64int;

/* Segment access rights flags */
#define ACS_PRESENT     0x80            /* segment present */
#define ACS_CSEG        0x18            /* code segment */
#define ACS_DSEG        0x10            /* data segment */
#define ACS_READ        0x02            /* read segment */
#define ACS_WRITE       0x02            /* write segment */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386     0x0E    		/* Interrupt GATE 32 bits */
#define ACS_EXC_386     0x0F            /* Exception GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )
#define ACS_EXC         ( ACS_PRESENT | ACS_EXC_386 )
#define ACS_USED        0x10            /* Interrupt programed */

#define ACS_CODE        ( ACS_PRESENT | ACS_CSEG | ACS_READ )
#define ACS_DATA        ( ACS_PRESENT | ACS_DSEG | ACS_WRITE )
#define ACS_STACK       ( ACS_PRESENT | ACS_DSEG | ACS_WRITE )

#define KEYBOARD_BUFFER_SIZE  32

#pragma pack (1)    /* Allign structures to 1 byte */

/* Segment descriptor */
typedef struct {
	word limit;
	word base_l;
	byte base_m;
	byte access;
	byte attribs;
	byte base_h;
} DESCR_SEG;

#endif