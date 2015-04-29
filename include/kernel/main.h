#ifndef _main_
#define _main_

void kmain(multiboot_info*, int);
void k_set_loading_screen(void);
int k_isLoading(void);
void k_LoadingScreenEffect();
void k_loading_log(char*);
void k_SetUpDrivers(void);
void k_turnOn(void);
void k_turnOff(void);
int k_isOn(void);

#endif
