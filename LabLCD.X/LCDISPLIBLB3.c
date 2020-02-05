/*
 * File:   LCDISPLIBLB3.c
 * Author: David Vela
 *
 * Created on 3 de febrero de 2020, 09:57 PM
 */


#include <xc.h>
#include <stdint.h>
#include "LCDISPLIBLB3.h"
#define _XTAL_FREQ 4000000

/*
void lcd_databits(char iord, volatile uint8_t *port1){
    *port1 = iord;
}
*/

void lcd_databits(char data)
{
    if(data & 1){
        D0 = 1;   
    }
	else{
		D0 = 0;
    }
	if(data & 2){
        D1 = 1;
    }
	else{
		D1 = 0;
    }
	if(data & 4){
		D2 = 1;
    }
	else{
		D2 = 0;
    }
	if(data & 8){
		D3 = 1;
    }
	else{
		D3 = 0;
    }
	if(data & 16){
        D4 = 1;   
    }
	else{
		D4 = 0;
    }
	if(data & 32){
        D5 = 1;
    }
	else{
		D5 = 0;
    }
	if(data & 64){
		D6 = 1;
    }
	else{
		D6 = 0;
    }
	if(data & 128){
		D7 = 1;
    }
	else{
		D7 = 0;
    }
}

void lcd_char(char iord){
	RSBIT = 1;             // => RS = 0
	lcd_databits(iord);
	ENBL  = 1;             // => E = 1
    __delay_ms(5);
    ENBL  = 0;             // => E = 0
    __delay_ms(1);
}

void lcd_stinst5(char iord){
	RSBIT = 0;             // => RS = 0
	lcd_databits(iord);
	ENBL  = 1;             // => E = 1
    __delay_ms(5);
    ENBL  = 0;             // => E = 0
    __delay_ms(1);
}

void lcd_stinst1(char iord){
	RSBIT = 0;             // => RS = 0
	lcd_databits(iord);
	ENBL  = 1;             // => E = 1
    __delay_ms(1);
    ENBL  = 0;             // => E = 0
    __delay_ms(1);
}


void lcd_inst(char iord){
	RSBIT = 0;             // => RS = 0
	lcd_databits(iord);
	ENBL  = 1;             // => E = 1
    __delay_ms(4);
    ENBL  = 0;             // => E = 0
    __delay_ms(1);
}

void lcd_start(void){
    __delay_ms(15);
    lcd_stinst5(0x030);
    lcd_stinst1(0x030);
    lcd_stinst1(0x030);
  /////////////////////////////////////////////////////
    lcd_stinst5(0x38);
    lcd_inst(0x10);
    lcd_inst(0x01);
    lcd_inst(0x06);
    lcd_inst(0x0F);
}


void lcd_writechar(char m){
    RSBIT = 1;             // => RS = 0
	lcd_databits(m);
	ENBL  = 1;             // => E = 1
    __delay_ms(4);
    ENBL  = 0;             // => E = 0
    __delay_ms(1);
}


void lcd_cursor_set(char x, char y)
{
	char a;
	if(x == 1){
        a = 127 + y;
        lcd_inst(a);
	}
	else if(x == 2){
        a = 191 + y;
		lcd_inst(a);
	}
}
void lcd_clear(void)
{
	lcd_inst(0);
	lcd_inst(1);
}

void lcd_wstring(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   lcd_writechar(a[i]);
}