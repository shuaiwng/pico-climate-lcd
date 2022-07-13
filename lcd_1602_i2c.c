/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lcd_1602_i2c.h"

//// commands
int LCD_CLEARDISPLAY = 0x01;
int LCD_RETURNHOME = 0x02;
int LCD_ENTRYMODESET = 0x04;
int LCD_DISPLAYCONTROL = 0x08;
int LCD_CURSORSHIFT = 0x10;
int LCD_FUNCTIONSET = 0x20;
int LCD_SETCGRAMADDR = 0x40;
int LCD_SETDDRAMADDR = 0x80;
//
//// flags for display entry mode
int LCD_ENTRYSHIFTINCREMENT = 0x01;
int LCD_ENTRYLEFT = 0x02;
//
//// flags for display and cursor control
int LCD_BLINKON = 0x01;
int LCD_CURSORON = 0x02;
int LCD_DISPLAYON = 0x04;
//
//// flags for display and cursor shift
int LCD_MOVERIGHT = 0x04;
int LCD_DISPLAYMOVE = 0x08;
//
//// flags for function set
int LCD_5x10DOTS = 0x04;
int LCD_2LINE = 0x08;
int LCD_8BITMODE = 0x10;
//
//// flag for backlight control
int LCD_BACKLIGHT = 0x08;
//
int LCD_ENABLE_BIT = 0x04;
//
//// By default these LCD display drivers are on bus address 0x27
static int addr = 0x27;

/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val) {
#ifdef i2c_default
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
#endif
}

void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
#define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// go to location on LCD
void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

static void inline lcd_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

