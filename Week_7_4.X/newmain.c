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
unsigned long value[6] = {0, 0, 0, 0, 0, 0};
unsigned char digits[4] = {0, 0, 0, 0};
unsigned char digit_counter = 0;
unsigned char an_cnt = 0;
unsigned char an_list[6] = {12, 10, 8, 9, 11, 13};
unsigned char disp_cnt = 0;
unsigned char disp_num = 0;

void main(void) {
    ANSELA = 0x00;
    TRISA = 0xF0;
    PORTA = 0x00;

    ANSELB = 0x3F;
    TRISB = 0xFF;

    ANSELC = 0x00;
    TRISC = 0xC0;
    PORTC = 0x00;

    ANSELD = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    //ADCON0 = 0x1B;
    ADCON0bits.CHS = an_list[0];
    ADCON0bits.ADON = 1;
    GODONE = 1;
    T0CON = 0xC3;
    TMR0L = 6;
    TMR0IE = 1;
    TMR0IP = 1;
    GIE = 1;
    while (1) {
        if (!GODONE) {
            value[an_cnt] = ((long) 5000 * (ADRESH * 256 + ADRESL)) / 1023;
            an_cnt++;
            if (an_cnt >= 6) {
                an_cnt = 0;
            }
            ADCON0bits.CHS = an_list[an_cnt];
            GODONE = 1;
        }
        digits[3] = value[disp_num] / 1000;
        digits[2] = (value[disp_num] % 1000) / 100;
        digits[1] = (value[disp_num] % 100) / 10;
        digits[0] = value[disp_num] % 10;
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
        PORTA = 0x01 << digit_counter;
        digit_counter++;
        if (digit_counter >= 4) {
            digit_counter = 0;
            disp_cnt++;
            if (disp_cnt >= 125) {
                disp_cnt = 0;
                disp_num++;
                if (disp_num >= 6) {
                    disp_num = 0;
                }
                PORTC = 0x01 << disp_num;
            }
        }
    }
}