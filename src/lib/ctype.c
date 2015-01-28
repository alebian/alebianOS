#include "../../include/lib/ctype.h"

int isspace(int c){
    return (c == ' ') || (c == '\t') || (c == '\n');
}

int isdigit(int c){
    return (c >= '0') && (c <= '9');
}

int isxdigit(int c){
    return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int isalpha(int c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isspecial(int c){
	return (c=='!'||c=='@'||c=='#'||c=='$'||c=='%'||c=='&');
}
