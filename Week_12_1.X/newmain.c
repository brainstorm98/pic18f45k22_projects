/*
 * File:   newmain.c
 * Author: Abdullah Ak
 *
 * Created on 20 May?s 2023 Cumartesi, 21:59
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>
#include "LCD4.H"

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned int value[2] = {0, 0};
unsigned char an_cnt = 0;
unsigned char an_list[2] = {0, 1};

void main(void) {
    ANSELA = 0x03;
    TRISA = 0xFF;

    ANSELB = 0x00;
    TRISB = 0xFF;

    ANSELC = 0x00;
    TRISC = 0xF9;

    ANSELD = 0x00;
    TRISD = 0xFF;

    ANSELE = 0x00;
    TRISE = 0x7F;

    ADCON2 = 0xA1;
    ADCON0 = 0x00;

    ADCON0bits.CHS = an_list[0];
    ADCON0bits.ADON = 1;
    GODONE = 1;
    
    T2CON=0x04;
    PR2=255;
    CCP1CON=0x0C;
    CCP2CON=0x0C;

    while (1) {
        if (!GODONE) {
            value[an_cnt] = (ADRESH * 256) + ADRESL;
            an_cnt++;
            if (an_cnt >= 2) {
                an_cnt = 0;
            }
            ADCON0bits.CHS = an_list[an_cnt];
            GODONE = 1;
        }
        CCPR1L=value[0]>>2;
        CCP1CON=(CCP1CON & 0xCF)+((value[0]&0x0003)<<4);
        CCPR2L=value[1]>>2;
        CCP2CON=(CCP2CON & 0xCF)+((value[1]&0x0003)<<4);
    }
}