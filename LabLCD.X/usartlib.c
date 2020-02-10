/*
 * File:   usartlib.c
 * Author: David Vela
 *
 * Created on 7 de febrero de 2020, 12:31 PM
 */

#include "usartlib.h"
#include <pic16f887.h>
#include <xc.h>

void usart_setup(void) {
    
    SPBRG = 25 ;
    
    BAUDCTLbits.SCKP = 0;
    BAUDCTLbits.BRG16 = 0;
    BAUDCTLbits.ABDEN = 0;
    
    TXSTAbits.TX9 = 0;/*
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    PIE1bits.TXIE = 1;
    
    */
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
   
    TXSTAbits.BRGH = 1;
    SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
    SPEN = 1;                                     //Enables Serial Port
    TRISC7 = 1;                                   //As Prescribed in Datasheet
    TRISC6 = 1;                                   //As Prescribed in Datasheet
    CREN = 1;                                     //Enables Continuous Reception
    TXEN = 1;                                     //Enables Transmission
    
    return;
}

void UART_Write(char data)
{
  while(!TRMT);
  TXREG = data;
}
char UART_Read(void){
int intro;
  while(!RCIF){
      asm("nop");
  }
  intro = RCREG;
  return intro;
}

char UART_Data_Ready(void)
{
  return RCIF;
}