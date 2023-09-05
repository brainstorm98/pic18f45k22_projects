/*
 * File:   newmain.c
 * Author: Abdullah Ak
 *
 * Created on 24 Nisan 2023 Pazartesi, 23:09
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned char seg_char[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x1C};
unsigned long value;
unsigned char digits[4] = {0, 0, 0, 0};
unsigned char digit_counter = 0;
unsigned char conversion_done = 0;
unsigned char power = 1;

void main(void) {
    ANSELA = 0x00;
    TRISA = 0xF0;
    PORTA = 0x00;

    ANSELB = 0x00;
    TRISB = 0xFF;

    ANSELD = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;

    ANSELE = 0x02;
    TRISE = 0X07;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0 = 0x1B;
    T0CON = 0xC3;
    TMR0L = 6;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 1;
    INTCON2bits.INTEDG0 = 0;
    INTCONbits.INT0IE = 1;
    TMR0IE = 1;
    TMR0IP = 1;
    GIE = 1;
    while (1) {
        //if(!GODONE){
        // value = ((long)5000*(ADRESH*256+ADRESL))/1023;
        // GODONE = 1;
        //}
        if (conversion_done == 1) {
            conversion_done = 0;
            digits[3] = value / 1000;
            digits[2] = (value % 1000) / 100;
            digits[1] = (value % 100) / 10;
            digits[0] = value % 10;
            GODONE = 1;
        }
    }
    return;
}

void __interrupt(high_priority) isr() {
    if (TMR0IF) {
        TMR0IF = 0;
        TMR0L = 6;
        PORTA = 0x00;
                
        if (digit_counter == 0) {
            PORTD = seg_char[10];
        } else {
            PORTD = seg_char[digits[digit_counter]] + 0x80 * (digit_counter == 3);
        }
        
        PORTA = (0x01 * power) << digit_counter;
        digit_counter++;
        
        if (digit_counter >= 4) {
            digit_counter = 0;
        }
    }
    
    if (ADIF) {
        ADIF = 0;
        value = ((long) 5000 * (ADRESH * 256 + ADRESL)) / 1023;
        conversion_done = 1;
    }
    
    if (INT0IF) {
        INT0IF = 0;
        if (power == 1) {
            power = 0;
        } else {
            power = 1;
        }
    }
}