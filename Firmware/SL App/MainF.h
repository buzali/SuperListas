/*********************************************************************
 *
 *                  Headers for TCPIP  App
 */

#ifndef _MainF_H
#define _MainF_H

#define BAUD_RATE       (19200)		// bps

#if !defined(THIS_IS_STACK_APPLICATION)
	extern BYTE AN0String[8];
#endif

void DoUARTConfig(void);

#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
	void SaveAppConfig(const APP_CONFIG *AppConfig);
#else
	#define SaveAppConfig(a)
#endif

void TCPClient(void);


// Define a header structure for validating the AppConfig data structure in EEPROM/Flash
typedef struct
{
	unsigned short wConfigurationLength;	// Number of bytes saved in EEPROM/Flash (sizeof(APP_CONFIG))
	unsigned short wOriginalChecksum;		// Checksum of the original AppConfig defaults as loaded from ROM (to detect when to wipe the EEPROM/Flash record of AppConfig due to a stack change, such as when switching from Ethernet to Wi-Fi)
	unsigned short wCurrentChecksum;		// Checksum of the current EEPROM/Flash data.  This protects against using corrupt values if power failure occurs while writing them and helps detect coding errors in which some other task writes to the EEPROM in the AppConfig area.
} NVM_VALIDATION_STRUCT;


// An actual function defined in MainF.c for displaying the current IP 
// address on the UART and/or LCD.
void DisplayIPValue(IP_ADDR IPVal);


#endif // _MainF_H
