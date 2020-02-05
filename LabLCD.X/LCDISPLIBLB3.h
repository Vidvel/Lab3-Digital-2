/* 
 * File:   LCDISPLIBLB3.h
 * Author: David Vela
 *
 * Created on 3 de febrero de 2020, 05:16 PM
 */

#ifndef LCDISPLIBLB3_H
#define	LCDISPLIBLB3_H
#include <xc.h> 
#include <stdint.h>


#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif             

#ifndef RSBIT
#define RSBIT RD1
#endif

#ifndef ENBL
#define ENBL RD0
#endif

#ifndef D0
#define D0 RA0
#endif

#ifndef D1
#define D1 RA1
#endif

#ifndef D2
#define D2 RA2
#endif

#ifndef D3
#define D3 RA3
#endif

#ifndef D4
#define D4 RA4
#endif

#ifndef D5
#define D5 RA5
#endif

#ifndef D6
#define D6 RA6
#endif

#ifndef D7
#define D7 RA7
#endif

void lcd_start(void);
void lcd_databits(char iord);
void lcd_stinst5(char iord);
void lcd_stinst4(char iord);
void lcd_stinst1(char iord);
void lcd_inst(char iord);
void lcd_cursor_set(char x, char y);
void lcd_writechar(char m);
void lcd_char(char iord);
/*
void lcd_clear(void);
void lcd_wchar(char word);*/

#endif	/* LCDISPLIBLB3_H */