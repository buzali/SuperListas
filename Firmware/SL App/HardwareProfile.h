#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

#include "Compiler.h"
//#include "/Users/Tofi/Desktop/Firmware/HWProfile.h"


// Set configuration fuses (but only in MainF.c where THIS_IS_STACK_APPLICATION is defined)
#if defined(THIS_IS_STACK_APPLICATION)
_CONFIG2(FNOSC_PRI & POSCMOD_HS & IOL1WAY_OFF)// Primary HS OSC
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF & ICS_PGx2)		// JTAG off, watchdog timer off

#endif


// Clock frequency values

#if defined (BETA_1)
#define MAXIMUM_PIC_FREQ		(9830400ul)
#elif defined (BETA_2)
#define MAXIMUM_PIC_FREQ		(32000000ul)
#endif


// These directly influence timed events using the Tick module.  They also are used for UART and SPI baud rate generation.
#define GetSystemClock()		(MAXIMUM_PIC_FREQ)			// Hz
#define GetInstructionClock()	(GetSystemClock()/2)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Might need changing if using Doze modes.
#define GetPeripheralClock()	(GetSystemClock()/2)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Divisor may be different if using a PIC32 since it's configurable.
#define FCY                     GetInstructionClock()

// Peripheral Pin Select pin output registers
#define RP0_O   RPOR0bits.RP0R
#define RP1_O   RPOR0bits.RP1R
#define RP2_O   RPOR1bits.RP2R
#define RP3_O   RPOR1bits.RP3R
#define RP4_O   RPOR2bits.RP4R

#define RP6_O   RPOR3bits.RP6R
#define RP7_O   RPOR3bits.RP7R
#define RP8_O   RPOR4bits.RP8R
#define RP9_O   RPOR4bits.RP9R
#define RP10_O  RPOR5bits.RP10R
#define RP11_O  RPOR5bits.RP11R
#define RP12_O  RPOR6bits.RP12R
#define RP13_O  RPOR6bits.RP13R
#define RP14_O  RPOR7bits.RP14R

#define RP16_O  RPOR8bits.RP16R
#define RP17_O  RPOR8bits.RP17R
#define RP18_O  RPOR9bits.RP18R
#define RP19_O  RPOR9bits.RP19R
#define RP20_O  RPOR10bits.RP20R
#define RP21_O  RPOR10bits.RP21R
#define RP22_O  RPOR11bits.RP22R
#define RP23_O  RPOR11bits.RP23R
#define RP24_O  RPOR12bits.RP24R
#define RP25_O  RPOR12bits.RP25R
#define RP26_O  RPOR13bits.RP26R
#define RP27_O  RPOR13bits.RP27R
#define RP28_O  RPOR14bits.RP28R
#define RP29_O  RPOR14bits.RP29R
#define RP30_O  RPOR15bits.RP30R


//these are shortcut names for the LED and IO pins
#define EX6_TRIS                (TRISEbits.TRISE5)
#define EX6_I                   (PORTEbits.RE5)
#define EX6_O                   (LATEbits.LATE5)
#define EX5_TRIS                (TRISEbits.TRISE4)
#define EX5_I                   (PORTEbits.RE4)
#define EX5_O                   (LATEbits.LATE4)
#define EX4_TRIS                (TRISEbits.TRISE3)
#define EX4_I                   (PORTEbits.RE3)
#define EX4_O                   (LATEbits.LATE3)
#define EX3_TRIS                (TRISEbits.TRISE2)
#define EX3_I                   (PORTEbits.RE2)
#define EX3_O                   (TRISEbits.TRISE2)
#define EX2_TRIS                (LATEbits.LATE1)
#define EX2_I                   (PORTEbits.RE1)
#define EX2_O                   (LATEbits.LATE1)
#define EX1_TRIS                (TRISEbits.TRISE0)
#define EX1_I                   (PORTEbits.RE0)
#define EX1_O                   (LATEbits.LATE0)

#define OUT4_TRIS               (TRISBbits.TRISB9)
#define OUT4_I                  (PORTBbits.RB9)
#define OUT4_O                  (LATBbits.LATB9)
#define OUT5_TRIS               (TRISBbits.TRISB10)
#define OUT5_I                  (PORTBbits.RB10)
#define OUT5_O                  (LATBbits.LATB10)
#define WIFI_LED_TRIS           (TRISBbits.TRISB9)
#define WIFI_LED_I              (PORTBbits.RB9)
#define WIFI_LED_O              (LATBbits.LATB9)
#define DATA_LED_TRIS           (TRISBbits.TRISB10)
#define DATA_LED_I              (PORTBbits.RB10)
#define DATA_LED_O              (LATBbits.LATB10)

#define USB_CTS_TRIS            (TRISDbits.TRISD4)
#define USB_CTS_I               (PORTDbits.RD4)
#define USB_CTS_O               (LATDbits.LATD4)
#define USB_RTS_TRIS            (TRISDbits.TRISD3)
#define USB_RTS_I               (PORTDbits.RD3)
#define USB_RTS_O               (LATDbits.LATD3)
#define USB_TX_TRIS             (TRISDbits.TRISD1)
#define USB_TX_I                (PORTDbits.RD1)
#define USB_TX_O                (LATDbits.LATD1)
#define USB_RX_TRIS             (TRISDbits.TRISD2)
#define USB_RX_I                (PORTDbits.RD2)
#define USB_RX_O                (LATDbits.LATD2)
//Invertido
#define TX_TRIS                 (TRISDbits.TRISD2)
#define TX_I                    (PORTDbits.RD2)
#define TX_O                    (LATDbits.LATD2)
#define TX_RP                   RP23_O
#define RX_TRIS                 (TRISDbits.TRISD1)
#define RX_I                    (PORTDbits.RD1)
#define RX_O                    (LATDbits.LATD1)
#define RX_RPPIN                24


#define CB_TX_TRIS              (TRISDbits.TRISD11)
#define CB_TX_I                 (PORTDbits.RD11)
#define CB_TX_O                 (LATDbits.LATD11)
#define CB_TX_RP                RP12_O
#define CB_RX_TRIS              (TRISDbits.TRISD10)
#define CB_RX_I                 (PORTDbits.RD10)
#define CB_RX_O                 (LATDbits.LATD10)
#define CB_RX_RP                RP3_O
#define CB_RX_RPPIN             3
#define CB_RTS_TRIS             (TRISDbits.TRISD9)
#define CB_RTS_I                (PORTDbits.RD9)
#define CB_RTS_O                (LATDbits.LATD9)
#define CB_CTS_TRIS             (TRISDbits.TRISD8)
#define CB_CTS_I                (PORTDbits.RD8)
#define CB_CTS_O                (LATDbits.LATD8)


#define LCD_A0_TRIS             (TRISGbits.TRISG2)
#define LCD_A0_I                (PORTGbits.RG2)
#define LCD_A0_O                (LATGbits.LATG2)
#define LCD_CS_TRIS             (TRISGbits.TRISG3)
#define LCD_CS_I                (PORTGbits.RG3)
#define LCD_CS_O                (LATGbits.LATG3)


#if defined (BETA_1)

#define LCD_SO_TRIS             (TRISFbits.TRISF3) //RP16
#define LCD_SO_I                (PORTFbits.RF3)
#define LCD_SO_O                (LATFbits.LATF3)
#define LCD_SO_RP               RP16_O
#define LCD_SCL_TRIS            (TRISFbits.TRISF2) //RP30
#define LCD_SCL_I               (PORTFbits.RF2)
#define LCD_SCL_O               (LATFbits.LATF2)
#define LCD_SCL_RP              RP30_O

#elif defined (BETA_2)
//Para beta 2
#define LCD_SO_TRIS             (TRISFbits.TRISF2) //RP30
#define LCD_SO_I                (PORTFbits.RF2)
#define LCD_SO_O                (LATFbits.LATF2)
#define LCD_SO_RP               RP30_O
#define LCD_SCL_TRIS            (TRISFbits.TRISF3) //RP16
#define LCD_SCL_I               (PORTFbits.RF3)
#define LCD_SCL_O               (LATFbits.LATF3)
#define LCD_SCL_RP              RP16_O

#endif




#define LCD_SPI_IF              (IFS2bits.SPI2IF)
#define LCD_SSPBUF              (SPI2BUF)
#define LCD_SPICON1             (SPI2CON1)
#define LCD_SPICON1bits         (SPI2CON1bits)
#define LCD_SPICON2             (SPI2CON2)
#define LCD_SPISTAT             (SPI2STAT)
#define LCD_SPISTATbits         (SPI2STATbits)
//#define LCD_SPI_ON_BIT  (LCD_SPICON1bits.SSEN)
//#define WaitForDataByte()   do{while(!LCD_SPI_IF); LCD_SPI_IF = 0;}while(0)


#define ENC_SW_TRIS             (TRISBbits.TRISB11)
#define ENC_SW_I                (PORTBbits.RB11)
#define ENC_SW_O                (LATBbits.LATB11)


#if defined (BETA_1)
//Para Beta1
#define ENC_A_TRIS              (TRISBbits.TRISB0) //RP0
#define ENC_A_I                 (PORTBbits.RB0)
#define ENC_A_O                 (LATBbits.LATB0)
#define ENC_A_RP                0
#define ENC_A_INT_DEF           (_INT1Interrupt)
#define ENC_A_IE                (IEC1bits.INT1IE)
#define ENC_A_IF                (IFS1bits.INT1IF)

#elif defined (BETA_2)
//Para beta2
#define ENC_A_TRIS              (TRISFbits.TRISF6) //RP0
#define ENC_A_I                 (PORTFbits.RF6)
#define ENC_A_O                 (LATFbits.LATF6)
#define ENC_A_RP                45
#define ENC_A_INT_DEF           (_INT0Interrupt)
#define ENC_A_IE                (IEC0bits.INT0IE)
#define ENC_A_IF                (IFS0bits.INT0IF)

#endif

#define ENC_B_TRIS              (TRISBbits.TRISB8) //RP8
#define ENC_B_I                 (PORTBbits.RB8)
#define ENC_B_O                 (LATBbits.LATB8)
#define ENC_B_RP                8


// Hardware I/O pin mappings

// LEDs
#define LED0_TRIS			(TRISBbits.TRISB9)
#define LED0_IO				(LATBbits.LATB9)
#define LED1_TRIS			(TRISBbits.TRISB10)	// Ref D4
#define LED1_IO				(LATBbits.LATB10)
#define LED2_TRIS			(TRISBbits.TRISB10)
#define LED2_IO				(LATBbits.LATB10)
#define LED3_TRIS			(TRISBbits.TRISB10)
#define LED3_IO				(LATBbits.LATB10)
#define LED4_TRIS			(TRISBbits.TRISB10)
#define LED4_IO				(LATBbits.LATB10)
#define LED5_TRIS			(TRISBbits.TRISB10)
#define LED5_IO				(LATBbits.LATB10)
#define LED6_TRIS			(TRISBbits.TRISB10)
#define LED6_IO				(LATBbits.LATB10)
#define LED7_TRIS			(TRISBbits.TRISB10)
#define LED7_IO				(LATBbits.LATB10)
#define LED_GET()			((LATBbits.LATB9<<0) | (LATBbits.LATB10<<1))
#define LED_PUT(a)			do{unsigned char vTemp = (a); LED0_IO = vTemp&0x1; LED2_IO = vTemp&0x2; LED3_IO = vTemp&0x4;} while(0)

// Momentary push buttons
//#define BUTTON0_TRIS		(ENC_SW_TRIS)
//#define	BUTTON0_IO		(ENC_SW_IO)
#define BUTTON0_TRIS		(TRISBbits.TRISB11)
#define	BUTTON0_IO		(PORTBbits.RB11)

#define BUTTON1_TRIS		(((unsigned char*)&NVMKEY)[1])
#define	BUTTON1_IO			(((unsigned char*)&NVMKEY)[1])
#define BUTTON2_TRIS		(((unsigned char*)&NVMKEY)[1])
#define	BUTTON2_IO			(((unsigned char*)&NVMKEY)[1])
#define BUTTON3_TRIS		(((unsigned char*)&NVMKEY)[1])
#define	BUTTON3_IO			(((unsigned char*)&NVMKEY)[1])

        //FLYPICUS
//#define UARTTX_TRIS			(TRISDbits.TRISD5)
//#define UARTTX_IO			(PORTDbits.RD5)
//#define UARTRX_TRIS			(TRISDbits.TRISD1)
//#define UARTRX_IO			(PORTDbits.RD1)

        //SUPERLISTAS
#define UARTTX_TRIS			(TRISDbits.TRISD2)
#define UARTTX_IO			(PORTDbits.RD2)
#define UARTRX_TRIS			(TRISDbits.TRISD1)
#define UARTRX_IO			(PORTDbits.RD1)


// // 25LC256 I/O pins
// #if defined(__PIC24FJ256GB110__)
// 	// PIC24FJ256GB110 USB PIM has RD12 pin on Explorer 16 schematic
// 	// remapped and actually connected to PIC24FJ256GB110 pin 90 (RG0).
// 	#define EEPROM_CS_TRIS		(TRISGbits.TRISG0)
// 	#define EEPROM_CS_IO		(LATGbits.LATG0)
// #elif defined(__PIC24FJ256GB210__)
// 	// PIC24FJ256GB210 USB PIM has RD12 pin on Explorer 16 schematic
// 	// remapped and actually connected to PIC24FJ256GB210 pin 90 (RG0) when
// 	// JP1 on PIM has pins 1-2 shorted (USB).  When JP1 pins 2-3 are shorted
// 	// (PMP), PIC pin 90 does connect to RD12.  To make the PIM work with
// 	// either jumper setting, we will drive both RG0 and RD12 simultaneously
// 	// as chip select to the same states.  For an actual application, you'd
// 	// want to specify only the single necessary pin as this double
// 	// assignment operation generates inefficient code by the C compiler.
// 	#define EEPROM_CS_TRIS		TRISGbits.TRISG0 = TRISDbits.TRISD12
// 	#define EEPROM_CS_IO		LATGbits.LATG0 = LATDbits.LATD12
// #else
// 	#define EEPROM_CS_TRIS		(TRISDbits.TRISD12)
// 	#define EEPROM_CS_IO		(LATDbits.LATD12)
// #endif
// #define EEPROM_SCK_TRIS		(TRISGbits.TRISG6)
// #define EEPROM_SDI_TRIS		(TRISGbits.TRISG7)
// #define EEPROM_SDO_TRIS		(TRISGbits.TRISG8)
// #define EEPROM_SPI_IF		(IFS2bits.SPI2IF)
// #define EEPROM_SSPBUF		(SPI2BUF)
// #define EEPROM_SPICON1		(SPI2CON1)
// #define EEPROM_SPICON1bits	(SPI2CON1bits)
// #define EEPROM_SPICON2		(SPI2CON2)
// #define EEPROM_SPISTAT		(SPI2STAT)
// #define EEPROM_SPISTATbits	(SPI2STATbits)

// // LCD Module I/O pins.  NOTE: On the Explorer 16, the LCD is wired to the
// // same PMP lines required to communicate with an ENCX24J600 in parallel
// // mode.  Since the LCD does not have a chip select wire, if you are using
// // the ENC424J600/624J600 in parallel mode, the LCD cannot be used.
// #if !defined(ENC100_INTERFACE_MODE) || (ENC100_INTERFACE_MODE == 0)	// SPI only
// 	#define LCD_DATA_TRIS		(*((volatile unsigned char*)&TRISE))
// 	#define LCD_DATA_IO			(*((volatile unsigned char*)&LATE))
// 	#define LCD_RD_WR_TRIS		(TRISDbits.TRISD5)
// 	#define LCD_RD_WR_IO		(LATDbits.LATD5)
// 	#define LCD_RS_TRIS			(TRISBbits.TRISB15)
// 	#define LCD_RS_IO			(LATBbits.LATB15)
// 	#define LCD_E_TRIS			(TRISDbits.TRISD4)
// 	#define LCD_E_IO			(LATDbits.LATD4)
// #endif



//----------------------------
// MRF24WB0M WiFi I/O pins
//----------------------------


// PIC24FJ256GA110 PIM on Explorer 16 must use SPI2, not SPI1
#define MRF24WB0M_IN_SPI1

#define WF_CS_TRIS			(TRISGbits.TRISG9)
#define WF_CS_IO			(LATGbits.LATG9)
#define WF_SDI_TRIS			(TRISGbits.TRISG7)
#define WF_SDI_RPPIN                    19
#define WF_SCK_TRIS			(TRISGbits.TRISG6)
#define WF_SCK_RP                       RP21_O
#define WF_SDO_TRIS			(TRISGbits.TRISG8)
#define WF_SDO_RP                       RP26_O
#define WF_RESET_TRIS                   (TRISEbits.TRISE7)
#define WF_RESET_IO			(LATEbits.LATE7)
#define WF_INT_TRIS                     (TRISBbits.TRISB2)  // INT1
#define WF_INT_IO			(PORTBbits.RB2)
#define WF_INT_RPPIN                    13
#define WF_HIBERNATE_TRIS               (TRISEbits.TRISE6)
#define	WF_HIBERNATE_IO                 (PORTEbits.RE6)
#define WF_INT_EDGE                     (INTCON2bits.INT1EP)
#define WF_INT_IE			(IEC1bits.INT1IE)
#define WF_INT_IF			(IFS1bits.INT1IF)

#define WF_SSPBUF			(SPI1BUF)
#define WF_SPISTAT			(SPI1STAT)
#define WF_SPISTATbits                  (SPI1STATbits)
#define WF_SPICON1			(SPI1CON1)
#define WF_SPICON1bits                  (SPI1CON1bits)
#define WF_SPICON2			(SPI1CON2)
#define WF_SPI_IE			(IEC0bits.SPI1IE)
//#define WF_SPI_IP			(IPC2bits.SPI1IP)
#define WF_SPI_IF			(IFS0bits.SPI1IF)


// Select which UART the STACK_USE_UART and STACK_USE_UART2TCP_BRIDGE
// options will use.  You can change these to U1BRG, U1MODE, etc. if you
// want to use the UART1 module instead of UART2.
#define UBRG				U2BRG
#define UMODE				U2MODE
#define USTA				U2STA
#define BusyUART()			BusyUART2()
#define CloseUART()			CloseUART2()
#define ConfigIntUART(a)	ConfigIntUART2(a)
#define DataRdyUART()		DataRdyUART2()
#define OpenUART(a,b,c)		OpenUART2(a,b,c)
#define ReadUART()			ReadUART2()
#define WriteUART(a)		WriteUART2(a)
#define getsUART(a,b,c)		getsUART2(a,b,c)
#define putsUART(a)			putsUART2((unsigned int*)a)
#define getcUART()			getcUART2()
#define putcUART(a)			do{while(BusyUART()); WriteUART(a); while(BusyUART()); }while(0)
#define putrsUART(a)		putrsUART2(a)










#endif // #ifndef HARDWARE_PROFILE_H

