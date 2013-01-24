/*********************************************************************
 *
 *  Main Application Entry Point based on Microchip TCP/IP DemoAPP 
 */

#define THIS_IS_STACK_APPLICATION

// Include all headers for any enabled TCPIP Stack functions
#include "TCPIP Stack/TCPIP.h"

// Include functions specific to this stack application
#include "MainF.h"
#include <PPS.h>
#include "LCD.h"


// Used for Wi-Fi assertions
#define WF_MODULE_NUMBER   WF_MODULE_MAIN_DEMO

// Declare AppConfig structure and some other supporting stack variables
APP_CONFIG AppConfig;
static unsigned short wOriginalAppConfigChecksum;	// Checksum of the ROM defaults for AppConfig
BYTE AN0String[8];

// Use UART2 instead of UART1 for stdout (printf functions).  Explorer 16 
// serial port hardware is on PIC UART2 module.
#if defined(EXPLORER_16) || defined(PIC24FJ256DA210_DEV_BOARD)
	int __C30_UART = 2;
#endif


// Private helper functions.
// These may or may not be present in all applications.
static void InitAppConfig(void);
static void InitializeBoard(void);
static void ProcessIO(void);
#if defined(WF_CS_TRIS)
    static void WF_Connect(void);
#endif



int main(void)

{
	static DWORD t = 0;
	static DWORD dwLastIP = 0;
	#if defined(WF_USE_POWER_SAVE_FUNCTIONS)
	BOOL  PsPollEnabled;
	BOOL  psConfDone = FALSE;
	#endif

	// Initialize application specific hardware
        
	InitializeBoard();
        

	// Initialize stack-related hardware components that may be 
	// required by the UART configuration routines
        TickInit();
	#if defined(STACK_USE_MPFS2)
	MPFSInit();
	#endif

	// Initialize Stack and application related NV variables into AppConfig.
	InitAppConfig();



    // Initiates board setup process if button is depressed 
	// on startup
    if(BUTTON0_IO == 0)
    {
		#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
		// Invalidate the EEPROM contents if BUTTON0 is held down for more than 4 seconds
		DWORD StartTime = TickGet();
		LED_PUT(0x00);
				
		while(BUTTON0_IO == 0u)
		{
			if(TickGet() - StartTime > 4*TICK_SECOND)
			{
				#if defined(EEPROM_CS_TRIS)
			    XEEBeginWrite(0x0000);
			    XEEWrite(0xFF);
			    XEEWrite(0xFF);
			    XEEEndWrite();
			    #elif defined(SPIFLASH_CS_TRIS)
			    SPIFlashBeginWrite(0x0000);
			    SPIFlashWrite(0xFF);
			    SPIFlashWrite(0xFF);
			    #endif
			    
				#if defined(STACK_USE_UART)
				putrsUART("\r\n\r\nBUTTON0 held for more than 4 seconds.  Default settings restored.\r\n\r\n");
				#endif

				LED_PUT(0x0F);
				while((LONG)(TickGet() - StartTime) <= (LONG)(9*TICK_SECOND/2));
				LED_PUT(0x00);
				while(BUTTON0_IO == 0u);
				Reset();
				break;
			}
		}
		#endif

		#if defined(STACK_USE_UART)
                DoUARTConfig();
		#endif
    }

	// Initialize core stack layers (MAC, ARP, TCP, UDP) and
	// application modules (HTTP, SNMP, etc.)
    
        LCD_init();
        LCD_printLogo();
    StackInit();

    #if defined(WF_CS_TRIS)
    WF_Connect();
    #endif


    while(1)
    {
	#if defined(WF_USE_POWER_SAVE_FUNCTIONS)
		if (!psConfDone && WFisConnected()) {	
			PsPollEnabled = (MY_DEFAULT_PS_POLL == WF_ENABLED);
			if (!PsPollEnabled) {	 
				/* disable low power (PS-Poll) mode */
				#if defined(STACK_USE_UART)
				putrsUART("Disable PS-Poll\r\n");		 
				#endif
				WF_PsPollDisable();
			} else {
				/* Enable low power (PS-Poll) mode */
				#if defined(STACK_USE_UART)
				putrsUART("Enable PS-Poll\r\n");		
				#endif
				WF_PsPollEnable(TRUE);
			}	
			psConfDone = TRUE;
		}
	#endif
        // Blink LED0 (right most one) every second.
        if(TickGet() - t >= TICK_SECOND/2ul)
        {
            t = TickGet();
            LED0_IO ^= 1;
        }

        // This task performs normal stack task including checking
        // for incoming packet, type of packet and calling
        // appropriate stack entity to process it.
        StackTask();

        // This tasks invokes each of the core stack application tasks
        StackApplications();

 	ProcessIO();

        // If the local IP address has changed (ex: due to DHCP lease change)
        // write the new IP address to the LCD display, UART, and Announce 
        // service
		if(dwLastIP != AppConfig.MyIPAddr.Val)
		{
			dwLastIP = AppConfig.MyIPAddr.Val;
			
			#if defined(STACK_USE_UART)
				putrsUART((ROM char*)"\r\nNew IP Address: ");
			#endif

			DisplayIPValue(AppConfig.MyIPAddr);

			#if defined(STACK_USE_UART)
				putrsUART((ROM char*)"\r\n");
			#endif


			#if defined(STACK_USE_ANNOUNCE)
				AnnounceIP();
			#endif

            #if defined(STACK_USE_ZEROCONF_MDNS_SD)
				mDNSFillHostRecord();
			#endif
		}

        TCPClient();
	}
}

#if defined(WF_CS_TRIS)
/*****************************************************************************
 * FUNCTION: WF_Connect
 *netw
 * RETURNS:  None
 *
 * PARAMS:   None
 *
 *  NOTES:   Connects to an 802.11 network.  Customize this function as needed 
 *           for your application.
 *****************************************************************************/
static void WF_Connect(void)
{
    UINT8 ConnectionProfileID;
    UINT8 channelList[] = MY_DEFAULT_CHANNEL_LIST;
    
    /* create a Connection Profile */
    WF_CPCreate(&ConnectionProfileID);

    #if defined(STACK_USE_UART)
    putrsUART("Set SSID (");
    putsUART(AppConfig.MySSID);
    putrsUART(")\r\n");
    #endif
    WF_CPSetSsid(ConnectionProfileID, 
                 AppConfig.MySSID, 
                 AppConfig.SsidLength);

    #if defined(STACK_USE_UART)
    putrsUART("Set Network Type\r\n");
	#endif
    WF_CPSetNetworkType(ConnectionProfileID, MY_DEFAULT_NETWORK_TYPE);
    
	#if defined(STACK_USE_UART)
	putrsUART("Set Scan Type\r\n");
	#endif
    WF_CASetScanType(MY_DEFAULT_SCAN_TYPE);
    
    #if defined(STACK_USE_UART)
    putrsUART("Set Channel List\r\n");
    #endif    
    WF_CASetChannelList(channelList, sizeof(channelList));
    
    #if defined(STACK_USE_UART)
    putrsUART("Set list retry count\r\n");
    #endif
    // The Retry Count parameter tells the WiFi Connection manager how many attempts to make when trying
    // to connect to an existing network.  In the Infrastructure case, the default is to retry forever so that
    // if the AP is turned off or out of range, the radio will continue to attempt a connection until the
    // AP is eventually back on or in range.  In the Adhoc case, the default is to retry 3 times since the 
    // purpose of attempting to establish a network in the Adhoc case is only to verify that one does not
    // initially exist.  If the retry count was set to WF_RETRY_FOREVER in the AdHoc mode, an AdHoc network
    // would never be established.  The constants MY_DEFAULT_LIST_RETRY_COUNT_ADHOC and 
    // MY_DEFAULT_LIST_RETRY_COUNT_INFRASTRUCTURE have been created specifically for the June 2011 MAL release.
    #if defined(EZ_CONFIG_STORE)
        if (AppConfig.networkType == WF_ADHOC)
            WF_CASetListRetryCount(MY_DEFAULT_LIST_RETRY_COUNT_ADHOC);
        else
            WF_CASetListRetryCount(MY_DEFAULT_LIST_RETRY_COUNT_INFRASTRUCTURE);
    #else
        #if (MY_DEFAULT_NETWORK_TYPE == WF_ADHOC)
            WF_CASetListRetryCount(MY_DEFAULT_LIST_RETRY_COUNT_ADHOC);
        #else
            WF_CASetListRetryCount(MY_DEFAULT_LIST_RETRY_COUNT_INFRASTRUCTURE);
        #endif
    #endif
    

    #if defined(STACK_USE_UART)        
    putrsUART("Set Event Notify\r\n");    
    #endif
    WF_CASetEventNotificationAction(MY_DEFAULT_EVENT_NOTIFICATION_LIST);
    
    #if defined(STACK_USE_UART)
    putrsUART("Set Beacon Timeout\r\n");
    #endif
    WF_CASetBeaconTimeout(40);
    
    /* Set Security */
    #if (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_OPEN)
        #if defined(STACK_USE_UART)
        putrsUART("Set Security (Open)\r\n");
        #endif
    #elif (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WEP_40)
        #if defined(STACK_USE_UART)
        putrsUART("Set Security (WEP40)\r\n");
        #endif
    #elif (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WEP_104)
        #if defined(STACK_USE_UART)
        putrsUART("Set Security (WEP104)\r\n");
        #endif
    #elif MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA_WITH_KEY 
        #if defined(STACK_USE_UART)
        putrsUART("Set Security (WPA with key)\r\n");
        #endif
    #elif MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA2_WITH_KEY 
        #if defined(STACK_USE_UART)
        putrsUART("Set Security (WPA2 with key)\r\n");
        #endif
    #elif MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA_WITH_PASS_PHRASE
        #if defined(STACK_USE_UART)
        putrsUART("Set Security (WPA with pass phrase)\r\n");
        #endif
    #elif MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA2_WITH_PASS_PHRASE
        #if defined(STACK_USE_UART)
        putrsUART("Set Security (WPA2 with pass phrase)\r\n");    
        #endif
    #elif MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA_AUTO_WITH_KEY
        #if defined(STACK_USE_UART)
        putrsUART("Set Security (WPA with key, auto-select)\r\n");
        #endif
    #elif MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE
        #if defined(STACK_USE_UART)
        putrsUART("Set Security (WPA with pass phrase, auto-select)\r\n");
        #endif
    #endif /* MY_DEFAULT_WIFI_SECURITY_MODE */

    WF_CPSetSecurity(ConnectionProfileID,
                     AppConfig.SecurityMode,
                     AppConfig.WepKeyIndex,   /* only used if WEP enabled */
                     AppConfig.SecurityKey,
                     AppConfig.SecurityKeyLength);
    #if defined(STACK_USE_UART)                     
    putrsUART("Start WiFi Connect\r\n");        
    #endif
    WF_CMConnect(ConnectionProfileID);
}   
#endif /* WF_CS_TRIS */

// Writes an IP address to the LCD display and the UART as available
void DisplayIPValue(IP_ADDR IPVal)
{
//	printf("%u.%u.%u.%u", IPVal.v[0], IPVal.v[1], IPVal.v[2], IPVal.v[3]);
#if defined (__dsPIC33E__) || defined (__PIC24E__)
	static BYTE IPDigit[4];    					/* Needs to be declared as static to avoid the array getting optimized by C30 v3.30 compiler for dsPIC33E/PIC24E. 
												   Otherwise the LCD displays corrupted IP address on Explorer 16. To be fixed in the future compiler release*/
#else
    BYTE IPDigit[4];
#endif
	BYTE i;
#ifdef USE_LCD
	BYTE j;
	BYTE LCDPos=16;
#endif

	for(i = 0; i < sizeof(IP_ADDR); i++)
	{
	    uitoa((WORD)IPVal.v[i], IPDigit);

		#if defined(STACK_USE_UART)
			putsUART((char *) IPDigit);
		#endif

		#ifdef USE_LCD
			for(j = 0; j < strlen((char*)IPDigit); j++)
			{
				LCDText[LCDPos++] = IPDigit[j];
			}
			if(i == sizeof(IP_ADDR)-1)
				break;
			LCDText[LCDPos++] = '.';
		#else
			if(i == sizeof(IP_ADDR)-1)
				break;
		#endif

		#if defined(STACK_USE_UART)
			while(BusyUART());
			WriteUART('.');
		#endif
	}

	#ifdef USE_LCD
		if(LCDPos < 32u)
			LCDText[LCDPos] = 0;
		LCDUpdate();
	#endif
}

// Processes A/D data from the potentiometer
static void ProcessIO(void)
{
#if defined(__C30__) || defined(__C32__)
    // Convert potentiometer result into ASCII string
    uitoa((WORD)ADC1BUF0, AN0String);
#else
    // AN0 should already be set up as an analog input
    ADCON0bits.GO = 1;

    // Wait until A/D conversion is done
    while(ADCON0bits.GO);

	// AD converter errata work around (ex: PIC18F87J10 A2)
	#if !defined(__18F87J50) && !defined(_18F87J50) && !defined(__18F87J11) && !defined(_18F87J11) 
	{
		BYTE temp = ADCON2;
		ADCON2 |= 0x7;	// Select Frc mode by setting ADCS0/ADCS1/ADCS2
		ADCON2 = temp;
	}
	#endif

    // Convert 10-bit value into ASCII string
    uitoa(*((WORD*)(&ADRESL)), AN0String);
#endif
}


/****************************************************************************
  Function:
    static void InitializeBoard(void)

  Description:
    This routine initializes the hardware.  It is a generic initialization
    routine for many of the Microchip development boards, using definitions
    in HardwareProfile.h to determine specific initialization.

  Precondition:
    None

  Parameters:
    None - None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
static void InitializeBoard(void)
{

    AD1PCFGL = 0xFFFF; 
	// LEDs
	LED0_TRIS = 0;
	LED1_TRIS = 0;
	LED2_TRIS = 0;
	LED3_TRIS = 0;
	LED4_TRIS = 0;
	LED5_TRIS = 0;
	LED6_TRIS = 0;
	LED7_TRIS = 0;
	LED_PUT(0x03);

        
        BUTTON0_TRIS=1;



        CLKDIVbits.RCDIV = 0;		// Set 1:1 8MHz FRC postscalar

		
		// ADC


	// UART
	#if defined(STACK_USE_UART)

		UARTTX_TRIS = 0;
		UARTRX_TRIS = 1;
		UMODE = 0x8000;			// Set UARTEN.  Note: this must be done before setting UTXEN
                USTA = 0x0400;		// UTXEN set
		#define CLOSEST_UBRG_VALUE ((GetPeripheralClock()+8ul*BAUD_RATE)/16/BAUD_RATE-1)
		#define BAUD_ACTUAL (GetPeripheralClock()/16/(CLOSEST_UBRG_VALUE+1))
		#define BAUD_ERROR ((BAUD_ACTUAL > BAUD_RATE) ? BAUD_ACTUAL-BAUD_RATE : BAUD_RATE-BAUD_ACTUAL)
		#define BAUD_ERROR_PRECENT	((BAUD_ERROR*100+BAUD_RATE/2)/BAUD_RATE)
		#if (BAUD_ERROR_PRECENT > 3)
			#warning UART frequency error is worse than 3%
		#elif (BAUD_ERROR_PRECENT > 2)
			#warning UART frequency error is worse than 2%
		#endif
		UBRG = CLOSEST_UBRG_VALUE;
	#endif


        //init_CB_UART

	// This is an EXAMPLE, so brutal typing goes into explaining all bit sets

	// The Explorer 16 board has a DB9 connector wired to UART2, so we will
	// be configuring this port only

	// configure U2MODE
        U1MODE = 0;

        #define CB_BAUD_RATE   9600
        #define CLOSEST_CB_UBRG_VALUE ((GetPeripheralClock()+8ul*CB_BAUD_RATE)/16/CB_BAUD_RATE-1)
        #define CB_BAUD_ACTUAL (GetPeripheralClock()/16/(CLOSEST_CB_UBRG_VALUE+1))
	#define CB_BAUD_ERROR ((CB_BAUD_ACTUAL > BAUD_RATE) ? CB_BAUD_ACTUAL-CB_BAUD_RATE : CB_BAUD_RATE-CB_BAUD_ACTUAL)
	#define CB_BAUD_ERROR_PRECENT	((CB_BAUD_ERROR*100+CB_BAUD_RATE/2)/CB_BAUD_RATE)
	#if (CB_BAUD_ERROR_PRECENT > 3)
		#warning UART frequency error is worse than 3%
	#elif (CB_BAUD_ERROR_PRECENT > 2)
		#warning UART frequency error is worse than 2%
	#endif
	U1BRG = CLOSEST_CB_UBRG_VALUE;

	// Load all values in for U1STA SFR
        U1STA =0;
	IFS0bits.U1TXIF = 0;	// Clear the Transmit Interrupt Flag
	IEC0bits.U1TXIE = 0;	// Enable Transmit Interrupts
	IFS0bits.U1RXIF = 0;	// Clear the Recieve Interrupt Flag
	IEC0bits.U1RXIE = 1;	// Enable Recieve Interrupts

	U1MODEbits.UARTEN = 1;	// And turn the peripheral on
	U1STAbits.UTXEN = 1;




// Deassert all chip select lines so there isn't any problem with 
// initialization order.  Ex: When ENC28J60 is on SPI2 with Explorer 16, 
// MAX3232 ROUT2 pin will drive RF12/U2CTS ENC28J60 CS line asserted, 
// preventing proper 25LC256 EEPROM operation.

#if defined(WF_CS_TRIS)
	WF_CS_IO = 1;
	WF_CS_TRIS = 0;
#endif


#if defined(__PIC24FJ256GA106__)
/*
	__builtin_write_OSCCONL(OSCCON & 0xBF);

	RPOR10bits.RP21R = 8;	// Assign RG6/RP21 to SCK1 (output)
	RPOR13bits.RP26R = 7;	// Assign RG8/RP19 to SDO1 (output)
	RPOR11bits.RP23R = 5;	// Assign RD1/RP24 to U2TX (output)


	RPINR20bits.SDI1R = 19;	// Assign RG7/RP26 to SDI1 (input)
	RPINR0bits.INT1R = 13;	// Assign RB2/RP13 to INT1 (input)
	RPINR19bits.U2RXR = 24;	// Assign RD2/RP23 to U2RX (input)
	__builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS
*/

    PPSUnLock;

    iPPSOutput(WF_SCK_RP, OUT_FN_PPS_SCK1OUT);
    iPPSOutput(WF_SDO_RP, OUT_FN_PPS_SDO1);
    iPPSOutput(TX_RP, OUT_FN_PPS_U2TX);

    iPPSOutput(CB_TX_RP, OUT_FN_PPS_U1TX);
    iPPSOutput(LCD_SO_RP, OUT_FN_PPS_SDO2);
    iPPSOutput(LCD_SCL_RP, OUT_FN_PPS_SCK2OUT);


    iPPSInput(IN_FN_PPS_U1RX, CB_RX_RPPIN);
    //PPSInput(IN_FN_PPS_INT1, ENC_A_RPPIN);
    //iPPSInput(IN_FN_PPS_INT2, ENC_B_RPPIN);
    iPPSInput(IN_FN_PPS_SDI1, WF_SDI_RPPIN);
    iPPSInput(IN_FN_PPS_INT1, WF_INT_RPPIN);
    iPPSInput(IN_FN_PPS_U2RX, RX_RPPIN);
    PPSLock;
        
#endif

}

/*********************************************************************
 * Function:        void InitAppConfig(void)
 *
 * PreCondition:    MPFSInit() is already called.
 *
 * Input:           None
 *
 * Output:          Write/Read non-volatile config variables.
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
// MAC Address Serialization using a MPLAB PM3 Programmer and 
// Serialized Quick Turn Programming (SQTP). 
// The advantage of using SQTP for programming the MAC Address is it
// allows you to auto-increment the MAC address without recompiling 
// the code for each unit.  To use SQTP, the MAC address must be fixed
// at a specific location in program memory.  Uncomment these two pragmas
// that locate the MAC address at 0x1FFF0.  Syntax below is for MPLAB C 
// Compiler for PIC18 MCUs. Syntax will vary for other compilers.
//#pragma romdata MACROM=0x1FFF0
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1, MY_DEFAULT_MAC_BYTE2, MY_DEFAULT_MAC_BYTE3, MY_DEFAULT_MAC_BYTE4, MY_DEFAULT_MAC_BYTE5, MY_DEFAULT_MAC_BYTE6};
//#pragma romdata

static void InitAppConfig(void)
{
#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
	unsigned char vNeedToSaveDefaults = 0;
#endif
	
	while(1)
	{
		// Start out zeroing all AppConfig bytes to ensure all fields are 
		// deterministic for checksum generation
		memset((void*)&AppConfig, 0x00, sizeof(AppConfig));
		
		AppConfig.Flags.bIsDHCPEnabled = TRUE;
		AppConfig.Flags.bInConfigMode = TRUE;
		memcpypgm2ram((void*)&AppConfig.MyMACAddr, (ROM void*)SerializedMACAddress, sizeof(AppConfig.MyMACAddr));
//		{
//			_prog_addressT MACAddressAddress;
//			MACAddressAddress.next = 0x157F8;
//			_memcpy_p2d24((char*)&AppConfig.MyMACAddr, MACAddressAddress, sizeof(AppConfig.MyMACAddr));
//		}
		AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2<<8ul | MY_DEFAULT_IP_ADDR_BYTE3<<16ul | MY_DEFAULT_IP_ADDR_BYTE4<<24ul;
		AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
		AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2<<8ul | MY_DEFAULT_MASK_BYTE3<<16ul | MY_DEFAULT_MASK_BYTE4<<24ul;
		AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
		AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 | MY_DEFAULT_GATE_BYTE2<<8ul | MY_DEFAULT_GATE_BYTE3<<16ul | MY_DEFAULT_GATE_BYTE4<<24ul;
		AppConfig.PrimaryDNSServer.Val = MY_DEFAULT_PRIMARY_DNS_BYTE1 | MY_DEFAULT_PRIMARY_DNS_BYTE2<<8ul  | MY_DEFAULT_PRIMARY_DNS_BYTE3<<16ul  | MY_DEFAULT_PRIMARY_DNS_BYTE4<<24ul;
		AppConfig.SecondaryDNSServer.Val = MY_DEFAULT_SECONDARY_DNS_BYTE1 | MY_DEFAULT_SECONDARY_DNS_BYTE2<<8ul  | MY_DEFAULT_SECONDARY_DNS_BYTE3<<16ul  | MY_DEFAULT_SECONDARY_DNS_BYTE4<<24ul;
	
	
		// SNMP Community String configuration
		#if defined(STACK_USE_SNMP_SERVER)
		{
			BYTE i;
			static ROM char * ROM cReadCommunities[] = SNMP_READ_COMMUNITIES;
			static ROM char * ROM cWriteCommunities[] = SNMP_WRITE_COMMUNITIES;
			ROM char * strCommunity;
			
			for(i = 0; i < SNMP_MAX_COMMUNITY_SUPPORT; i++)
			{
				// Get a pointer to the next community string
				strCommunity = cReadCommunities[i];
				if(i >= sizeof(cReadCommunities)/sizeof(cReadCommunities[0]))
					strCommunity = "";
	
				// Ensure we don't buffer overflow.  If your code gets stuck here, 
				// it means your SNMP_COMMUNITY_MAX_LEN definition in TCPIPConfig.h 
				// is either too small or one of your community string lengths 
				// (SNMP_READ_COMMUNITIES) are too large.  Fix either.
				if(strlenpgm(strCommunity) >= sizeof(AppConfig.readCommunity[0]))
					while(1);
				
				// Copy string into AppConfig
				strcpypgm2ram((char*)AppConfig.readCommunity[i], strCommunity);
	
				// Get a pointer to the next community string
				strCommunity = cWriteCommunities[i];
				if(i >= sizeof(cWriteCommunities)/sizeof(cWriteCommunities[0]))
					strCommunity = "";
	
				// Ensure we don't buffer overflow.  If your code gets stuck here, 
				// it means your SNMP_COMMUNITY_MAX_LEN definition in TCPIPConfig.h 
				// is either too small or one of your community string lengths 
				// (SNMP_WRITE_COMMUNITIES) are too large.  Fix either.
				if(strlenpgm(strCommunity) >= sizeof(AppConfig.writeCommunity[0]))
					while(1);
	
				// Copy string into AppConfig
				strcpypgm2ram((char*)AppConfig.writeCommunity[i], strCommunity);
			}
		}
		#endif
	
		// Load the default NetBIOS Host Name
		memcpypgm2ram(AppConfig.NetBIOSName, (ROM void*)MY_DEFAULT_HOST_NAME, 16);
		FormatNetBIOSName(AppConfig.NetBIOSName);
	
		#if defined(WF_CS_TRIS)
			// Load the default SSID Name
			WF_ASSERT(sizeof(MY_DEFAULT_SSID_NAME) <= sizeof(AppConfig.MySSID));
			memcpypgm2ram(AppConfig.MySSID, (ROM void*)MY_DEFAULT_SSID_NAME, sizeof(MY_DEFAULT_SSID_NAME));
			AppConfig.SsidLength = sizeof(MY_DEFAULT_SSID_NAME) - 1;
	
	        AppConfig.SecurityMode = MY_DEFAULT_WIFI_SECURITY_MODE;
	        AppConfig.WepKeyIndex  = MY_DEFAULT_WEP_KEY_INDEX;
	        
	        #if (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_OPEN)
	            memset(AppConfig.SecurityKey, 0x00, sizeof(AppConfig.SecurityKey));
	            AppConfig.SecurityKeyLength = 0;
	
	        #elif MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WEP_40
	            memcpypgm2ram(AppConfig.SecurityKey, (ROM void*)MY_DEFAULT_WEP_KEYS_40, sizeof(MY_DEFAULT_WEP_KEYS_40) - 1);
	            AppConfig.SecurityKeyLength = sizeof(MY_DEFAULT_WEP_KEYS_40) - 1;
	
	        #elif MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WEP_104
			    memcpypgm2ram(AppConfig.SecurityKey, (ROM void*)MY_DEFAULT_WEP_KEYS_104, sizeof(MY_DEFAULT_WEP_KEYS_104) - 1);
			    AppConfig.SecurityKeyLength = sizeof(MY_DEFAULT_WEP_KEYS_104) - 1;
	
	        #elif (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA_WITH_KEY)       || \
	              (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA2_WITH_KEY)      || \
	              (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA_AUTO_WITH_KEY)
			    memcpypgm2ram(AppConfig.SecurityKey, (ROM void*)MY_DEFAULT_PSK, sizeof(MY_DEFAULT_PSK) - 1);
			    AppConfig.SecurityKeyLength = sizeof(MY_DEFAULT_PSK) - 1;
	
	        #elif (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA_WITH_PASS_PHRASE)     || \
	              (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA2_WITH_PASS_PHRASE)    || \
	              (MY_DEFAULT_WIFI_SECURITY_MODE == WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE)
	            memcpypgm2ram(AppConfig.SecurityKey, (ROM void*)MY_DEFAULT_PSK_PHRASE, sizeof(MY_DEFAULT_PSK_PHRASE) - 1);
	            AppConfig.SecurityKeyLength = sizeof(MY_DEFAULT_PSK_PHRASE) - 1;
	
	        #else 
	            #error "No security defined"
	        #endif /* MY_DEFAULT_WIFI_SECURITY_MODE */
	
		#endif

		// Compute the checksum of the AppConfig defaults as loaded from ROM
		wOriginalAppConfigChecksum = CalcIPChecksum((BYTE*)&AppConfig, sizeof(AppConfig));

		#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
		{
			NVM_VALIDATION_STRUCT NVMValidationStruct;

			// Check to see if we have a flag set indicating that we need to 
			// save the ROM default AppConfig values.
			if(vNeedToSaveDefaults)
				SaveAppConfig(&AppConfig);
		
			// Read the NVMValidation record and AppConfig struct out of EEPROM/Flash
			#if defined(EEPROM_CS_TRIS)
			{
				XEEReadArray(0x0000, (BYTE*)&NVMValidationStruct, sizeof(NVMValidationStruct));
				XEEReadArray(sizeof(NVMValidationStruct), (BYTE*)&AppConfig, sizeof(AppConfig));
			}
			#elif defined(SPIFLASH_CS_TRIS)
			{
				SPIFlashReadArray(0x0000, (BYTE*)&NVMValidationStruct, sizeof(NVMValidationStruct));
				SPIFlashReadArray(sizeof(NVMValidationStruct), (BYTE*)&AppConfig, sizeof(AppConfig));
			}
			#endif
	
			// Check EEPROM/Flash validitity.  If it isn't valid, set a flag so 
			// that we will save the ROM default values on the next loop 
			// iteration.
			if((NVMValidationStruct.wConfigurationLength != sizeof(AppConfig)) ||
			   (NVMValidationStruct.wOriginalChecksum != wOriginalAppConfigChecksum) ||
			   (NVMValidationStruct.wCurrentChecksum != CalcIPChecksum((BYTE*)&AppConfig, sizeof(AppConfig))))
			{
				// Check to ensure that the vNeedToSaveDefaults flag is zero, 
				// indicating that this is the first iteration through the do 
				// loop.  If we have already saved the defaults once and the 
				// EEPROM/Flash still doesn't pass the validity check, then it 
				// means we aren't successfully reading or writing to the 
				// EEPROM/Flash.  This means you have a hardware error and/or 
				// SPI configuration error.
				if(vNeedToSaveDefaults)
				{
					while(1);
				}
				
				// Set flag and restart loop to load ROM defaults and save them
				vNeedToSaveDefaults = 1;
				continue;
			}
			
			// If we get down here, it means the EEPROM/Flash has valid contents 
			// and either matches the ROM defaults or previously matched and 
			// was run-time reconfigured by the user.  In this case, we shall 
			// use the contents loaded from EEPROM/Flash.
			break;
		}
		#endif
		break;
	}
}

#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
void SaveAppConfig(const APP_CONFIG *ptrAppConfig)
{
	NVM_VALIDATION_STRUCT NVMValidationStruct;

	// Ensure adequate space has been reserved in non-volatile storage to 
	// store the entire AppConfig structure.  If you get stuck in this while(1) 
	// trap, it means you have a design time misconfiguration in TCPIPConfig.h.
	// You must increase MPFS_RESERVE_BLOCK to allocate more space.
	#if defined(STACK_USE_MPFS2)
		if(sizeof(NVMValidationStruct) + sizeof(AppConfig) > MPFS_RESERVE_BLOCK)
			while(1);
	#endif

	// Get proper values for the validation structure indicating that we can use 
	// these EEPROM/Flash contents on future boot ups
	NVMValidationStruct.wOriginalChecksum = wOriginalAppConfigChecksum;
	NVMValidationStruct.wCurrentChecksum = CalcIPChecksum((BYTE*)ptrAppConfig, sizeof(APP_CONFIG));
	NVMValidationStruct.wConfigurationLength = sizeof(APP_CONFIG);

	// Write the validation struct and current AppConfig contents to EEPROM/Flash
	#if defined(EEPROM_CS_TRIS)
	    XEEBeginWrite(0x0000);
	    XEEWriteArray((BYTE*)&NVMValidationStruct, sizeof(NVMValidationStruct));
		XEEWriteArray((BYTE*)ptrAppConfig, sizeof(APP_CONFIG));
    #else
		SPIFlashBeginWrite(0x0000);
		SPIFlashWriteArray((BYTE*)&NVMValidationStruct, sizeof(NVMValidationStruct));
		SPIFlashWriteArray((BYTE*)ptrAppConfig, sizeof(APP_CONFIG));
    #endif
}
#endif

