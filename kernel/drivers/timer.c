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

#include "../include/system.h"

static int time_style = 0;
static char current_time[14];
static char* months[] = { "000", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
"Aug", "Sep", "Oct", "Nov", "Dec" };
/*
 * Changes the frequency of PIC's Channel 0
 */
int set_frecuency(int frequency){
  // The value we send to the PIT is the value to divide it's input clock
  // (1193180 Hz) by, to get our required frequency. Important to note is
  // that the divisor must be small enough to fit into 16-bits.
  int divisor = 1193180 / frequency;
  // Send the command uint8_t.
  _outb(0x43, 0x36);
  // Divisor has to be sent uint8_t-wise, so split here into upper/lower uint8_ts.
  uint8_t l = (uint8_t)(divisor & 0xFF);
  uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );
  // Send the frequency divisor.
  _outb(0x40, l);
  _outb(0x40, h);
  return frequency;
}

void sleep(int ticks){
    unsigned long aux;
    aux = getTicks() + ticks;
    while(getTicks() < aux){}
    return;
}

/*
 * Loads the time structure given with data from the RTC
 */
void read_time(tm * tp){
	_Cli();
	// Read year
	_outb(0x70, 0x09);
    tp->tm_year = _inb(0x71);
    // Read month
	_outb(0x70, 0x08);
    tp->tm_mon = _inb(0x71);
    // Read day of the month
	_outb(0x70, 0x07);
    tp->tm_mday = _inb(0x71);
    // Read day of the week
	_outb(0x70, 0x06);
    tp->tm_wday = _inb(0x71);
    // Read hour
    _outb(0x70, 0x04);
    tp->tm_hour = _inb(0x71);
    // Read minutes
    _outb(0x70, 0x02);
    tp->tm_min = _inb(0x71);
    // Read seconds
    _outb(0x70, 0x00);
    tp->tm_sec = _inb(0x71);
    _Sti();
    return;
}

char* get_time(){
	tm tp;
	read_time(&tp);
	char* mon = months[tp.tm_mon];
	//char* year = years[tp.tm_year];
	switch(time_style){
		case 0:
			current_time[0] = ((tp.tm_mday & 0xF0) >> 4) + '0';
			current_time[1] = ((tp.tm_mday & 0x0F)) + '0';
			current_time[2] = ' ';
			current_time[3] = mon[0];
			current_time[4] = mon[1];
			current_time[5] = mon[2];
			current_time[6] = ',';
			current_time[7] = ' ';
			current_time[8] = ((tp.tm_hour & 0xF0) >> 4) + '0';
			current_time[9] = ((tp.tm_hour & 0x0F)) + '0';
			current_time[10] = ':';
			current_time[11] = ((tp.tm_min & 0xF0) >> 4) + '0';
			current_time[12] = ((tp.tm_min & 0x0F)) + '0';
			current_time[13] = 0;
			break;
		case 1:
			current_time[0] = ' ';
			current_time[1] = ' ';
			current_time[2] = ' ';
			current_time[3] = ' ';
			current_time[4] = ' ';
			current_time[5] = ((tp.tm_hour & 0xF0) >> 4) + '0';
			current_time[6] = ((tp.tm_hour & 0x0F)) + '0';
			current_time[7] = ':';
			current_time[8] = ((tp.tm_min & 0xF0) >> 4) + '0';
			current_time[9] = ((tp.tm_min & 0x0F)) + '0';
			current_time[10] = ':';
			current_time[11] = ((tp.tm_sec & 0xF0) >> 4) + '0';
			current_time[12] = ((tp.tm_sec & 0x0F)) + '0';
			current_time[13] = 0;
			break;
		case 2:
			current_time[0] = ' ';
			current_time[1] = ' ';
			current_time[2] = ((tp.tm_mday & 0xF0) >> 4) + '0';
			current_time[3] = ((tp.tm_mday & 0x0F)) + '0';
			current_time[4] = '/';
			current_time[5] = mon[0];
			current_time[6] = mon[1];
			current_time[7] = mon[2];
			current_time[8] = '/';
			current_time[9] = '2';
			current_time[10] = '0';
			current_time[11] = ((tp.tm_year & 0xF0) >> 4) + '0';
			current_time[12] = ((tp.tm_year & 0x0F)) + '0';
			current_time[13] = 0;
			break;
		default:
			break;
	}
	return current_time;
}

char* get_current_time(){
	char* ans;
	int aux = time_style;
	time_style = 1;
	ans = get_time();
	time_style = aux;
	return ans;
}

void set_time_style(int sty){
	if(sty < TIME_STYLES){
		time_style = sty;
		k_updateStartBar();
	}
	return;
}

void change_time_style(){
	switch(time_style){
		case 2:
			time_style=0;
			break;
		default:
			time_style++;
		break;
	}
	set_time_style(time_style);
	k_updateStartBar();
	return;
}