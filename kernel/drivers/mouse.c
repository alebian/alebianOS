/*
* Copyright 2015 Alejandro Bezdjian
* Based in JS-OS Copyright 2013 JS-OS <js@duck-squirell>
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

static MousePosition mouse_pos;
static uint8_t mouse_sensibility = 3;
static char connected = 0;

int init_mouse(){
  uint8_t status;
  if(checkConnection()){
    _Cli();
    //Enable the auxiliary mouse device
    mouse_wait(1);
    _outb(0x64, 0xA8);
    //Enable the interrupts
    mouse_wait(1);
    _outb(0x64, 0x20);
    mouse_wait(0);
    status=(_inb(0x60) | 2);
    mouse_wait(1);
    _outb(0x64, 0x60);
    mouse_wait(1);
    _outb(0x60, status);
    //Tell the mouse to use default settings
    mouse_write(0xF6);
    mouse_read();  //Acknowledge
    //Enable the mouse
    mouse_write(0xF4);
    mouse_read();  //Acknowledge

    mouse_pos.actual_x = 40;
    mouse_pos.actual_y = 12;
    mouse_pos.last_x = 40;
    mouse_pos.last_y = 12;
    mouse_pos.right_click = 0;
    mouse_pos.left_click = 0;
    mouse_pos.middle_click = 0;

    connected = 1;
    
    _Sti();
    return 0;
  }else{
    return -1;
  }
}

int checkConnection(){
    uint8_t read;
    mouse_write(0xEB);
    read = mouse_read();
    if(read == 0xFA){
        return 1;
    }else{
        return -1;
    }
}

int isMouseConnected(){
  return connected;
}

void mouse_wait(uint8_t type){
  int time_out=100000;
  if(type==0){
    // Data
    while(time_out--){
      if((_inb(0x64) & 1)==1){
        return;
      }
    }
    return;
  }else{
    // Signal
    while(time_out--){
      if((_inb(0x64) & 2)==0){
        return;
      }
    }
    return;
  }
}

uint8_t mouse_read(){
  mouse_wait(0);
  return _inb(0x60);
}

void mouse_write(uint8_t a_write){
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  _outb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  _outb(0x60, a_write);
  return;
}

void mouse_handler(uint8_t package){
  static uint8_t cycle = 0;
  static uint8_t mouse_uint8_ts[3];
  int incX, incY;
  mouse_uint8_ts[cycle++] = (uint8_t) package;

  if (cycle == 3){ 
    cycle = 0;
    if ((mouse_uint8_ts[0] & 0x80) || (mouse_uint8_ts[0] & 0x40)){
      return; // the mouse only sends information about overflowing, do not care about it and return
    }
    /* If a button is pressed */
    if ((mouse_uint8_ts[0] & 0x04) && mouse_pos.middle_click == 0){
      mouse_pos.middle_click = 1;
      k_mouseMidClick(mouse_pos.actual_x, mouse_pos.actual_y);
    }
    if ((mouse_uint8_ts[0] & 0x02) && mouse_pos.right_click == 0) {
      mouse_pos.right_click = 1;
      k_mouseRightClick(mouse_pos.actual_x, mouse_pos.actual_y);
    }
    if ((mouse_uint8_ts[0] & 0x01) && mouse_pos.left_click == 0){
      mouse_pos.left_click = 1;
      k_mouseLeftClick(mouse_pos.actual_x, mouse_pos.actual_y);
    }
    /* If a button is released */
    if ((mouse_uint8_ts[0] & 0x04) && mouse_pos.middle_click == 1){
      mouse_pos.middle_click = 0;
    }
    if ((mouse_uint8_ts[0] & 0x02) && mouse_pos.right_click == 1) {
      mouse_pos.right_click = 0;
    }
    if ((mouse_uint8_ts[0] & 0x01) && mouse_pos.left_click == 1){
      mouse_pos.left_click = 0;
    }

    incX = mouse_uint8_ts[1] % mouse_sensibility;
    incY = mouse_uint8_ts[2] % mouse_sensibility;
    erase_mouse();
    mouse_pos.last_x = mouse_pos.actual_x;
    mouse_pos.last_y = mouse_pos.actual_y;
    if(mouse_uint8_ts[0] & 0x10){
      incX = mouse_pos.actual_x - incX;
    } else {
      incX = mouse_pos.actual_x + incX;
    }
    if(mouse_uint8_ts[0] & 0x20){
      incY = mouse_pos.actual_y + incY;
    } else {
      incY = mouse_pos.actual_y - incY;
    }
    if((incX >= 0 && incX < 80) && (incY >= 0 && incY < 25)){
      mouse_pos.actual_x = incX;
      mouse_pos.actual_y = incY;
    }
    draw_mouse();
  }
  return;
}

void disableMousePackets(){
  mouse_write(0xF5);
  mouse_read();  //Acknowledge
  return;
}

void enableMousePackets(){
  mouse_write(0xF4);
  mouse_read();  //Acknowledge
  return;
}

MousePosition getMousePosition(){
  return mouse_pos;
}

void setMouseSensitivity(char sens){
  mouse_sensibility = sens;
  return;
}