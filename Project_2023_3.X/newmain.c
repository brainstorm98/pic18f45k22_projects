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

unsigned char state = 0; // traffic light state information.
// It is 0 for red, 1 for yellow, 2 for green.
// System will start with red light
unsigned char redLightPeriod = 30; // period of red traffic light
unsigned char yellowLightPeriod = 3; // period of yellow traffic light
unsigned char greenLightPeriod = 15; // period of green traffic light
unsigned char chronometer = 30; // Countdown timer variable.
// It will start from 30 seconds because traffic light opens
// in red light state and counts down
unsigned char seg_char[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x40};
unsigned char digits[4] = {5, 10, 0, 0};
unsigned char digit_counter = 0;
unsigned char time_cnt = 0;
unsigned char power = 0xFF;

void main(void) {
    ANSELA = 0x00;
    TRISA = 0xFF;

    ANSELB = 0x00;
    TRISB = 0xFF;
    INTCON2bits.RBPU = 0;

    ANSELC = 0x00;
    TRISC = 0x00;
    PORTC = 0x00;

    ANSELD = 0x00;
    TRISD = 0x0F;
    PORTD = 0x00;

    ANSELE = 0x00;
    TRISE = 0x00;
    PORTE = 0x01;

    T0CON = 0xC3;
    TMR0L = 131;

    INTCON2bits.INTEDG0 = 0;
    INTCONbits.INT0IE = 1;
    TMR0IE = 1;
    TMR0IP = 1;
    GIE = 1;

    while (1) {
        digits[3] = chronometer / 10;
        digits[2] = chronometer % 10;
    }
    return;
}

void __interrupt(high_priority) isr() {
    if (TMR0IF) {
        TMR0IF = 0;
        TMR0L = 131;
        PORTD = 0x00;
        PORTC = seg_char[digits[digit_counter]];
        PORTD = (0x80 >> digit_counter) & power;
        digit_counter++;
        if (digit_counter >= 4) {
            digit_counter = 0;
            time_cnt++;
            if (time_cnt >= 125 * 2) {
                time_cnt = 0;
                chronometer--;
                if (chronometer == 0) {
                    if (state == 0) {
                        state = 1;
                        chronometer = yellowLightPeriod;
                    } else if (state == 1) {
                        state = 2;
                        chronometer = greenLightPeriod;
                    } else if (state == 2) {
                        state = 0;
                        chronometer = redLightPeriod;
                    }
                    PORTE = (0x01 << state) & power;
                }
            }
        }
    }
    if (INT0IF) {
        INT0IF = 0;
        power = ~power;
        PORTE = (0x01 << state) & power;
        PORTD = (0x80 >> digit_counter) & power;
    }
}