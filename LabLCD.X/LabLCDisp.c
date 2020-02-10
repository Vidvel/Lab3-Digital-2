/*
 * File:   LabLCDisp.c
 * Author: David Vela
 *
 * Created on 3 de febrero de 2020, 05:19 PM
 */
/*NOTAS PARA SERIAL (PAGE 155)
 * TRANSMITIR DATA
 TXSTA register: Transmit status and control
 RCSTA register: Recive Status control
 BAUDCTL register: Baud Rate 
 Transmisor del EUSART: TXEN y SPEN en 1, SYNC en 0
 TXIF es la interrupción que se habilita con TXEN
 TXREG es lo que contiene el valor que se envia del PIC
 * TXIF esta en el PIR1; está en 1 si el transmisor está
 * habilitado y no hay datos en el TXREG.
 * TXIF es 0 si TSR está con otro caracter y hay otro en cola
 * TXIF sólo se puede leer, no modificarla
 * 
 * To use interrupts when transmitting data, set the TXIE
bit only when there is more data to send. Clear the
TXIE interrupt enable bit upon writing the last character
of the transmission to the TXREG.
 * 
 * RECIBIR DATA
 * RCIE
Fosc de 4MHz, SYNC = 0, BRGH = 1 y BRH16 = 0 da BOUDR de 9615 (0.16%)
 * SPBRG = 25 
 * Entonces: TXSTA-> TX9 = 0 (8bit data)
 * TXEN = 1 (transmision encendida)
 * SYNC = 0 (asíncrono)
 * 
 * RCSTA -> SPEN = 1
 * RX9 = 0
 * CREN = ?
 * 
 * BAUDCTL -> SCKP = 0
 * Librería Para el USART extraída de:
 * https://electrosome.com/uart-pic-microcontroller-mplab-xc8/
 * 
 */

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
#include "usartlib.h"

#define _XTAL_FREQ 4000000

char ADCval1 = 0; 
char ADCval2 = 0;
char ADCchannel = 0;
char send = 0;
char contador = 0;
char recived = 0;

void dispval1(float dataf);
void dispval2(float dataf);
void cntdisp(char datac);

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
    else if (PIR1bits.TXIF == 1) {
        send = 1; 
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
    usart_setup();
    lcd_cursor_set(1,2);
    lcd_wstring("SP1"); 
    
    lcd_cursor_set(1,8);
    lcd_wstring("SP2"); 
    
    lcd_cursor_set(1,13);
    lcd_wstring("CONT"); 
    
    while(1){
        UART_Write(ADCval1);
        UART_Write(ADCval2);
        valf1 = (ADCval1/51.0f);
        valf2 = (ADCval2/51.0f);
        dispval1(valf1);
        dispval2(valf2); 
        if (UART_Data_Ready()){
           recived = UART_Read(); 
           RCSTAbits.CREN = 1; 
        }
        else{
            asm("nop");
        }
        if (recived == 0x2B){
            contador++;
            recived = 0;
        }
        else if (recived == 0x2D){
            contador--;
            recived = 0;
        }
        else{
            asm("nop");
        }
        cntdisp(contador);
    }
    return;
}

void cntdisp(char datac){
    int vals;
    int ent1;
    int ent2;
    int ent3;
    ent3 = datac%10;
    vals = datac/10;
    ent2 = vals%10;
    ent1 = vals/10;
    lcd_cursor_set(2,13);
    switch (ent1){
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
    switch (ent2){
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
    switch (ent3){
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
