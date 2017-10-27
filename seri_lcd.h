/*******************************************************************************
 * Library            : I2C LCD                                                *
 * Author             : gencmucitler                                           *
 * Web                : https://github.com/gencmucitler                        *
 * Creation date      : 02/04/17                                               *
 * Arrangement        : 23/10/17                                               *
 * Version            : 0.4                                                    *
 *                                                                             *
 * PCF8574 module and I2C Lcd usage library                                    *
 ******************************************************************************/

//v0.4 SoftI2C has been edited to work with.

/*      Standard LCD pin connections
1: Gnd  2: Vcc  3: Vee - contrast pin
4: RS   5: RW   6: E
7: D0   8: D1   9: D2   10: D3  11: D4  12: D5  13: D6  14: D7
15: Backlight Vcc       16: Backlight Gnd                                     */

/*LCD display connection of PCF8574A integrated
 * 
 * P0: RS
 * P1: RW
 * P2: E
 * P3: lighting
 * P4: D4
 * P5: D5
 * P6: D6
 * P7: D7
 */

#define pcf_adres   0x3F    //i2c slave address
//#define softI2C           //Use the software I2C functions.

//Will the printf function write to the lcd?
#define lcdprintf_active

// lcd pin to be used?
#define lcd_data	pcf_port.port

#define lcd_rs          pcf_port.lcd.RS
#define lcd_e           pcf_port.lcd.E
#define lcd_rw           pcf_port.lcd.RW


//function prototypes
void lcd_start(void);
void lcd_go(char line, char column);
void lcd_writemessage(char line, char column, const char *message);
void lcd_writemessage_cp(const char *message);
void lcd_busy(void);
void lcd_writeletter(char letter);
void lcd_specialcharacter(char character,char pattern[8]);
void lcd_command(char command);
void lcd_delete(void);
void lcd_carriageret(void);
void lcd_cursor_ac(void);
void lcd_cursor_close(void);
void lcd_display_ac(void);
void lcd_display_close(void);
void lcd_blink_ac(void);
void lcd_blink_close(void);
void lcd_left_record(void);
void lcd_right_record(void);
void lcd_line1(void);
void lcd_line2(void);
void lcd_line3(void);
void lcd_line4(void);
void lcd_lighting_ac(void);
void lcd_lighting_close(void);
#ifdef lcdprintf_active
void putch(unsigned char byte);
#endif
