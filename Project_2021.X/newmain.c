/*
 * File:   newmain.c
 * Author: Abdullah Ak
 *
 * Created on 24 Nisan 2023 Pazartesi, 23:09
 */

#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned char seg_char[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x39};
unsigned char digits[4] = {0, 0, 0, 0};
unsigned char dig_cnt = 0;
__bit LED1;
__bit LED2;
unsigned int disp_cnt = 0;
unsigned long Analog0 = 0;
unsigned long Analog1 = 0;
unsigned long Temp = 0;

void main(void) {
    LED1 = 1;
    LED2 = 0;

    ANSELA = 0x00;
    TRISA = 0xFF;

    ANSELB = 0x00;
    TRISB = 0xFF;

    ANSELC = 0x00;
    TRISC = 0X00;
    PORTC = 0x00;

    ANSELD = 0xC0;
    TRISD = 0xC0;
    PORTD = 0x00;

    ANSELE = 0x00;
    TRISE = 0x7F;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0 = 0x6B;

    T0CON = 0xC3;
    TMR0IE = 1;
    TMR0IP = 1;
    GIE = 1;
   
    while (1) {
        PORTDbits.RD4 = LED1;
        PORTDbits.RD5 = LED2;
        if (ADCON0bits.GODONE == 0) {
            if (ADCON0bits.CHS == 26) {
                Analog0 = (ADRESH & 0x03)*256 + ADRESL;
                ADCON0bits.CHS = 27;
            } else if (ADCON0bits.CHS == 27) {
                Analog1 = (ADRESH & 0x03)*256 + ADRESL;
                ADCON0bits.CHS = 26;
            }
            ADCON0bits.GODONE = 1;
        }
    if (LED1) {
        Temp = (50000 * Analog0) / 1023;
    } else {
        Temp = (50000 * Analog1) / 1023;
    }
        digits[3] = Temp / 1000;
        digits[2] = (Temp - digits[3]*1000) / 100;
        digits[1] = (Temp - digits[3]*1000 - digits[2]*100) / 10;
        digits[0] = Temp % 10;
    }
    return;
}

void __interrupt(high_priority) isr() {
    if (TMR0IF) {
        TMR0IF = 0;
        PORTD = 0x0F + (LED2 * 32) + (LED1 * 16);
        if (dig_cnt != 0) {
            PORTC = seg_char[digits[dig_cnt]]+(128 * (dig_cnt == 2));
        } else {
            PORTC = seg_char[10];
        }
        PORTD = (~(0x01 << dig_cnt) & 0X0F) + (LED2 * 32) + (LED1 * 16);
        dig_cnt++;
        if (dig_cnt == 4) {
            dig_cnt = 0;
        }
        disp_cnt++;
        if (disp_cnt == 500) {
            disp_cnt = 0;
            LED1 = !LED1;
            LED2 = !LED2;
        }
    }
}