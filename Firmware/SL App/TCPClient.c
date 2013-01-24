/*********************************************************************
 *
 *  Generic TCP Client Example Application
 *   -Implements an HTTP client to connect to send barcode info and receive 
	product information.
 *
 *********************************************************************/

#define __TCPClient_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_GENERIC_TCP_CLIENT_EXAMPLE)

#include "TCPIP Stack/TCPIP.h"


// Defines the server to be accessed for this application
static BYTE ServerName[] =	"192.168.0.8";

// Defines the port to be accessed for this application
static WORD ServerPort = 8000;

// Defines the URL to be requested by this HTTP client
static char RemoteURL_base[] = "/api/agregar/?upc=%s&key=0481516234200";

static ROM BYTE RemoteURL[100];

static char codigo[15] = {'\0'};

static int int_cb =0;

static BYTE buffer_HTTP[500];

static int buffer_i = 0;



//Interrupt triggered when a new barcode is read.
void __attribute__((interrupt)) _U1RXInterrupt(void){
    static int primera = 0;
    //Si es la primera vez no hagas nada
    if (primera == 0){
        primera = 1;
        IFS0bits.U1RXIF = 0;
        return;
    }else{

        int i;
        i = getsUART1(13, codigo, 5000);
        int_cb=1;
        DATA_LED_O = 1;
        IFS0bits.U1RXIF = 0;
        return;
    }
}

/*****************************************************************************
  Function:
	void TCPClient(void)

  Summary:
	Implements an HTTP client to connect to send barcode info and receive 
	product information.

  Description:
	This function implements a simple HTTP client, which operates over TCP.  
	The function is called periodically by the stack, whenever a barcode is read.
	When a new code is read, the application opens a TCP
	connection to the SL server, sends the barcode, and displays on the LCD
	the name of the product read. 
	
	When the product is not found in the DB, the LCD displays an error message.

  ***************************************************************************/
void TCPClient(void)
{
	BYTE 				i;
	WORD				w;
	BYTE				vBuffer[9];
        char *body;
        //int filas = 0;
        static char multilinea[5][22] = {'\0'};
        
//        char **multilinea;
	static DWORD		Timer;
	static TCP_SOCKET	MySocket = INVALID_SOCKET;
	static enum _SuperListasTCPState
	{
                SM_WAITING = 0,
		SM_GOTCB,
		SM_SOCKET_OBTAINED,
		SM_PROCESS_RESPONSE,
                SM_DISPLAY,
		SM_DISCONNECT,
		
	} SuperListasTCPState = SM_WAITING;

	switch(SuperListasTCPState)
	{
                case SM_WAITING:
                    // Do nothing unless their's a new barcode 
                    if(int_cb == 1)
                            SuperListasTCPState = SM_GOTCB;
                    break;
		case SM_GOTCB:

			//Creates and connects a TCP socket 
                    int_cb=0;
                    sprintf(RemoteURL,RemoteURL_base, codigo);
                    putrsUART(RemoteURL);
			// Connect a socket to the remote TCP server
			MySocket = TCPOpen((DWORD)&ServerName[0], TCP_OPEN_RAM_HOST, ServerPort, TCP_PURPOSE_GENERIC_TCP_CLIENT);
			
			// Abort operation if no TCP socket of type TCP_PURPOSE_GENERIC_TCP_CLIENT is available
			if(MySocket == INVALID_SOCKET)
				break;

			#if defined(STACK_USE_UART)
			putrsUART((ROM char*)"\r\n\r\nConnecting to server...\r\n");
			#endif

			SuperListasTCPState++;
			Timer = TickGet();
			break;

		case SM_SOCKET_OBTAINED:
			//Transmits the HTTP request 

			// Wait for the remote server to accept our connection request
			if(!TCPIsConnected(MySocket))
			{
				// Time out if too much time is spent in this state
				if(TickGet()-Timer > 5*TICK_SECOND)
				{
					// Close the socket so it can be used by other modules
					TCPDisconnect(MySocket);
					MySocket = INVALID_SOCKET;
                                        //Si falla se regresa al principio.
                                        SuperListasTCPState = SM_WAITING;
					//SuperListasTCPState--;
				}
				break;
			}

			Timer = TickGet();

			// Make certain the socket can be written to
			if(TCPIsPutReady(MySocket) < 125u)
				break;
			
			// Place the application protocol data into the transmit buffer.  For this example, we are connected to an HTTP server, so we'll send an HTTP GET request.
			TCPPutROMString(MySocket, (ROM BYTE*)"GET ");
			TCPPutROMString(MySocket, RemoteURL);
			TCPPutROMString(MySocket, (ROM BYTE*)" HTTP/1.1\r\nHost: ");
			TCPPutString(MySocket, ServerName);
			TCPPutROMString(MySocket, (ROM BYTE*)"\r\nConnection: close\r\n\r\n");

			// Send the packet
			TCPFlush(MySocket);
			SuperListasTCPState++;
			break;

		case SM_PROCESS_RESPONSE:
			//Processes the HTTP response that includes the product details.


			// Check to see if the remote node has disconnected from us or sent us any application data
			if(!TCPIsConnected(MySocket))
			{
				SuperListasTCPState = SM_DISCONNECT;
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}
	
			// Get count of RX bytes waiting
			w = TCPIsGetReady(MySocket);	
	
			// Obtian and print the server reply
			i = sizeof(vBuffer)-1;
			vBuffer[i] = '\0';
			while(w)
			{
				if(w < i)
				{
					i = w;
					vBuffer[i] = '\0';
				}
				w -= TCPGetArray(MySocket, vBuffer, i);
                                buffer_i +=i;

                                //copia todo a buffer_HTTP
                                strcat(buffer_HTTP, vBuffer);
                                
				#if defined(STACK_USE_UART)
				putsUART((char*)vBuffer);
				#endif
				
				if(SuperListasTCPState == SM_PROCESS_RESPONSE)
					break;
			}
	
			break;

	
		case SM_DISCONNECT:
						//Sends the string received to the LCD screen 

                        //Busca el body del http header
                        body = strstr(buffer_HTTP, "\n\r\n");
                        body = body + 3;

                        

                        //Manda lo que tiene al LCD antes de regresar a waiting
                        if (strlen(buffer_HTTP) > 0)
                            LCD_printStr(body);
                        
                        //borrar array
                        buffer_i = 0;
                        memset (buffer_HTTP,'\0',sizeof(buffer_HTTP));
                        memset (codigo,'\0',sizeof(codigo));
			// Close the socket so it can be used by other modules
			// For this application, we wish to stay connected, but this state will still get entered if the remote server decides to disconnect
			TCPDisconnect(MySocket);
			MySocket = INVALID_SOCKET;
			SuperListasTCPState = SM_WAITING;
			break;
	

	}
}


#endif	//#if defined(STACK_USE_GENERIC_TCP_CLIENT_EXAMPLE)
