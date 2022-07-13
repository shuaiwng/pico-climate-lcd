/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

/* Example code to drive a 16x2 LCD panel via a I2C bridge chip (e.g. PCF8574)

   NOTE: The panel must be capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefor I2C) cannot be used at 5v.

   You will need to use a level shifter on the I2C lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO 4 (pin 6)-> SDA on LCD bridge board
   GPIO 5 (pin 7)-> SCL on LCD bridge board
   3.3v (pin 36) -> VCC on LCD bridge board
   GND (pin 38)  -> GND on LCD bridge board
*/
//// commands
extern int LCD_CLEARDISPLAY ;
extern int LCD_RETURNHOME ;
extern int LCD_ENTRYMODESET ;
extern int LCD_DISPLAYCONTROL ;
extern int LCD_CURSORSHIFT ;
extern int LCD_FUNCTIONSET ;
extern int LCD_SETCGRAMADDR ;
extern int LCD_SETDDRAMADDR ;

// flags for display entry mode
extern int LCD_ENTRYSHIFTINCREMENT ;
extern int LCD_ENTRYLEFT ;

// flags for display and cursor control
extern int LCD_BLINKON ;
extern int LCD_CURSORON ;
extern int LCD_DISPLAYON ;

// flags for display and cursor shift
extern int LCD_MOVERIGHT ;
extern int LCD_DISPLAYMOVE ;

// flags for function set
extern int LCD_5x10DOTS ;
extern int LCD_2LINE ;
extern int LCD_8BITMODE ;

// flag for backlight control
extern int LCD_BACKLIGHT ;

extern int LCD_ENABLE_BIT ;

// By default these LCD display drivers are on bus address 0x27
//static int addr ;

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES     2 
#define MAX_CHARS      16

/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val); 

void lcd_toggle_enable(uint8_t val); 

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode);

void lcd_clear(void);


// go to location on LCD
void lcd_set_cursor(int line, int position);

static void inline lcd_char(char val);

void lcd_string(const char *s);

void lcd_init();

