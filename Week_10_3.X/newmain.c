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
#include "LCD.H"

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned int cnt = 0; // Sayaç de?i?kenini tan?mlama
unsigned char line[17] = "                 "; // LCD'de görüntülenecek karakter dizisi

void main(void) {
    
    ANSELB = 0x00; // PORTB'nin tüm pinlerini dijital olarak yap?land?r
    TRISB = 0xC0; // PORTB'nin ilk 6 pinini ç?k??, son 2 pinini giri? olarak yap?land?r

    lcd_init(); // LCD'yi ba?lat 
    lcd_clear(); // LCD ekran?n? temizle
    lcd_goto(0); // LCD'de ilk konuma git

    lcd_puts("  HELLO WORLD!  "); // LCD'ye "HELLO WORLD!" yazd?r

    while (1) {
        __delay_ms(20); // 20 milisaniye beklet
        //sprintf(line, "%d  ", cnt);
        //sprintf(line,"%d",cnt);
        //sprintf(line,"%d ",cnt);
        //sprintf(line,"%3d",cnt);
        //sprintf(line,"%03d",cnt);
        sprintf(line," %01d %02d ",cnt); // 'cnt' de?erini 'line' dizisine yazd?r
        lcd_goto(0x40); // LCD'nin ikinci sat?r?na git
        lcd_puts(line); // LCD'ye 'line' dizisini yazd?r
        cnt++; // 'cnt' de?erini bir art?r
        if (cnt >= 1000) { // E?er 'cnt' 1000'e e?it veya büyükse
            cnt = 0; // 'cnt' de?erini s?f?rla
        }
    }
    return;
}
