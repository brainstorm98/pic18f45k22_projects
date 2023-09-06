/*
 * File:   newmain.c
 * Author: Abdullah Ak
 *
 * Created on 22 May?s 2023 Pazartesi, 23:04
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned char seg_char[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned int value[3] = {0, 0, 0};
unsigned char digits[3] = {0, 0, 0};
unsigned char dig_cnt = 0;
unsigned char an_cnt = 0;
unsigned char an_list[3] = {12, 10, 8};

void main(void) {
    ANSELA = 0x00;
    TRISA = 0xFF;

    ANSELB = 0x07;
    TRISB = 0xFF;

    ANSELC = 0x00;
    TRISC = 0xFF;

    ANSELD = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;

    ANSELE = 0x00;
    TRISE = 0x00;
    PORTE = 0x07;

    ADCON2 = 0xA1;
    ADCON0 = 0x00;

    ADCON0bits.CHS = an_list[0];
    ADCON0bits.ADON = 1;
    GODONE = 1;

    T0CON = 0xC2;
    TMR0L = 6;

    TMR0IE = 1;
    TMR0IP = 1;
    GIE = 1;

    while (1) {
        if (!GODONE) {
            value[an_cnt] = (ADRESH * 256) + ADRESL;
            an_cnt++;
            if (an_cnt >= 3) {
                an_cnt = 0;
            }
            ADCON0bits.CHS = an_list[an_cnt];
            GODONE = 1;
        }


        if (value[0] >= value[1] && value[0] >= value[2]) {
            digits[0] = 1;
            if (value[1] >= value[2]) {
                digits[1] = 2;
                digits[2] = 3;
            } else {
                digits[1] = 3;
                digits[2] = 2;
            }
        } else if (value[1] >= value[0] && value[1] >= value[2]) {
            digits[0] = 2;
            if (value[1] >= value[2]) {
                digits[1] = 1;
                digits[2] = 3;
            } else {
                digits[1] = 3;
                digits[2] = 1;
            }
        } else {
            digits[0] = 3;
            if (value[0] >= value[1]) {
                digits[1] = 1;
                digits[2] = 2;
            } else {
                digits[1] = 2;
                digits[2] = 1;
            }
        }
    }
    return;
}

void __interrupt(high_priority) isr() {
    if (TMR0IF) {
        TMR0IF = 0;
        TMR0L = 6;

        PORTE = 0x07;
        PORTD = seg_char[digits[dig_cnt]];
        PORTE = ~(0x01 << dig_cnt);

        dig_cnt++;
        if (dig_cnt >= 3) {
            dig_cnt = 0;
        }
    }
}