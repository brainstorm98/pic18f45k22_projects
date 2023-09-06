/*
 * LCD interface example
 *Uses routines from delay.c
 *This code will interface to a standard LCD controller
 *like the Hitachi HD44780. It uses it in 4 bit mode, with
 *the hardware connected as follows (the standard 14 pin
 *LCD connector is used):
 *PORTD bits 0-3 are connected to the LCD data bits 4-7
 *PORTA bit 3 is connected to the LCD RS input (register select)
 *PORTA bit 1 is connected to the LCD EN bit (enable)
 *To use these routines, set up the port I/O (TRISA, TRISD) then
 *call lcd_init(), then other routines as required.
 */
#include <pic18f45k22.h>
#include <htc.h>

#define _XTAL_FREQ 8000000
#define LCD_RS PORTDbits.RD4 //PORTDbits.RB4
#define LCD_EN1 PORTDbits.RD5 //PORTDbits.RB5
#define LCD_EN2 PORTDbits.RD6 //PORTDbits.RB5
#define LCD_DATA PORTD

//#define LCD_STROBE() ((LCD_EN = 1),(LCD_EN=0))

void LCD_STROBE(unsigned char lcd_no) {
    if (lcd_no == 1) {
        LCD_EN1 = 1;
        LCD_EN1 = 0;
    }
    if (lcd_no == 2) {
        LCD_EN2 = 1;
        LCD_EN2 = 0;
    }
}

/* write a byte to the LCD in 4 bit mode */

void lcd_write(unsigned char c, unsigned char lcd_no) {
    __delay_us(40);
    LCD_DATA = (LCD_DATA & 0xF0)+((c >> 4) & 0x0F);
    LCD_STROBE(lcd_no);
    LCD_DATA = (LCD_DATA & 0xF0)+(c & 0x0F);
    LCD_STROBE(lcd_no);
}

/* Clear and home the LCD */

void lcd_clear(unsigned char lcd_no) {
    LCD_RS = 0;
    lcd_write(0x1, lcd_no);
    __delay_ms(2);
}

/* write a string of chars to the LCD */

void lcd_puts(const char * s, unsigned char lcd_no) {
    LCD_RS = 1; // write characters
    while (*s)
        lcd_write(*s++, lcd_no);
}

/* write one character to the LCD */

void lcd_putch(char c, unsigned char lcd_no) {
    LCD_RS = 1; // write characters
    lcd_write(c, lcd_no);
}

/* Go to the specified position */

void lcd_goto(unsigned char pos, unsigned char lcd_no) {
    LCD_RS = 0;
    lcd_write(0x80 + pos, lcd_no);
}

/* initialise the LCD - put into 4 bit mode */

void lcd_init(unsigned char lcd_no) {
    char init_value;
    init_value = 0x3;
    LCD_RS = 0;
    if (lcd_no == 1) {
        LCD_EN1 = 0;
    }
    if (lcd_no == 2) {
        LCD_EN2 = 0;
    }
    __delay_ms(15); // wait 15mSec after power applied,
    LCD_DATA = (LCD_DATA & 0xF0) + init_value;
    LCD_STROBE(lcd_no);
    __delay_ms(5);
    LCD_STROBE(lcd_no);
    __delay_us(200);
    LCD_STROBE(lcd_no);
    __delay_us(200);
    LCD_DATA = (LCD_DATA & 0xF0) + 2; // Four bit mode
    LCD_STROBE(lcd_no);
    lcd_write(0x28, lcd_no); // Set interface length
    lcd_write(0xC, lcd_no); // Display ON, Cursor OFF, Cursor Blink OFF
    lcd_clear(lcd_no); // Clear screen
    lcd_write(0x6, lcd_no); // Set entry Mode
}