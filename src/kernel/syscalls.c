#include "../../include/system.h"

void k_syscall(dword syscall, dword ebx, dword ecx, dword edx){
	switch(syscall){
		/* Syscall: Read */
		case 3:
			__read(ebx, (unsigned char*)ecx, edx);
			break;
		/* Syscall: Write */
		case 4:
			__write(ebx, (unsigned char*)ecx, edx);
			break;
		default:
			break;
	}
	return;
}

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