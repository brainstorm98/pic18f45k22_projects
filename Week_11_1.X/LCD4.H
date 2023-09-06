/*
* LCD interface header file
* See lcd.c for more info
*/
/* write a byte to the LCD in 4 bit mode */

extern void lcd_write(unsigned char, unsigned char);

/* Clear and home the LCD */

extern void lcd_clear(unsigned char);

/* write a string of characters to the LCD */

extern void lcd_puts(const char * s, unsigned char);

/* Go to the specified position */

extern void lcd_goto(unsigned char pos, unsigned char);

/* intialize the LCD - call before anything else */

extern void lcd_init(unsigned char);

extern void lcd_putch(char, unsigned char);

extern void LCD_STROBE(unsigned char);

/* Set the cursor position */

#define lcd_cursor(x, lcd_no) lcd_write(((x)&0x7F)|0x80, lcd_no)