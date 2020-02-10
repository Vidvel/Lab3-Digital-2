/* 
 * File:   usartlib.h
 * Author: David Vela
 *
 * Created on 6 de febrero de 2020, 03:33 PM
 */

#ifndef USARTLIB_H
#define	USARTLIB_H
void usart_setup(void);
void UART_Write(char data);
char UART_Read(void);
char UART_Data_Ready(void);
#endif	/* USARTLIB_H */