{{

----------------------------------

Parallax BadgeWX TestCode
  
Compiled from various sources/authors by: Michael Mulholland
Last Update: 2019 Mar 15

----------------------------------



' ------------------------------------------------------------------------------------------------------------------------

' Version history

' ------------------------------------------------------------------------------------------------------------------------

' 2019.03.15.
'
'       Cleaned up old objects for GitHub push
'

' 2019.02.12.
'
'       RGB Timing and brightness tweak (for eye sanity!)
'


' 2018.09.26.
' 
'       Fix RGB strip length (from 4 to 5 for test jig)
'       

' 2018.09.09.
'
'       Add exit state to work with mfg test jig
'


' 2018.08.24.
'
'       SNEcog updated to allow audio on both P0 and P1 (previous P0 was not allowed, as 0 meant off)
'


' 2018.06.19.
'
'       Try a stability fix on the EEprom driver


' 2018.05.21.
'
'       IR testing changed a little, to enable easier manual testing
'
'       1. TX led check: Test code simply blinks the IR led on/off slowly, so you can easily see that with your camera
'       2. RX diode check: Test code waits to receive anything.  Ie. you could use a TV remote and press any button to confirm the RX diode is receiving. RX test period increased to 10 seconds.
'

'       
' 2018.05.14.
'
'       Dim the RGB leds for tester sanity.
'       Add test of IR 


' 2018.05.11.
'
'       Initial version



' ------------------------------------------------------------------------------------------------------------------------

' TODO: 

' ------------------------------------------------------------------------------------------------------------------------
' ------------------------------------------------------------------------------------------------------------------------
' ------------------------------------------------------------------------------------------------------------------------ 



Steps...



 
' ------------------------------------------------------------------------------------------------------------------------ 

}}


CON
        _clkmode = xtal1 + pll16x                       'Standard clock mode * crystal frequency = 80 MHz
        _xinfreq = 5_000_000

        CLK_FREQ = ((_clkmode - xtal1) >> 6) * _xinfreq               ' system freq as a constant
        MS_001   = CLK_FREQ / 1_000                                   ' ticks in 1ms
        US_001   = CLK_FREQ / 1_000_000                               ' ticks in 1us

        
        {
        ' Compile for RevA2 BadgeWX
        WX_BOOT_PIN = 16 ' RevA2
        WX_RESET_PIN = 17 ' RevA2
        }

        
        ' Compile for RevA BadgeWX
        WX_BOOT_PIN = 17 ' RevA
        WX_RESET_PIN = 16 ' RevA


        LED_COM = 9
        RGB     = 10
        LEDS    = 10                                                     ' LED signal pin
        RGB_STRIP_LEN = 5
        RGB_BRIGHTNESS = 5

        STRIP_LEN = 5
        PIX_BITS  = 24                                                ' RGB pixels

        MAX_LEN = 5                                          ' 4 chars + zero terminator
        
        VIDEO_BASEPIN = 24

        IR_OUT   = 3                                                  ' IR coms
        IR_IN    = 23
        'IR_DUMMYOUT = 27
        
        IR_BAUD  =   2_400  { max supported using IR connection }
        IR_FREQ  =  36_000  { matches receiver on DC22 badge }
  
        OLED_CS  = 18
        OLED_RST = 19
        OLED_CLK = 20
        OLED_DAT = 21
        OLED_DC  = 22

        WX_DI =  30 ' Duplicate pin defines, from WiFi module perspective, used in WiFi code
        WX_DO =  31 ' Duplicate pin defines, from WiFi module perspective, used in WiFi code

        P27MASK = 1 << 27
        
        
        SDA      = 29                                                  ' eeprom / i2c
        SCL      = 28

        EEPROM = $50 ' 7-bit device ID for EEPROM
        
        ' SD pins (REV A & A2 - Black Badge)
        _dopin = 8
        _clkpin = 7
        _dipin = 6
        _cspin = 5
        _cdpin = -1 ' -1 if unused.
        _wppin = -1 ' -1 if unused.
         
        _rtcres1 = -1 ' -1 always.
        _rtcres2 = -1 ' -1 always.  
        _rtcres3 = -1 ' -1 always.  
         
        

        ' Audio
        playRate = 21 'Hz
        rightPin = 0 
        leftPin  = 1

        
         
         
VAR

        long label, offset, hits
        long errorString

        byte bcount

        long len, xg, yg, zg, countdown
        
        long  idx                                             ' pointer into string
        byte  nstr[MAX_LEN]                                   ' string for numeric data

        byte RGBCOG_STOP, LEDCOG_STOP, MUSICCOG_STOP
        long cogid_music

        long  pixbuf1[STRIP_LEN]                                      ' pixel buffers

        byte buffer[8]  
        
        long volume[3] 
        
        long StackCog1[16] ' 0/1 LEDs
        long StackCog2[64] ' RGB LEDs
        long StackCog3[200] ' Music
        
            


OBJ
        
        wificom : "FullDuplexSerial"

        oled    : "sj_oled"
        strip   : "jm_rgbx_pixel_minimal"

        text    : "tv_text"
        acc     : "jm_mma7660fc"
        I2C     : "I2C SPIN driver v1.4od"

        irdata  : "jm_ir_hdserial"                      ' half-duplex serial via IR

        d       : "DTMF"                                ' Phone Dialer
        
        
PUB Start | i ', t, xg, yg, zg


   RGBCOG_STOP := 0
   LEDCOG_STOP := 0 
   MUSICCOG_STOP := 0
   
   ' Flashing 0/1 leds in cog
   cognew(FlashLEDs, @StackCog1)
   

   
   ' Flashing RGB leds in cog
   cognew(FlashRGBLEDs, @StackCog2)

      
   
   ' Set up display
   oled.init(OLED_CS, OLED_DC, OLED_DAT, OLED_CLK, OLED_RST, oled#SSD1306_SWITCHCAPVCC, oled#TYPE_128X64)
   oled.AutoUpdateOff
   oled.clearDisplay
   'oled.write2x8String(string(" HELLO! "),8,0)   ' str,len,row  
   oled.write4x16String(string("  BadgeWX Test  "),16,0,0) ' str,len,row,col
   

   oled.write4x16String(string("Waiting for jig!"),16,2,0)
   oled.updateDisplay

   
   ' Wait for jig to toggle P27 few times
   repeat 5
      waitpeq(P27MASK, P27MASK, 0)
      waitpne(P27MASK, P27MASK, 0)

      
   oled.write4x16String(string("                "),16,2,0)
   oled.updateDisplay

   

   ' Check some pins

   High(2)
   In(2)
   waitcnt(clkfreq/8 + cnt)
   if (In(2) == 1)
      oled.write4x16String(string(" P2 resistor bad"),16,3,0)
      oled.write4x16String(string("   TEST FAIL!   "),16,7,0)
      oled.updateDisplay
      repeat
        waitcnt(0)
    
   High(4)
   In(4)
   waitcnt(clkfreq/8 + cnt)
   if (In(4) == 1)
      oled.write4x16String(string(" P4 resistor bad"),16,3,0)
      oled.write4x16String(string("   TEST FAIL!   "),16,7,0)
      oled.updateDisplay
      repeat
        waitcnt(0)
   
   High(14)
   In(14)
   waitcnt(clkfreq/8 + cnt)
   if (In(14) == 1)
      oled.write4x16String(string("P14 resistor bad"),16,3,0)
      oled.write4x16String(string("   TEST FAIL!   "),16,7,0)
      oled.updateDisplay
      repeat
        waitcnt(0)

   High(15)
   In(15)
   waitcnt(clkfreq/8 + cnt)
   if (In(15) == 1)
      oled.write4x16String(string("P15 resistor bad"),16,3,0)
      oled.write4x16String(string("   TEST FAIL!   "),16,7,0)
      oled.updateDisplay
      repeat
        waitcnt(0)
        



   
   ' Music (takes 2 cogs)
   cogid_music := cognew(SoundTest, @StackCog3)

   ' Video
   ' Output some stuff to screen
   text.start(VIDEO_BASEPIN)
   'text.str(string(13,"   BadgeWX Demo...",13,13,$C,5," www.blocklyprop.com | Parallax ",$C,1))
   text.str(string(13,"   BadgeWX Demo...",13,13,$C,5," blockly.parallax.com | Parallax",$C,1))
   repeat 14
      text.out(" ")
   repeat i from $0E to $FF
      text.out(i)
   text.str(string($C,6,"     Uses internal ROM font     ",$C,2))

   
   
   ' EEprom
   I2C.init(SCL,SDA)
   waitcnt(clkfreq/2 + cnt)

   if not \EEPROM_test
      oled.write4x16String(string("  EEPROM ERROR  "),16,2,0)
      oled.write4x16String(string("Not found! FAIL!"),16,7,0)
      oled.updateDisplay
      repeat
         waitcnt(0)
   
      
   ' If get here, EEprom test OK
   oled.write4x16String(string("   EEPROM OK    "),16,1,0) ' str,len,row,col
   oled.updateDisplay


   I2C.I2C_stop

     

   ' Accelerometer
   acc.start(SCL, SDA)
   
   ifnot (acc.present)                                            ' check for MMA7660
    oled.write4x16String(string("No MMA7660"),10,3,3)
    oled.write4x16String(string("   TEST FAIL!   "),16,7,0)
    oled.updateDisplay
    repeat
      waitcnt(0)                  

   

   ' read all axes, Signed byte 6-bit 2's complement data with allowable range of +31 to -32
   repeat 5

     acc.read_all(@xg)
     show_gforce(xg, 3, 0)
     show_gforce(yg, 3, 5)
     show_gforce(zg, 3, 10)
    
     oled.updateDisplay
     waitcnt(clkfreq/4 + cnt)
      

   ' if last values not zero, assume OK!
   xg := xg + yg + zg
   if (xg == 0)
    oled.write4x16String(string("XYZ INVALID!"),12,4,2)
    oled.write4x16String(string("   TEST FAIL!   "),16,7,0)
    oled.updateDisplay
    repeat
      waitcnt(0)

   
   'acc.stop

   ' If get here, Accelerometer test OK
   oled.write4x16String(string("ACCELEROMETER OK"),16,4,0) ' str,len,row,col
   oled.updateDisplay


   

   ' Nav Buttons
   oled.write4x16String(string("Move L-NAV Left "),16,7,0)
   oled.updateDisplay
   High(13)
   repeat until In(14) == 1
   In(13)

   oled.write4x16String(string("Move L-NAV Right"),16,7,0)
   oled.updateDisplay
   High(11)
   repeat until In(14) == 1
   In(11)

   oled.write4x16String(string("Press L-NAV In  "),16,7,0)
   oled.updateDisplay
   High(12)
   repeat until In(14) == 1
   In(12)

   '
   oled.write4x16String(string("Move R-NAV Left "),16,7,0)
   oled.updateDisplay
   High(13)
   repeat until In(15) == 1
   In(13)

   oled.write4x16String(string("Move R-NAV Right"),16,7,0)
   oled.updateDisplay
   High(11)
   repeat until In(15) == 1
   In(11)

   oled.write4x16String(string("Press R-NAV In  "),16,7,0)
   oled.updateDisplay
   High(12)
   repeat until In(15) == 1
   In(12)
   
   
   ' If get here, Buttons test OK
   oled.write4x16String(string(" NAV BUTTONS OK "),16,5,0) ' str,len,row,col
   oled.updateDisplay



   ' Passive buttons test?
   ' TODO:

   ' ------------------------------------------------------------------------------

   
   ' Must free up some cogs for the IR and WiFi tests to follow!
   
   
   RGBCOG_STOP := 1
   'LEDCOG_STOP := 1 ' Leave the 2 LEDs running; a handy indicator of life!
   MUSICCOG_STOP := 1 ' Stops 2 music cogs
   text.stop ' Stop the Video cog

   
   'strip.clear
   longfill(@pixbuf1, $00_00_05_00, STRIP_LEN) ' set all RGB's calm blue
   
      
   ' ------------------------------------------------------------------------------    

   ' Setup IR test
   irdata.start(IR_IN, IR_OUT, IR_BAUD, IR_FREQ)                  ' start IR serial
   
   oled.write4x16String(string("  Link IR LEDs  "),16,7,0)
   oled.updateDisplay
      
   i := 0
   countdown := 40 ' Allow 10 seconds, then give up waiting (based on 1/4 second pause in the loop)
   hits := 0
   
   repeat until (hits > 2) or (countdown == 0)

    irdata.tx("U")
    i := irdata.rxcheck

    if i > 0
      ++hits

    if hits > 9
      oled.write4x16String(dec(hits),2,6,13)
    else
      oled.write4x16String(dec(hits),1,6,13)

    oled.write4x16String(dec(i),2,6,8) ' str,len,row,col
    oled.updateDisplay

    

    --countdown
        
    waitcnt(clkfreq/4 + cnt)


   if (hits > 2)

      ' If get here, IR test OK

      oled.write4x16String(string("     IR OK      "),16,6,0) ' str,len,row,col
      oled.updateDisplay


   elseif (countdown == 0)

      oled.write4x16String(string("IR TEST SKIPPED!"),16,6,0) ' str,len,row,col
      oled.updateDisplay

   else
    
      ' If get here, IR test FAILED  (can't get here with countdown!)

      oled.write4x16String(string("    IR FAIL     "),16,6,0) ' str,len,row,col
      oled.updateDisplay   


   ' ------------------------------------------------------------------------------
   ' ------------------------------------------------------------------------------


       
   ' WiFi
   
   oled.write4x16String(string("Starting WiFi..."),16,7,0)
   oled.updateDisplay


   ' Disable wifi module protection and bootloader
   'High(WX_BOOT_PIN)

   ' Reset WiFi module
   'Low(WX_RESET_PIN)
   'waitcnt(clkfreq/800 + cnt)      ' 125us second pause
   'In(WX_RESET_PIN)

   'waitcnt(clkfreq + cnt) ' 1 second wait

    
   ' Set wiFi module to command mode, by holding DI low longer than 30/baud seconds
   Low(WX_DI)
   waitcnt(clkfreq/4 + cnt)
   In(WX_DI)
             
   wificom.start(WX_DO,WX_DI,0,115200) ' rxpin, txpin, mode, baudrate




   oled.write4x16String(string("Run Bootloader.."),16,7,0)
   oled.updateDisplay


   'strip.clear
   longfill(@pixbuf1, $00_00_00_00, STRIP_LEN) ' clear all RGB's
    

   ' Signal to testjig that this is done...

   High(27)
   waitcnt(clkfreq/2 + cnt) ' Give jig a second to detect high signal

    

   ' --- FRUN load the next autorun stage
   ' This will test all the WiFi functions, AND ensure Propeller-to-WiFi comms works!

   wificom.str(string($FE,13,10))
   wificom.str(string($FE,"FRUN:autorunBL.bin",13,10))

   'reboot


    
   ' ------------------------------------------------------------------------------
   ' ------------------------------------------------------------------------------

   
   



   
   
   ' ------------------------------------------------------------------------------
   ' ------------------------------------------------------------------------------
      
   ' The end!
   ' WiFi module on the Badge will be loading, so won't get here (or not for very long) !
   repeat
    waitcnt(0)
   


   
   ' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   ' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   ' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   
  
    
  




' ----------------------------------------------------------------------------------------------------------------
' ----------------------------------------------------------------------------------------------------------------

PRI EEPROM_test


    I2C.writeBytes(EEPROM,$8003,@EEpromTestData,7)

    I2C.readBytes(EEPROM,$8003,@buffer,7)

    oled.write4x16String(@EEpromTestData,7,2,0)
    oled.write4x16String(@buffer,7,2,8)
      
    if !strcomp(@EEpromTestData, @buffer)
     
        oled.write4x16String(string("EEPROM R/W ERROR"),16,2,0)
        oled.write4x16String(string("   TEST FAIL!   "),16,7,0)
        oled.updateDisplay
        repeat
            waitcnt(0)
      
    
    return true
    

' ----------------------------------------------------------------------------------------------------------------
' ----------------------------------------------------------------------------------------------------------------




pub show_gforce(g, row, colbase) | div

'' Show g-force value as n.nn


'oled.write5x7Char(dec(xg), 3, 0) ' ch,row,col

  if (g < 0)                         
    oled.write5x7Char("-", row, colbase) ' oled.char("-")
    g := ||g
  elseif (g == 0)
    oled.write5x7Char(" ", row, colbase) ' oled.char(" ")  
  else
    oled.write5x7Char("+", row, colbase) ' oled.char("+")

  oled.write5x7Char("0" + (g / 100), row, ++colbase) ' oled.char("0" + (g / 100))                                     ' print 100s
  oled.write5x7Char(".", row, ++colbase) ' oled.char(".")                                                 ' decimal point
  g //= 100                                                      ' remove 100s
  oled.write5x7Char("0" + (g  / 10), row, ++colbase) ' oled.char("0" + (g  / 10))                                     ' print 10s
  oled.write5x7Char("0" + (g  // 10), row, ++colbase) ' oled.char("0" + (g // 10))                                     ' print 1s
    

' ----------------------------------------------------------------------------------------------------------------


' ----------------------------------------------------------------------------------------------------------------

PRI FlashLEDs

  ' --- FLASH LEDS

   repeat until LEDCOG_STOP == 1

      High(IR_OUT)
      
      ' Flash 0
      repeat 2
      
        Low(LED_COM)
        waitcnt(clkfreq/8 + cnt) 

        In(LED_COM)
        waitcnt(clkfreq/8 + cnt)

      ' Flash 1  
      repeat 2
         
        High(LED_COM)
        waitcnt(clkfreq/8 + cnt)

        In(LED_COM)
        waitcnt(clkfreq/8 + cnt)

      ' Flash both 0 and 1
      repeat 8000
         
        High(LED_COM)
        Low(LED_COM)


      Low(IR_OUT)
          
      ' Both leds off and little pause
      In(LED_COM)
      waitcnt(clkfreq/2 + cnt) 


' ----------------------------------------------------------------------------------------------------------------

PRI FlashRGBLEDs

    longfill(@pixbuf1, $00_00_00_00, STRIP_LEN)                   ' prefill buffers
    
    'strip.start_2812b(@pixbuf1, STRIP_LEN, LEDS, 0_5)             ' start pixel driver for WS2812b
    strip.startx(@pixbuf1, STRIP_LEN, LEDS, 1_0, true, 24, 250,700,600,500) ' Timing for RevA 1818, RevB 1823

    
    ' Scroll from right to left, through each color Red, Green, Blue, White
    repeat until RGBCOG_STOP == 1 

      repeat 3

        if RGBCOG_STOP == 1
          quit
      
        color_wipe($05_00_00_00, 400)
        color_wipe($00_05_00_00, 400)      
        color_wipe($00_00_05_00, 400)
        color_wipe($05_05_05_00, 400)
      
    cogstop(cogid)

' ----------------------------------------------------------------------------------------------------------------


pub color_wipe(rgbval, ms) | ch

'' Sequentially fills strip with color rgb
'' -- ms is delay between pixels, in milliseconds

  repeat ch from 0 to STRIP_LEN-1 ' strip.num_pixels-1 

    if RGBCOG_STOP == 1
      return
     
    'strip.set(ch, rgbval)
    pixbuf1[ch] := rgbval 
    pause(ms)
    
 
' ----------------------------------------------------------------------------------------------------------------

PUB find(ch, p_str) | fidx

'' Find position of char ch in string at p_str
'' -- returns -1 if not found

  repeat fidx from 0 to strsize(p_str)-1
    if (byte[p_str][fidx] == ch)
      return fidx                    

  return -1
  
' ----------------------------------------------------------------------------------------------------------------

pub left(destpntr, srcpntr, llen)

'' Copies left len characters from src string to destination str 

  bytemove(destpntr, srcpntr, llen)
  byte[destpntr][llen] := 0


{pub right(destpntr, srcpntr, rlen) | sl

'' Copies right len characters from src string to destination str

  sl := strsize(srcpntr)                                        ' length of source
  bytemove(destpntr, srcpntr+(sl-rlen), rlen+1)                   ' include terminating 0
}

pub mid(destpntr, srcpntr, midstart, mlen)

'' Copies middle len characters from src to dest
'' -- start is zero indexed

  bytemove(destpntr, srcpntr+midstart, mlen)
  byte[destpntr][mlen] := 0

' ----------------------------------------------------------------------------------------------------------------

PUB Trim (strAddr) | i, size 
{{Removes byte characters 9,10,11,13,32 from beginning and end of a string
Trim(" Test here ",13)
Output: "Test here"
}}
  size := strsize(strAddr)
  result := strAddr

  REPEAT size                                                                   ' look at beginning of string
    CASE byte[result]
      9..11,13,32: result++
      OTHER: QUIT
  
  
  i := strAddr + size
  REPEAT WHILE (i > result)                                                     ' look at end of string
    CASE byte[i--]
      9..11,13,32:
      OTHER: QUIT

  byte[i] := 0

' ----------------------------------------------------------------------------------------------------------------


PUB High(pin)
{{Set I/O pin to output-high = 3.3 V.

Parameter:

  pin = I/O pin number

Example:

  pin.High(9)  ' P9 output-high

  See Example with pin Nickname above.
}}
  
  outa[pin] := 1
  dira[pin] := 1

PUB Low(pin)
{{Set I/O pin to output-low = 0 V.

  PARAMETER: pin = I/O pin number

Example:

  pin.Low(9)   ' P9 output-low
}}
  
  outa[pin] := 0
  dira[pin] := 1  
  
PUB In(pin) : state
{{Sets I/O pin to input and checks state
of voltage applied to the pin.
 
  PARAMETER: pin = I/O pin number
  RETURNS: state = 1 if above 1.65 V
                   0 if below 1.65 V

Example:

             Returns 1 if voltage at pin is above 1.65 V,   
             or 0 if it's below.

             var := pin.In(3)

             Result copied to a variable named var.
}}

  dira[pin] := 0                                                                                                            
  state := ina[pin]


' ----------------------------------------------------------------------------------------------------------------

pub pause(ms) | t

'' Delay program in milliseconds

  if (ms < 1)                                                   ' delay must be > 0
    return
  else
    t := cnt - 1776                                             ' sync with system counter
    repeat ms                                                   ' run delay
      waitcnt(t += MS_001)

  

' ----------------------------------------------------------------------------------------------------------------
' ----------------------------------------------------------------------------------------------------------------
' ----------------------------------------------------------------------------------------------------------------


PUB dec(value)

'' Returns pointer to signed-decimal string

  clrstr(@nstr, MAX_LEN)                                ' clear output string
  return decstr(value)                                  ' return pointer to numeric string
  

' ----------------------------------------------------------------------------------------------------------------

PRI clrstr(strAddr, size)

' Clears string at strAddr
' -- also resets global character pointer (idx)

  bytefill(strAddr, 0, size)                            ' clear string to zeros
  idx~                                                  ' reset index
 
' ----------------------------------------------------------------------------------------------------------------

PRI decstr(value) | div, z_pad   

' Converts value to signed-decimal string equivalent
' -- characters written to current position of idx
' -- returns pointer to nstr

  if (value < 0)                                        ' negative value? 
    -value                                              '   yes, make positive
    nstr[idx++] := "-"                                  '   and print sign indicator

  div := 1_000_000_000                                  ' initialize divisor
  z_pad~                                                ' clear zero-pad flag

  repeat 10
    if (value => div)                                   ' printable character?
      nstr[idx++] := (value / div + "0")                '   yes, print ASCII digit
      value //= div                                     '   update value
      z_pad~~                                           '   set zflag
    elseif z_pad or (div == 1)                          ' printing or last column?
      nstr[idx++] := "0"
    div /= 10 

  return @nstr


' ----------------------------------------------------------------------------------------------------------------
' ----------------------------------------------------------------------------------------------------------------
' ----------------------------------------------------------------------------------------------------------------
' ----------------------------------------------------------------------------------------------------------------
' ----------------------------------------------------------------------------------------------------------------


PUB SoundTest

      ' Use passive button pins are dummy tpin2's

      repeat 3
      
        d.start_up(leftPin, 2)    'Start up object
        'd.tech_support                'tech_support
        d.happy1
        
        pause (500)
        
        d.start_up(rightPin, 4)    'Start up object
        'd.tech_support                'tech_support
        d.happy2
        
        pause(2500)      




DAT

EEpromTestData  byte      $42,$6c,$4f,$63,$4b,$6c,$59,0

                       
' ----------------------------------------------------------------------------------------------------------------