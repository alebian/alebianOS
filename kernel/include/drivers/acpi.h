/* Based on code written by kaworu */
/* http://forum.osdev.org/viewtopic.php?t=16990 */

#ifndef _ACPI_
#define _ACPI_

#include "../../../common.h"

struct RSDPtr
{
   uint8_t Signature[8];
   uint8_t CheckSum;
   uint8_t OemID[6];
   uint8_t Revision;
   sint32_t *RsdtAddress;
};

struct FACP
{
   uint8_t Signature[4];
   sint32_t Length;
   uint8_t unneded1[40 - 8];
   sint32_t *DSDT;
   uint8_t unneded2[48 - 44];
   sint32_t *SMI_CMD;
   uint8_t ACPI_ENABLE;
   uint8_t ACPI_DISABLE;
   uint8_t unneded3[64 - 54];
   sint32_t *PM1a_CNT_BLK;
   sint32_t *PM1b_CNT_BLK;
   uint8_t unneded4[89 - 72];
   uint8_t PM1_CNT_LEN;
};

unsigned int *acpiCheckRSDPtr(unsigned int*);
unsigned int *acpiGetRSDPtr(void);
int acpiCheckHeader(unsigned int*, char*);
int acpiEnable(void);
int init_ACPI(void);
void acpiPowerOff(void);
sint8_t ACPIinitialized();

#endif