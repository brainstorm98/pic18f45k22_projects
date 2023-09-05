/*
 * File:   newmain.c
 * Author: Abdullah Ak
 *
 * Created on 24 Nisan 2023 Pazartesi, 23:08
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

#define RA0 PORTAbits.RA0

void main(void) {
    ANSELA = 0x00;
    TRISA = 0xFE;
    PORTA = 0x00;

    T0CON = 0x84;
    TMR0H = 3036 / 256;
    TMR0L = 3036 % 256;
    while (1) {
        if (TMR0IF) {
            TMR0IF = 0;
            TMR0H = 3036 / 256;
            TMR0L = 3036 % 256;
            RA0 = !RA0;
        }
        __delay_ms(800);
    }
    return;
}