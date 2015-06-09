/*
* Copyright 2014, 2015 Alejandro Bezdjian
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

#ifndef _extras_
#define _extras_

#define START_MENU_SIZE 24

typedef struct{
	char firstline[81];
	char secondline[81];
	char enabled;
	char menu_opened;
} start_bar;

typedef struct{
	/* Shutdown   | */
	/* Reboot     | */
	/* Logout     | */
	/* Theme: 0000| */
	/* ------------ */
	char firstline[START_MENU_SIZE];
	char secondline[START_MENU_SIZE];
	char thirdline[START_MENU_SIZE];
	char savedfirstline[START_MENU_SIZE];
	char savedsecondline[START_MENU_SIZE];
	char savedthirdline[START_MENU_SIZE];
} startmenu;

void k_showLoadingScreen(void);
int k_isLoading(void);
void k_initStartBar(void);
void k_updateStartBar(void);
void k_set_start_menu(void);
void k_openStartMenu(void);
void k_closeStartMenu(void);
int k_sbarmenuopened(void);
void k_set_desktop(void);
void k_enableSbar(void);
void k_disableSbar(void);
void k_LoadingScreenAnimation(void);
void k_rebootanimation(void);
void k_shutdownScreen(void);

#endif