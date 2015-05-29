/*
* Copyright 2014 Alejandro Bezdjian
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../include/lib/stdlib.h"
#include "../include/lib/ctype.h"
#include "../include/lib/string.h"

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