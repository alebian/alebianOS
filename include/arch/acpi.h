#ifndef _ACPI_
#define _ACPI_

struct RSDPtr
{
   byte Signature[8];
   byte CheckSum;
   byte OemID[6];
   byte Revision;
   dword *RsdtAddress;
};

struct FACP
{
   byte Signature[4];
   dword Length;
   byte unneded1[40 - 8];
   dword *DSDT;
   byte unneded2[48 - 44];
   dword *SMI_CMD;
   byte ACPI_ENABLE;
   byte ACPI_DISABLE;
   byte unneded3[64 - 54];
   dword *PM1a_CNT_BLK;
   dword *PM1b_CNT_BLK;
   byte unneded4[89 - 72];
   byte PM1_CNT_LEN;
};

unsigned int *acpiCheckRSDPtr(unsigned int*);
unsigned int *acpiGetRSDPtr(void);
int acpiCheckHeader(unsigned int*, char*);
int acpiEnable(void);
int initAcpi(void);
void acpiPowerOff(void);

#endif