#ifndef _login_
#define _login_

#define USERNAME_MAX_SIZE 32
#define PASSWORD_MAX_SIZE 32

void login(void);
void login_set_screen(void);
void login_restart_buffers(void);
void login_keyboardGetUserListener(void);
void login_keyboardGetPassListener(void);
void login_lclickListener(int, int);
void login_rclickListener(int, int);
void login_mclickListener(int, int);
int login_check(void);
void login_guest(void);
int login_isLogued(void);
void login_in(void);
void login_out(void);

#endif