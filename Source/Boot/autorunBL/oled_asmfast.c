// Modified for BadgeWX, Updated 2019.May.15.:
// 1. remove HIGH reset pin state, set Input instead. BadgeWX has puillup, and this allows WiFi module to share the reset line and clear artifacts from the OLED.
// 2. remove support for 128x32 OLED displays - to save space



// automatically generated by spin2cpp v1.93 on Tue Aug 11 09:49:47 2015
// spin2cpp --ccode --main demo.spin 
//

// * charlieplexing driver for LEDs
// oled : "jm_ssd1306_spi_ez"                                     '   OLED display driver
/*       
************************************************
* screen (well, faster)                  *
* Thomas P. Sullivan                           *
* Copyright (c) 2012                           *
* Some original comments left/modified         *
* See end of file for terms of use.            *
************************************************
Revision History:
  V1.0   - Original program 12-2-2012
  V1.1   - Changes to comments and modification 
           of a few commands.
  V1.2   - Added support for the 128x64 display 12-16-2012

This is a Propeller driver object for the Adafruit
SSDD1306 OLED Display. It has functions to draw
individual pixels, lines, and rectangles. It also
has character functions to print 16x32 characters
derived from the Propeller's internal fonts.


     ┌─────────────────────────┐    
     │         SSD1306         │    
     │         Adafruit        │    
     │          128x32         │    
     │       OLED Display      │    
     │                         │    
     │   RST   CLK   VIN   GND │    
     │ CS   D/C   DATA  3.3    │    
     └─┬──┬──┬──┬──┬──┬──┬──┬──┘    
       │  │  │  │  │  │  │  │


This file is based on the following code sources:
************************************************
* Propeller SPI Engine                    v1.2 *
* Author: Beau Schwabe                         *
* Copyright (c) 2009 Parallax                  *
* See end of file for terms of use.            *
************************************************

...and this code:

*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in
any redistribution
*********************************************************************
Note: The splash screen is way down in the DAT section of this file.
         
 */
#include <stdlib.h>
#include "simpletools.h"
//#include <propeller.h>
#include "badgewxtools.h"

screen *screen_dataAddr();                     // Added 8/23 5:12 PM

//int screen_string8x1( char *str, int len);
//int screen_string8x2v2(char *str, int32_t len, int32_t row, int32_t col);


uint8_t oleddat[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
  0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x80, 0x80, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 
  0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xff, 
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 
  0x80, 0xff, 0xff, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x8c, 0x8e, 0x84, 0x00, 0x00, 0x80, 0xf8, 
  0xf8, 0xf8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0x80, 
  0x00, 0xe0, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xc7, 0x01, 0x01, 
  0x01, 0x01, 0x83, 0xff, 0xff, 0x00, 0x00, 0x7c, 0xfe, 0xc7, 0x01, 0x01, 0x01, 0x01, 0x83, 0xff, 
  0xff, 0xff, 0x00, 0x38, 0xfe, 0xc7, 0x83, 0x01, 0x01, 0x01, 0x83, 0xc7, 0xff, 0xff, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0x01, 0x01, 0x00, 0xff, 0xff, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7f, 0xff, 
  0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0x7f, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0xff, 
  0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0x0f, 0x3f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xc7, 0xc7, 0x8f, 
  0x8f, 0x9f, 0xbf, 0xff, 0xff, 0xc3, 0xc0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xfc, 0xfc, 
  0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xf8, 0xf8, 0xf0, 0xf0, 0xe0, 0xc0, 0x00, 0x01, 0x03, 0x03, 0x03, 
  0x03, 0x03, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 
  0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00, 
  0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
  0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x03, 
  0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x1f, 0x0f, 
  0x87, 0xc7, 0xf7, 0xff, 0xff, 0x1f, 0x1f, 0x3d, 0xfc, 0xf8, 0xf8, 0xf8, 0xf8, 0x7c, 0x7d, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x0f, 0x07, 0x00, 0x30, 0x30, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xc0, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 
  0x0f, 0x07, 0x1f, 0x7f, 0xff, 0xff, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0xe0, 
  0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00, 
  0x00, 0xfc, 0xfe, 0xfc, 0x0c, 0x06, 0x06, 0x0e, 0xfc, 0xf8, 0x00, 0x00, 0xf0, 0xf8, 0x1c, 0x0e, 
  0x06, 0x06, 0x06, 0x0c, 0xff, 0xff, 0xff, 0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xfc, 
  0xfe, 0xfc, 0x00, 0x18, 0x3c, 0x7e, 0x66, 0xe6, 0xce, 0x84, 0x00, 0x00, 0x06, 0xff, 0xff, 0x06, 
  0x06, 0xfc, 0xfe, 0xfc, 0x0c, 0x06, 0x06, 0x06, 0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00, 0xc0, 0xf8, 
  0xfc, 0x4e, 0x46, 0x46, 0x46, 0x4e, 0x7c, 0x78, 0x40, 0x18, 0x3c, 0x76, 0xe6, 0xce, 0xcc, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x0f, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 
  0x00, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x03, 0x07, 0x0e, 0x0c, 
  0x18, 0x18, 0x0c, 0x06, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x01, 0x0f, 0x0e, 0x0c, 0x18, 0x0c, 0x0f, 
  0x07, 0x01, 0x00, 0x04, 0x0e, 0x0c, 0x18, 0x0c, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 
  0x00, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x07, 
  0x07, 0x0c, 0x0c, 0x18, 0x1c, 0x0c, 0x06, 0x06, 0x00, 0x04, 0x0e, 0x0c, 0x18, 0x0c, 0x0f, 0x07, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xf0, 0xab, 0xbc, 0x0a, 0x00, 0x00, 0x68, 0x5c, 0x5d, 0x0a, 0xfc, 0x54, 0x55, 0xac, 0xbc, 0xa0, 
  0x05, 0xae, 0xfc, 0xa0, 0x56, 0xba, 0xbc, 0x08, 0x53, 0x0a, 0xbc, 0x80, 0x04, 0xac, 0xfc, 0x80, 
  0x05, 0xae, 0xfc, 0xe4, 0x10, 0xaa, 0xfc, 0x28, 0x01, 0xaa, 0x7c, 0x86, 0x10, 0x00, 0x68, 0x5c, 
  0x02, 0xaa, 0x7c, 0x86, 0x2e, 0x00, 0x68, 0x5c, 0xf0, 0xa5, 0x3c, 0x08, 0x00, 0x00, 0x7c, 0x5c, 
  0x01, 0xaa, 0xfc, 0xa0, 0x5d, 0xaa, 0xbc, 0x2c, 0x55, 0xaa, 0x3c, 0x61, 0x55, 0xec, 0xbf, 0x70, 
  0x55, 0xe8, 0xbf, 0x70, 0x01, 0xac, 0xfc, 0xa0, 0x5e, 0xac, 0xbc, 0x2c, 0x56, 0xac, 0x3c, 0x61, 
  0x56, 0xec, 0xbf, 0x70, 0x56, 0xe8, 0xbf, 0x70, 0x01, 0xb4, 0xfc, 0xa0, 0x5f, 0xb4, 0xbc, 0x2c, 
  0x5a, 0xb4, 0x3c, 0x61, 0x5a, 0xec, 0xbf, 0x70, 0x5a, 0xe8, 0xbf, 0x74, 0x61, 0xae, 0xbc, 0xa0, 
  0x80, 0xb2, 0xfc, 0xa0, 0x08, 0xb0, 0xfc, 0xa0, 0x59, 0xae, 0x3c, 0x61, 0x55, 0xe8, 0xbf, 0x70, 
  0x01, 0xb2, 0xfc, 0x28, 0x56, 0xac, 0x3c, 0x61, 0x56, 0xe8, 0xbf, 0x74, 0x56, 0xac, 0x3c, 0x61, 
  0x56, 0xe8, 0xbf, 0x70, 0x22, 0xb0, 0xfc, 0xe4, 0x5a, 0xb4, 0x3c, 0x61, 0x5a, 0xe8, 0xbf, 0x70, 
  0xf0, 0xa5, 0x3c, 0x08, 0x00, 0x00, 0x7c, 0x5c, 0x01, 0xaa, 0xfc, 0xa0, 0x5d, 0xaa, 0xbc, 0x2c, 
  0x55, 0xaa, 0x3c, 0x61, 0x55, 0xec, 0xbf, 0x70, 0x55, 0xe8, 0xbf, 0x70, 0x01, 0xac, 0xfc, 0xa0, 
  0x5e, 0xac, 0xbc, 0x2c, 0x56, 0xac, 0x3c, 0x61, 0x56, 0xec, 0xbf, 0x70, 0x56, 0xe8, 0xbf, 0x70, 
  0x01, 0xb4, 0xfc, 0xa0, 0x5f, 0xb4, 0xbc, 0x2c, 0x5a, 0xb4, 0x3c, 0x61, 0x5a, 0xec, 0xbf, 0x70, 
  0x5a, 0xe8, 0xbf, 0x70, 0x61, 0xb6, 0xbc, 0xa0, 0x54, 0xb8, 0xbc, 0xa0, 0x5b, 0xae, 0xbc, 0x00, 
  0x01, 0xb6, 0xfc, 0x80, 0x5a, 0xb4, 0x3c, 0x61, 0x5a, 0xe8, 0xbf, 0x74, 0x80, 0xb2, 0xfc, 0xa0, 
  0x08, 0xb0, 0xfc, 0xa0, 0x59, 0xae, 0x3c, 0x61, 0x55, 0xe8, 0xbf, 0x70, 0x01, 0xb2, 0xfc, 0x28, 
  0x56, 0xac, 0x3c, 0x61, 0x56, 0xe8, 0xbf, 0x74, 0x56, 0xac, 0x3c, 0x61, 0x56, 0xe8, 0xbf, 0x70, 
  0x45, 0xb0, 0xfc, 0xe4, 0x5a, 0xb4, 0x3c, 0x61, 0x5a, 0xe8, 0xbf, 0x70, 0x3f, 0xb8, 0xfc, 0xe4, 
  0xf0, 0xa5, 0x3c, 0x08, 0x00, 0x00, 0x7c, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
  0xff, 0xfc, 0xf8, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0xff, 0xa5, 0x99, 0xa5, 0xff, 0x00, 0x00, 0x00, 
  0x01, 0x07, 0x0f, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x81, 0x42, 0x24, 0x18, 0x18, 0x00, 0x00, 0x00, 
  0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00, 0x00, 0x00, 
  0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00, 0x00, 0x00, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00, 0x00, 0x00, 
  0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00, 0x00, 0x00, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00, 0x00, 0x00, 
  0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00, 0x00, 0x00, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00, 0x00, 0x00, 
  0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
  0x7e, 0xbd, 0xdb, 0xe7, 0xe7, 0x00, 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0xe7, 0x99, 0xe7, 0xff, 0x00, 0x00, 0x00, 
  0xff, 0xff, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 0xff, 0x81, 0x81, 0xff, 0xff, 0x00, 0x00, 0x00, 
  0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 
  0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 
  0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 
  0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 
  0xff, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x05, 0x00, 0x03, 0x05, 0x00, 0x00, 0x00, 
  0x14, 0x3e, 0x14, 0x3e, 0x14, 0x00, 0x00, 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00, 0x00, 0x00, 
  0x63, 0x10, 0x08, 0x04, 0x63, 0x00, 0x00, 0x00, 0x36, 0x49, 0x56, 0x20, 0x50, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00, 0x00, 0x00, 
  0x41, 0x22, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x18, 0x7e, 0x18, 0x24, 0x00, 0x00, 0x00, 
  0x08, 0x08, 0x3e, 0x08, 0x08, 0x00, 0x00, 0x00, 0xa0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x3e, 0x51, 0x49, 0x45, 0x3e, 0x00, 0x00, 0x00, 
  0x00, 0x42, 0x7f, 0x40, 0x00, 0x00, 0x00, 0x00, 0x62, 0x51, 0x51, 0x49, 0x46, 0x00, 0x00, 0x00, 
  0x22, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00, 0x18, 0x14, 0x12, 0x7f, 0x10, 0x00, 0x00, 0x00, 
  0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00, 0x00, 0x3c, 0x4a, 0x49, 0x49, 0x30, 0x00, 0x00, 0x00, 
  0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00, 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00, 
  0x06, 0x49, 0x49, 0x29, 0x1e, 0x00, 0x00, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xb6, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00, 
  0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x41, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00, 0x00, 
  0x02, 0x01, 0x51, 0x09, 0x06, 0x00, 0x00, 0x00, 0x3e, 0x41, 0x5d, 0x51, 0x4e, 0x00, 0x00, 0x00, 
  0x7c, 0x12, 0x11, 0x12, 0x7c, 0x00, 0x00, 0x00, 0x7f, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00, 
  0x1c, 0x22, 0x41, 0x41, 0x22, 0x00, 0x00, 0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00, 0x00, 0x00, 
  0x7f, 0x49, 0x49, 0x49, 0x41, 0x00, 0x00, 0x00, 0x7f, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00, 0x00, 
  0x3e, 0x41, 0x41, 0x51, 0x32, 0x00, 0x00, 0x00, 0x7f, 0x08, 0x08, 0x08, 0x7f, 0x00, 0x00, 0x00, 
  0x41, 0x41, 0x7f, 0x41, 0x41, 0x00, 0x00, 0x00, 0x20, 0x40, 0x40, 0x40, 0x3f, 0x00, 0x00, 0x00, 
  0x7f, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00, 0x7f, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 
  0x7f, 0x02, 0x0c, 0x02, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x04, 0x08, 0x10, 0x7f, 0x00, 0x00, 0x00, 
  0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00, 0x00, 0x00, 0x7f, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 
  0x3e, 0x41, 0x51, 0x21, 0x5e, 0x00, 0x00, 0x00, 0x7f, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00, 0x00, 
  0x26, 0x49, 0x49, 0x49, 0x32, 0x00, 0x00, 0x00, 0x01, 0x01, 0x7f, 0x01, 0x01, 0x00, 0x00, 0x00, 
  0x3f, 0x40, 0x40, 0x40, 0x3f, 0x00, 0x00, 0x00, 0x07, 0x18, 0x60, 0x18, 0x07, 0x00, 0x00, 0x00, 
  0x3f, 0x40, 0x38, 0x40, 0x3f, 0x00, 0x00, 0x00, 0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00, 0x00, 
  0x03, 0x04, 0x78, 0x04, 0x03, 0x00, 0x00, 0x00, 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x00, 
  0x7f, 0x7f, 0x41, 0x41, 0x41, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x60, 0x00, 0x00, 0x00, 
  0x41, 0x41, 0x41, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x10, 0x08, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 
  0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x05, 0x00, 0x00, 0x00, 0x00, 
  0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00, 0x00, 0x7f, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 
  0x38, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x7f, 0x00, 0x00, 0x00, 
  0x38, 0x54, 0x54, 0x54, 0x58, 0x00, 0x00, 0x00, 0x08, 0x7e, 0x09, 0x09, 0x02, 0x00, 0x00, 0x00, 
  0x18, 0xa4, 0xa4, 0xa4, 0x78, 0x00, 0x00, 0x00, 0x7f, 0x04, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00, 
  0x00, 0x44, 0x7d, 0x40, 0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x84, 0x7d, 0x00, 0x00, 0x00, 0x00, 
  0x6f, 0x10, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x41, 0x7f, 0x40, 0x00, 0x00, 0x00, 0x00, 
  0x7c, 0x04, 0x38, 0x04, 0x7c, 0x00, 0x00, 0x00, 0x7c, 0x04, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00, 
  0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0xfc, 0x24, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 
  0x18, 0x24, 0x24, 0x24, 0xfc, 0x00, 0x00, 0x00, 0x7c, 0x08, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 
  0x48, 0x54, 0x54, 0x54, 0x24, 0x00, 0x00, 0x00, 0x04, 0x3f, 0x44, 0x44, 0x20, 0x00, 0x00, 0x00, 
  0x3c, 0x40, 0x40, 0x20, 0x7c, 0x00, 0x00, 0x00, 0x1c, 0x20, 0x40, 0x20, 0x1c, 0x00, 0x00, 0x00, 
  0x7c, 0x40, 0x30, 0x40, 0x7c, 0x00, 0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 
  0x1c, 0xa0, 0xa0, 0xa0, 0x7c, 0x00, 0x00, 0x00, 0x44, 0x64, 0x54, 0x4c, 0x44, 0x00, 0x00, 0x00, 
  0x08, 0x3e, 0x77, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x41, 0x41, 0x77, 0x3e, 0x08, 0x00, 0x00, 0x00, 0x04, 0x02, 0x06, 0x04, 0x02, 0x00, 0x00, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
};

volatile screen badgeScreen;
volatile screen *self;
volatile int screenLock = 0;

//static  int32_t screen_setcommand( int32_t cmd, int32_t argptr);
int32_t screen_setcommand( int32_t cmd, int32_t argptr);

// ------------------------------------------------------------------------------------------------------------------------------

int32_t screen_SHIFTOUT(int32_t Dpin, int32_t Cpin, int32_t CSpin, int32_t Bits, int32_t Value)
{
  int32_t _parm__0012[5];
  _parm__0012[0] = Dpin;
  _parm__0012[1] = Cpin;
  _parm__0012[2] = CSpin;
  _parm__0012[3] = Bits;
  _parm__0012[4] = Value;
  screen_setcommand(1, (int32_t)(&_parm__0012[0]));
  return 0;
}

int32_t screen_WRITEBUFF(int32_t Dpin, int32_t Cpin, int32_t CSpin, int32_t Bits, int32_t Addr)
{
  int32_t _parm__0013[5];
  _parm__0013[0] = Dpin;
  _parm__0013[1] = Cpin;
  _parm__0013[2] = CSpin;
  _parm__0013[3] = Bits;
  _parm__0013[4] = Addr;
  screen_setcommand(2, (int32_t)(&_parm__0013[0]));
  return 0;
}

int32_t screen_start(void)
{
  self = &badgeScreen;
  int32_t okay = 0;
  // Start SPI Engine - starts a cog
  // returns false if no cog available
  screen_stop();
  okay = (self->cog = cognew((int32_t)(&(*(int32_t *)&oleddat[1024])), (int32_t)(&self->command)) + 1);
  screenLock = locknew();
  lockclr(screenLock);
  return okay;
}

int32_t screen_setcommand(int32_t cmd, int32_t argptr)
{
  // Write command and pointer
  
//  while(lockset(screenLock));
  self->command = (cmd << 16) + argptr;
  while (self->command) 
  {
    Yield__();
  }
//  lockclr(screenLock);
  return 0;
}

// Wait for command to be cleared, signifying receipt
int32_t screen_init(int32_t ChipSelect, int32_t DataCommand, int32_t TheData, int32_t TheClock, int32_t Reset, int32_t VCC_state, int32_t Type)
{
  // Startup the SPI system
  screen_start();
  // Initialize variables and initialize the display
  self->CS = ChipSelect;
  self->DC = DataCommand;
  self->DATA = TheData;
  self->CLK = TheClock;
  self->RST = Reset;
  self->vccstate = VCC_state;
  self->displayType = Type;
  /*if (self->displayType == TYPE_128X32) {
    self->displayWidth = SSD1306_LCDWIDTH;
    self->displayHeight = SSD1306_LCDHEIGHT32;
  } else {*/
    self->displayWidth = SSD1306_LCDWIDTH;
    self->displayHeight = SSD1306_LCDHEIGHT64;
  //}
  // Setup reset and pin direction  
  //screen_HIGH(self->RST);
  // VDD (3.3V) goes high at start; wait for a ms
  
  pause(1);
  //waitcnt(((CLKFREQ / 100000) + CNT));
  
  // force reset low
  screen_LOW(self->RST);
  // wait 10ms
  
  pause(1);
  //waitcnt(((CLKFREQ / 100000) + CNT));
  
  // remove reset
  //screen_HIGH(self->RST);
  //uint32_t rstmask = 1 << self->RST;
  DIRA &= ~(1 << self->RST); // rstmask; // set to input
  
  //if (self->displayType == TYPE_128X32) {
    // ************************************
    // Init sequence for 128x32 OLED module
    // ************************************
    /*
	screen_ssd1306_Command(SSD1306_DISPLAYOFF);
    screen_ssd1306_Command(SSD1306_SETDISPLAYCLOCKDIV);
    screen_ssd1306_Command(128);
    screen_ssd1306_Command(SSD1306_SETMULTIPLEX);
    screen_ssd1306_Command(31);
    screen_ssd1306_Command(SSD1306_SETDISPLAYOFFSET);
    screen_ssd1306_Command(0);
    screen_ssd1306_Command((SSD1306_SETSTARTLINE | 0x0));
    screen_ssd1306_Command(SSD1306_CHARGEPUMP);
    if (self->vccstate == SSD1306_EXTERNALVCC) {
      screen_ssd1306_Command(16);
    } else {
      screen_ssd1306_Command(20);
    }
    screen_ssd1306_Command(SSD1306_MEMORYMODE);
    screen_ssd1306_Command(0);
    screen_ssd1306_Command((SSD1306_SEGREMAP | 0x1));
    screen_ssd1306_Command(SSD1306_COMSCANDEC);
    screen_ssd1306_Command(SSD1306_SETCOMPINS);
    screen_ssd1306_Command(2);
    screen_ssd1306_Command(SSD1306_SETCONTRAST);
    screen_ssd1306_Command(143);
    screen_ssd1306_Command(SSD1306_SETPRECHARGE);
    if (self->vccstate == SSD1306_EXTERNALVCC) {
      screen_ssd1306_Command(34);
    } else {
      // SSD1306_SWITCHCAPVCC 
      screen_ssd1306_Command(241);
    }
    screen_ssd1306_Command(SSD1306_SETVCOMDETECT);
    screen_ssd1306_Command(64);
    screen_ssd1306_Command(SSD1306_DISPLAYALLON_RESUME);
    screen_ssd1306_Command(SSD1306_NORMALDISPLAY);
    // --turn on oled panel
    screen_ssd1306_Command(SSD1306_DISPLAYON);
	*/
  //} else {
    // ************************************   
    // Init sequence for 128x64 OLED module
    // ************************************
    screen_ssd1306_Command(SSD1306_DISPLAYOFF);
    // low col = 0
    screen_ssd1306_Command(SSD1306_SETLOWCOLUMN);
    // hi col = 0
    screen_ssd1306_Command(SSD1306_SETHIGHCOLUMN);
    // line #0
    screen_ssd1306_Command(SSD1306_SETSTARTLINE);
    screen_ssd1306_Command(SSD1306_SETCONTRAST);
    if (self->vccstate == SSD1306_EXTERNALVCC) {
      screen_ssd1306_Command(159);
    } else {
      screen_ssd1306_Command(207);
    }
    screen_ssd1306_Command(161);
    screen_ssd1306_Command(SSD1306_NORMALDISPLAY);
    screen_ssd1306_Command(SSD1306_DISPLAYALLON_RESUME);
    screen_ssd1306_Command(SSD1306_SETMULTIPLEX);
    screen_ssd1306_Command(63);
    screen_ssd1306_Command(SSD1306_SETDISPLAYOFFSET);
    // No offset                            
    screen_ssd1306_Command(0);
    screen_ssd1306_Command(SSD1306_SETDISPLAYCLOCKDIV);
    screen_ssd1306_Command(128);
    screen_ssd1306_Command(SSD1306_SETPRECHARGE);
    if (self->vccstate == SSD1306_EXTERNALVCC) {
      screen_ssd1306_Command(34);
    } else {
      screen_ssd1306_Command(241);
    }
    screen_ssd1306_Command(SSD1306_SETVCOMDETECT);
    screen_ssd1306_Command(64);
    screen_ssd1306_Command(SSD1306_SETCOMPINS);
    screen_ssd1306_Command(18);
    screen_ssd1306_Command(SSD1306_MEMORYMODE);
    screen_ssd1306_Command(0);
    screen_ssd1306_Command((SSD1306_SEGREMAP | 0x1));
    screen_ssd1306_Command(SSD1306_COMSCANDEC);
    screen_ssd1306_Command(SSD1306_CHARGEPUMP);
    if (self->vccstate == SSD1306_EXTERNALVCC) {
      screen_ssd1306_Command(16);
    } else {
      screen_ssd1306_Command(20);
    }
    // --turn on oled panel
    screen_ssd1306_Command(SSD1306_DISPLAYON);
  //}
  self->crsrX = 0;
  self->crsrY = 0;
  self->charSize = LARGE;
  invert(0);
  //screen_AutoUpdateOff();              // commented 8/23 5:26 PM
  //clear();
  return 0;
}

int32_t screen_update(void)
{
  while(lockset(screenLock));
  int32_t _local__0014[2] = {0, 0};
  // Writes the screen buffer to the memory of the display
  // low col = 0
  screen_ssd1306_Command(SSD1306_SETLOWCOLUMN);
  // hi col = 0
  screen_ssd1306_Command(SSD1306_SETHIGHCOLUMN);
  // line #0
  screen_ssd1306_Command(SSD1306_SETSTARTLINE);
  screen_HIGH(self->DC);
  screen_WRITEBUFF(self->DATA, self->CLK, self->CS, (int32_t)(&_local__0014[1]), (int32_t)(&self->buffer[0]));
  //screen_WRITEBUFF(self->DATA, self->CLK, self->CS, 8192, (int32_t)(&self->buffer[0]));
  screen_LOW(self->DC);
  lockclr(screenLock);
  return 0;
}

int32_t screen_HIGH(int32_t Pin)
{
  // Make a pin an output and drives it high
  DIRA = ((DIRA & (~(1 << Pin))) | (1 << Pin));
  OUTA = ((OUTA & (~(1 << Pin))) | (1 << Pin));
  return 0;
}

int32_t screen_LOW(int32_t Pin)
{
  // Make a pin an output and drives it low
  DIRA = ((DIRA & (~(1 << Pin))) | (1 << Pin));
  OUTA &= ~(1<<Pin);
  return 0;
}

int32_t screen_ssd1306_Command(int32_t thecmd)
{
  int32_t _local__0016[1];
  // Send a byte as a command to the display
  // Write SPI command to the OLED
  screen_LOW(self->DC);
  screen_SHIFTOUT(self->DATA, self->CLK, self->CS, (int32_t)(&_local__0016[0]), thecmd);
  return 0;
}

int32_t screen_ssd1306_Data(int32_t thedata)
{
  int32_t _local__0017[1];
  // Send a byte as data to the display
  // Write SPI data to the OLED
  screen_HIGH(self->DC);
  screen_SHIFTOUT(self->DATA, self->CLK, self->CS, (int32_t)(&_local__0017[0]), thedata);
  return 0;
}


// 
//
  

  
// $7F
/* 
┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                                   TERMS OF USE: MIT License                                                  │                                                            
├──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation    │ 
│files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,    │
│modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software│
│is furnished to do so, subject to the following conditions:                                                                   │
│                                                                                                                              │
│The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.│
│                                                                                                                              │
│THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE          │
│WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR         │
│COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,   │
│ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                         │
└──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
 */


