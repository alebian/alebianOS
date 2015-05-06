#include "../../include/arch/x86/x86.h"
#include "../../include/kernel/events.h"
#include "../../include/kernel/interrupts.h"
#include "../../include/drivers/video.h"
#include "../../include/drivers/mouse.h"

/*
 * This driver is based on the one in:
 * JS-OS Copyright 2013 JS-OS <js@duck-squirell>
 */

static MousePosition mouse_pos;
static byte mouse_sensibility = 3;
static int connected = 0;

int init_mouse(){
  byte status;
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
    byte read;
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

void mouse_wait(byte type){
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

byte mouse_read(){
  mouse_wait(0);
  return _inb(0x60);
}

void mouse_write(byte a_write){
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

void mouse_handler(byte package){
  static byte cycle = 0;
  static byte mouse_bytes[3];
  int incX, incY;
  mouse_bytes[cycle++] = (byte) package;

  if (cycle == 3){ 
    cycle = 0;
    if ((mouse_bytes[0] & 0x80) || (mouse_bytes[0] & 0x40)){
      return; // the mouse only sends information about overflowing, do not care about it and return
    }
    /* If a button is pressed */
    if ((mouse_bytes[0] & 0x04) && mouse_pos.middle_click == 0){
      mouse_pos.middle_click = 1;
      k_mouseMidClick(mouse_pos.actual_x, mouse_pos.actual_y);
    }
    if ((mouse_bytes[0] & 0x02) && mouse_pos.right_click == 0) {
      mouse_pos.right_click = 1;
      k_mouseRightClick(mouse_pos.actual_x, mouse_pos.actual_y);
    }
    if ((mouse_bytes[0] & 0x01) && mouse_pos.left_click == 0){
      mouse_pos.left_click = 1;
      k_mouseLeftClick(mouse_pos.actual_x, mouse_pos.actual_y);
    }
    /* If a button is released */
    if ((mouse_bytes[0] & 0x04) && mouse_pos.middle_click == 1){
      mouse_pos.middle_click = 0;
    }
    if ((mouse_bytes[0] & 0x02) && mouse_pos.right_click == 1) {
      mouse_pos.right_click = 0;
    }
    if ((mouse_bytes[0] & 0x01) && mouse_pos.left_click == 1){
      mouse_pos.left_click = 0;
    }

    incX = mouse_bytes[1] % mouse_sensibility;
    incY = mouse_bytes[2] % mouse_sensibility;
    erase_mouse();
    mouse_pos.last_x = mouse_pos.actual_x;
    mouse_pos.last_y = mouse_pos.actual_y;
    if(mouse_bytes[0] & 0x10){
      incX = mouse_pos.actual_x - incX;
    } else {
      incX = mouse_pos.actual_x + incX;
    }
    if(mouse_bytes[0] & 0x20){
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
