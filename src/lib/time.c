#include "../../include/lib/time.h"
#include "../../include/kernel/k_libasm.h"

time_t time(time_t * tp) {
    /*tm aux;
    _Cli();
    _outb(0x70, 4);
    aux->tm_hour = _inb(0x71);
    _outb(0x70, 2);
    aux->tm_min = _inb(0x71);
    _outb(0x70, 0);
    aux->tm_sec = _inb(0x71);
    _Sti();*/
    return 1;
}

char * asctime(tm * tp) {
    static char str_time[9];
    str_time[0] = ((tp->tm_hour & 0xF0) >> 4) + '0';
    str_time[1] = ((tp->tm_hour & 0x0F)) + '0';
    str_time[2] = ':';
    str_time[3] = ((tp->tm_min & 0xF0) >> 4) + '0';
    str_time[4] = ((tp->tm_min & 0x0F)) + '0';
    str_time[5] = ':';
    str_time[6] = ((tp->tm_sec & 0xF0) >> 4) + '0';
    str_time[7] = ((tp->tm_sec & 0x0F)) + '0';
    str_time[8] = 0;
    return str_time;
}

clock_t clock(void){
    clock_t ans = -1;
    return ans;
}

double difftime(time_t t1, time_t t2){
    double ans = -1;
    return ans;
}