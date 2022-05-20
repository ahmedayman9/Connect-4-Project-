#include "tm4c123gh6pm.h"
#include "Random.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "uart.h"

void UARTB_init()
{

	SYSCTL_RCGCUART_R |= (1<<1);
	GPIO_PORTB_AFSEL_R |= 0x03;
	GPIO_PORTB_DEN_R |= 0x03;
	GPIO_PORTB_PCTL_R |= 0x11;
	UART1_CTL_R &= ~(1<<0)  ;
	UART1_IBRD_R = 8 ;
	UART1_FBRD_R = 44 ;
	// IBRD = int(16,000,000 / (16 * 115200)) = int(8.6805)
  // FBRD = int(0.6805 * 64 + 0.5) = 44
	UART1_LCRH_R = (3<<5);
	UART1_CC_R = 0 ;
	UART1_CTL_R |= (1<<0) | (1<<8)| (1<<9);

}
 char UARTB_InChar(void){
// as part of Lab 11, modify this program to use UART0 instead of UART1
  while((UART1_FR_R&UART_FR_RXFE) != 0);
  return(( char)(UART1_DR_R));
}

void UARTB_OutChar( char data){
// as part of Lab 11, modify this program to use UART0 instead of UART1
  while((UART1_FR_R&(1<<5)) != 0);
  UART1_DR_R = data;
}

void UARTB_outString(char* buffer)
{
	while(*buffer != 0 )
	{
		UARTB_OutChar(*buffer) ;
		buffer++;

	}


}