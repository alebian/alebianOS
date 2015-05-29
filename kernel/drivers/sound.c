/*
 * Copyright 2013 JS-OS <js@duck-squirell>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */
 
#include "../include/system.h"

void play_sound(unsigned int freq){
	unsigned int Div;
	uint8_t tmp;

    //Set the PIT to the desired frequency
	Div = 1193180 / freq;
	_outb(0x43, 0xb6);
	_outb(0x42, (uint8_t) (Div) );
	_outb(0x42, (uint8_t) (Div >> 8));

    //And play the sound using the PC speaker
	tmp = _inb(0x61);
	if (tmp != (tmp | 3)){
		_outb(0x61, tmp | 3);
	}
	return;
}

void nosound(){
	uint8_t tmp = _inb(0x61) & 0xFC;
	_outb(0x61, tmp);
	return;
}

void beep(){
	play_sound(1000);
	sleep(10);
	nosound();
	return;
}

void playNote(char* note, int milliseconds){
	unsigned int frequency;
	
	if(*(note) == 'C'){
		if(*(note + 1) == '4'){
			if(*(note + 2) == '#'){
				frequency = 277;
			}else{
				frequency = 262;
			}
		}else if(*(note + 1) == '5'){
			if(*(note + 2) == '#'){
				frequency = 554;
			}else{
				frequency = 523;
			}      
		}else if(*(note + 1) == '6'){
			if(*(note + 2) == '#'){
				frequency = 1109;
			}else{
				frequency = 1046;
			}      
		}
	}else if(*(note) == 'D'){
		if(*(note + 1) == '4'){
			if(*(note + 2) == 'b'){
				frequency = 277;
			}else if(*(note + 2) == '#'){
				frequency = 311;
			}else{
				frequency = 294;
			}      
		}else if(*(note + 1) == '5'){
			if(*(note + 2) == 'b'){
				frequency = 554;
			}else if(*(note + 2) == '#'){
				frequency = 622;
			}else{
				frequency = 587;
			}
		}
	}else if(*(note) == 'E'){
		if(*(note + 1) == '4'){
			if(*(note + 2) == 'b'){
				frequency = 311;
			}else{
				frequency = 330;
			}      
		}else if(*(note + 1) == '5'){
			if(*(note + 2) == 'b'){
				frequency = 622;
			}else
			{
				frequency = 659;
			}      
		}
	}else if(*(note) == 'F'){
		if(*(note + 1) == '4'){
			if(*(note + 2) == '#'){
				frequency = 370;
			}else{
				frequency = 349;
			}      
		}else if(*(note + 1) == '5'){
			if(*(note + 2) == '#'){
				frequency = 740;
			}else{
				frequency = 698;
			}      
		}
	}else if(*(note) == 'G'){
		if(*(note + 1) == '4'){
			if(*(note + 2) == 'b'){
				frequency = 370;
			}else if(*(note + 2) == '#'){
				frequency = 415;
			}else{
				frequency = 392;
			}      
		}else if(*(note + 1) == '5'){
			if(*(note + 2) == 'b'){
				frequency = 740;
			}else if(*(note + 2) == '#'){
				frequency = 831;
			}else{
				frequency = 784;
			}      
		}
	}else if(*(note) == 'A'){
		if(*(note + 1) == '4'){
			if(*(note + 2) == 'b'){
				frequency = 415;
			}else if(*(note + 2) == '#'){
				frequency = 466;
			}else{
				frequency = 440;
			}      
		}else if(*(note + 1) == '5'){
			if(*(note + 2) == 'b'){
				frequency = 831;
			}else if(*(note + 2) == '#'){
				frequency = 932;
			}else{
				frequency = 880;
			}      
		}
	}else if(*(note) == 'B'){
		if(*(note + 1) == '4'){
			if(*(note + 2) == 'b'){
				frequency = 466;
			}else{
				frequency = 494;
			}      
		}else if(*(note + 1) == '5'){
			if(*(note + 2) == 'b'){
				frequency = 932;
			}else{
				frequency = 988;
			}      
		}
	}

	play_sound(frequency);
	sleep(milliseconds);
	nosound();
	return;
}

void song_pacman(){
	int wholeNote = 1100;
	int quarter = wholeNote / 4, eigth = wholeNote / 8, sixteenth = wholeNote / 16;

	//playNote("A5", eigth);
	playNote("B4", eigth);
	playNote("A5", eigth);
	playNote("F5#", eigth);
	playNote("E5b", eigth);

	playNote("A5", sixteenth);
	playNote("F5#", eigth);
	playNote("E5b", quarter);

	playNote("C5", eigth);
	playNote("C6", eigth);
	playNote("G5", eigth);
	playNote("E5", eigth);

	playNote("C6", sixteenth);
	playNote("G5", eigth);
	playNote("E5", quarter);

	playNote("B4", eigth);
	playNote("A5", eigth);
	playNote("F5#", eigth);
	playNote("E5b", eigth);

	playNote("A5", sixteenth);
	playNote("F5#", eigth);
	playNote("E5b", quarter);

	playNote("E5b", sixteenth);
	playNote("E5", sixteenth);
	playNote("F5", eigth);
	playNote("F5", sixteenth);
	playNote("F5#", sixteenth);
	playNote("G5", eigth);

	playNote("G5", sixteenth);
	playNote("A5b", sixteenth);
	playNote("A5", eigth);
	playNote("B5", quarter);
	return;
}
