/*
 * File:   adclib.c
 * Author: David Vela
 *
 * Created on 1 de febrero de 2020, 02:58 PM
 */

#include "adclib.h"
#include <pic16f887.h>
#include <xc.h>

void initADC(char var1) {
    ADCON1 = 0b00000000;
    switch (var1){
        default:
            ADCON0 = 0b11000001;
            break;
        case 1:
            ADCON0 = 0b11000101;
            break;
        case 2:
            ADCON0 = 0b11001001;
            break;
        case 3:
            ADCON0 = 0b11001101;
            break;
        case 4:
            ADCON0 = 0b11010001;
            break;
        case 5:
            ADCON0 = 0b11010101;
            break;
        case 6:
            ADCON0 = 0b11011001;
            break;
        case 7:
            ADCON0 = 0b11011101;
            break;
        case 8:
            ADCON0 = 0b11100001;
            break;
        case 9:
            ADCON0 = 0b11100101;
            break;
        case 10:
            ADCON0 = 0b11101001;
            break;
        case 11:
            ADCON0 = 0b11101101;
            break;
        case 12:
            ADCON0 = 0b11110001;
            break;
        case 13:
            ADCON0 = 0b11110101;
            break;
        case 14:
            ADCON0 = 0b11111001;
            break;
        case 15:
            ADCON0 = 0b11111101;    
            break;
    }
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    ADCON0bits.GO = 1;
    return;
}
