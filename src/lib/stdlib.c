#include "../../include/lib/stdlib.h"
#include "../../include/lib/ctype.h"
#include "../../include/lib/string.h"

int atoi(const char* s){
    int i, n, sign;
    for (i = 0; isspace(s[i]); i++){} /* skip white space */
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-'){ /* skip sign */
        i++;
    }
    for (n = 0; isdigit(s[i]); i++){
        n = 10 * n + (s[i] - '0');
    }
    return sign * n;
}

char* itoa(char* s, int n, int base){
    int i, sign;
    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */
    i = 0;
    if(base == 2)
        s[i++] = 'b';
    do {     /* generate digits in reverse order */
        if(base >= 2 && base <= 10){
            s[i++] = n % base + '0'; /* get next digit */
        }
        else if(base == 16){
            int aux = n % base;
            if(aux < 10)
                s[i++] = aux + '0';
            else
                s[i++] = aux - 10 + 'A';
        }
    } while ((n /= base) > 0);   /* delete it */
    if(base == 10 && sign < 0)
        s[i++] = '-';
    if(base == 16){
        //s[i++] = 'x';
        //s[i++] = '0';
    }
    if(base == 8)
        s[i++] = '0';
    s[i] = '\0';
    reverse(s);
    return s;
}   