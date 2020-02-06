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
#include "LCDISPLIBLB3.h"
#include "adclib.h"

#define _XTAL_FREQ 4000000

char ADCval1 = 0; 
char ADCval2 = 0;
char ADCchannel = 0;

void dispval1(float dataf);
void dispval2(float dataf);

void __interrupt() ISR(void){
    if (PIR1bits.ADIF == 1){
        PIR1bits.ADIF = 0;
        switch (ADCchannel){
            case 0:
                ADCval1 = ADRESH;
                ADCON0bits.CHS = 0b1100;
                ADCchannel = 1;
                break;
            case 1:
                ADCval2 = ADRESH;
                ADCON0bits.CHS = 0b1101;   
                ADCchannel = 0;
                break;
        }
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
    float valf1;
    float valf2;
    lcd_start();
    char ancha = 13;
    initADC(ancha);
    lcd_cursor_set(1,2);
    lcd_wstring("SP1"); 
    
    lcd_cursor_set(1,8);
    lcd_wstring("SP2"); 
    
    lcd_cursor_set(1,13);
    lcd_wstring("CONT"); 
    
    while(1){
        valf1 = (ADCval1/51.0f);
        valf2 = (ADCval2/51.0f);
        dispval1(valf1);
        dispval2(valf2);   
    }
    return;
}

void dispval1(float dataf){
    int vals;
    int dec2;
    int dec1;
    int ent;
    vals = dataf*100.0f;
    vals = vals/1;
    dec2 = vals%10;
    vals = vals/10;
    dec1 = vals%10;
    ent = vals/10;
    switch (ent){
        case 0:
            lcd_cursor_set(2,1);
            lcd_wstring("0");
            break;
        case 1:
            lcd_cursor_set(2,1);
            lcd_wstring("1");
            break;
        case 2:
            lcd_cursor_set(2,1);
            lcd_wstring("2");
            break;
        case 3:
            lcd_cursor_set(2,1);
            lcd_wstring("3");
            break;
        case 4:
            lcd_cursor_set(2,1);
            lcd_wstring("4");
            break;   
        case 5:
            lcd_cursor_set(2,1);
            lcd_wstring("5");
            break;
        case 6:
            lcd_cursor_set(2,1);
            lcd_wstring("6");
            break;
        case 7:
            lcd_cursor_set(2,1);
            lcd_wstring("7");
            break;
        case 8:
            lcd_cursor_set(2,1);
            lcd_wstring("8");
            break;
        case 9:
            lcd_cursor_set(2,1);
            lcd_wstring("9");
            break;
    }
    lcd_wstring(".");
    switch (dec1){
        case 0:
            lcd_wstring("0");
            break;
        case 1:
            lcd_wstring("1");
            break;
        case 2:
            lcd_wstring("2");
            break;
        case 3:
            lcd_wstring("3");
            break;
        case 4:
            lcd_wstring("4");
            break;   
        case 5:
            lcd_wstring("5");
            break;
        case 6:
            lcd_wstring("6");
            break;
        case 7:
            lcd_wstring("7");
            break;
        case 8:
            lcd_wstring("8");
            break;
        case 9:
            lcd_wstring("9");
            break;
    }
    switch (dec2){
        case 0:
            lcd_wstring("0");
            break;
        case 1:
            lcd_wstring("1");
            break;
        case 2:
            lcd_wstring("2");
            break;
        case 3:
            lcd_wstring("3");
            break;
        case 4:
            lcd_wstring("4");
            break;   
        case 5:
            lcd_wstring("5");
            break;
        case 6:
            lcd_wstring("6");
            break;
        case 7:
            lcd_wstring("7");
            break;
        case 8:
            lcd_wstring("8");
            break;
        case 9:
            lcd_wstring("9");
            break;
    }
    lcd_wstring("V");
    return;
}

void dispval2(float dataf){
    int vals;
    int dec2;
    int dec1;
    int ent;
    vals = dataf*100.0f;
    vals = vals/1;
    dec2 = vals%10;
    vals = vals/10;
    dec1 = vals%10;
    ent = vals/10;
    switch (ent){
        case 0:
            lcd_cursor_set(2,7);
            lcd_wstring("0");
            break;
        case 1:
            lcd_cursor_set(2,7);
            lcd_wstring("1");
            break;
        case 2:
            lcd_cursor_set(2,7);
            lcd_wstring("2");
            break;
        case 3:
            lcd_cursor_set(2,7);
            lcd_wstring("3");
            break;
        case 4:
            lcd_cursor_set(2,7);
            lcd_wstring("4");
            break;   
        case 5:
            lcd_cursor_set(2,7);
            lcd_wstring("5");
            break;
        case 6:
            lcd_cursor_set(2,7);
            lcd_wstring("6");
            break;
        case 7:
            lcd_cursor_set(2,7);
            lcd_wstring("7");
            break;
        case 8:
            lcd_cursor_set(2,7);
            lcd_wstring("8");
            break;
        case 9:
            lcd_cursor_set(2,7);
            lcd_wstring("9");
            break;
    }
    lcd_wstring(".");
    switch (dec1){
        case 0:
            lcd_wstring("0");
            break;
        case 1:
            lcd_wstring("1");
            break;
        case 2:
            lcd_wstring("2");
            break;
        case 3:
            lcd_wstring("3");
            break;
        case 4:
            lcd_wstring("4");
            break;   
        case 5:
            lcd_wstring("5");
            break;
        case 6:
            lcd_wstring("6");
            break;
        case 7:
            lcd_wstring("7");
            break;
        case 8:
            lcd_wstring("8");
            break;
        case 9:
            lcd_wstring("9");
            break;
    }
    switch (dec2){
        case 0:
            lcd_wstring("0");
            break;
        case 1:
            lcd_wstring("1");
            break;
        case 2:
            lcd_wstring("2");
            break;
        case 3:
            lcd_wstring("3");
            break;
        case 4:
            lcd_wstring("4");
            break;   
        case 5:
            lcd_wstring("5");
            break;
        case 6:
            lcd_wstring("6");
            break;
        case 7:
            lcd_wstring("7");
            break;
        case 8:
            lcd_wstring("8");
            break;
        case 9:
            lcd_wstring("9");
            break;
    }
    lcd_wstring("V");
    return;
}
