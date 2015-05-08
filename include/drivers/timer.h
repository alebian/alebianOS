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

#ifndef _timer_
#define _timer_

#define TIME_STYLES 3

typedef long time_t;
typedef long clock_t;

typedef struct {
	int tm_sec;         /* seconds,  range 0 to 59          */
	int tm_min;         /* minutes, range 0 to 59           */
	int tm_hour;        /* hours, range 0 to 23             */
	int tm_mday;        /* day of the month, range 1 to 31  */
	int tm_mon;         /* month, range 0 to 11             */
	int tm_year;        /* The number of years since 1900   */
	int tm_wday;        /* day of the week, range 0 to 6    */
	int tm_yday;        /* day in the year, range 0 to 365  */
	int tm_isdst;       /* daylight saving time             */
} __attribute__ ((aligned)) tm;


int set_frecuency(int);
void sleep(int);
void read_time(tm*);
char* get_time(void);
void set_time_style(int);
void change_time_style(void);

#endif