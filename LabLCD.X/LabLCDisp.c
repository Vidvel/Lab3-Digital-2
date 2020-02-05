/*
 * File:   LabLCDisp.c
 * Author: David Vela
 *
 * Created on 3 de febrero de 2020, 05:19 PM
 */
/* Codigo Float a Sting extraído de: https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "LCDISPLIBLB3.h"
#include "adclib.h"
#define MAX 100 

#define _XTAL_FREQ 4000000

char potval = 0; 

void __interrupt() ISR(void){
    if (PIR1bits.ADIF == 1){
        PIR1bits.ADIF = 0;
        potval = ADRESH;
        ADCON0bits.GO = 1;
    }
    else
        asm("nop");
}
//Inicialización puertos y variables
void main(void) {
    TRISB = 0b100001;
    ANSEL = 0;
    ANSELH = 0b0010100;
    TRISA = 0;
    TRISD = 0;
    ADCON1bits.ADFM = 1;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0 = 0b11000001;
    INTCON = 0b11000000;
    float valf;
    lcd_start();
    char ancha = 13;
    initADC(ancha);
    lcd_cursor_set(1,5);
    lcd_wstring("Aiuda");
    
    while(1){
        valf = (potval/51.0f);
        char text [16];
        sprintf(text, "%.2f", valf);
        lcd_cursor_set(2,1); 
        lcd_wstring(text);
        lcd_cursor_set(2,5);
        lcd_wstring("V");
    }
    return;
}

char hex_to_lcd (char var1){
    char out;
    switch (var1){
        default:
            out='-';
            break;
        case 1:
            out = '1';
            break;
        case 2:
            out = '2';
            break;
        case 3:
            out = '3';
            break;
        case 4:
            out = '4';
            break;
        case 5:
            out = '5';
            break;
        case 6:
            out = '6';
            break;
        case 7:
            out = '7';
            break;
        case 8:
            out = '8';
            break;
        case 9:
            out = '9';
            break;
        case 10:
            out = 'A';
            break;
        case 11:
            out = 'B';
            break;
        case 12:
            out = 'C';
            break;
        case 13:
            out = 'D';
            break;
        case 14:
            out = 'E';
            break;
        case 15:
            out = 'F';
            break;
    }
    return out;
}
