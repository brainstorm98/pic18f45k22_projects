/*
 * File:   newmain.c
 * Author: Abdullah Ak
 *
 * Created on 20 May?s 2023 Cumartesi, 19:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>
#include "LCD4.H"

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned int AN[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Analog okumalar için dizi
float Voltage[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Voltaj dönü?ümleri için dizi
unsigned char an_cnt = 0; // Analog okuma sayac?
unsigned char line[41] = "                                         "; // LCD'ye yazd?r?lacak metin için dizi

void main(void) {
    
    ANSELA = 0x2F;
    TRISA = 0xFF;
    
    ANSELB = 0x3F;
    TRISB = 0xFF;
    
    ANSELC = 0xFC;
    TRISC = 0xFF;
    
    ANSELD = 0x00;
    TRISD = 0x80;
    
    ANSELE = 0x07;
    TRISE = 0x7F;
    
    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    
    ADCON0bits.CHS = 0; // ADC
    ADCON0bits.ADON = 1;
    GODONE = 1;
    
    lcd_init(1);
    lcd_init(2);
    
    lcd_clear(1);
    lcd_clear(2);
    
    lcd_goto(0, 1);
    lcd_goto(0, 2);
    
    while (1) {
        while (an_cnt < 20) {
            ADCON0bits.CHS = an_cnt;
            GODONE = 1;
            while (GODONE);
            AN[an_cnt] = (ADRESH << 8) + ADRESL;
            Voltage[an_cnt] = 5.0 / 1023 * AN[an_cnt];
            an_cnt++;
        }
        an_cnt = 0;
        
        sprintf(line, "00:%4.2f 01:%4.2f 02:%4.2f 03:%4.2f 04:%4.2f",Voltage[0],Voltage[1],Voltage[2],Voltage[3],Voltage[4]);
        lcd_goto(0,1);
        lcd_puts(line,1);
        
        sprintf(line, "05:%4.2f 06:%4.2f 07:%4.2f 08:%4.2f 09:%4.2f",Voltage[5],Voltage[6],Voltage[7],Voltage[8],Voltage[9]);
        lcd_goto(64,1);
        lcd_puts(line,1);
        
        sprintf(line, "10:%4.2f 11:%4.2f 12:%4.2f 13:%4.2f 14:%4.2f",Voltage[10],Voltage[11],Voltage[12],Voltage[13],Voltage[14]);
        lcd_goto(0,2);
        lcd_puts(line,2);
        
        sprintf(line, "15:%4.2f 16:%4.2f 17:%4.2f 18:%4.2f 19:%4.2f",Voltage[15],Voltage[16],Voltage[17],Voltage[18],Voltage[19]);
        lcd_goto(64,2);
        lcd_puts(line,2);
    }
    return;
}
