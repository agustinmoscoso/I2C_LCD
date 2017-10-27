/*******************************************************************************
 * Library            : I2C LCD                                                *
 * Author             : gencmucitler                                           *
 * Web                : https://github.com/gencmucitler                        *
 * Creation date      : 02/04/17                                               *
 * Arrangement        : 23/10/17                                               *
 * Version            : 0.4                                                    *
 *                                                                             *
 * PCF8574 input / output port multiplier                                      *
 ******************************************************************************/

#include "mcc_generated_files/mcc.h"
#include "seri_lcd.h"
#include "pcf8574.h"

#ifdef softI2C
#include "softi2c.h"
#endif

//Variables
char display_cursor_blink; //display, cursors and blink open for close commands.

//PORT information
typedef union
{
    //bit level
    struct{
        char P0:1;      //RS
        char P1:1;      //RW
        char P2:1;      //E
        char P3:1;      //back lighting
        char P4:1;      //D4
        char P5:1;      //D5
        char P6:1;      //D6
        char P7:1;      //D7
    }pin;
    //bit level
    struct{
        char RS:1;      //RS
        char RW:1;      //RW
        char E:1;      //E
        char LED:1;      //back lighting
        char D4:1;      //D4
        char D5:1;      //D5
        char D6:1;      //D6
        char D7:1;      //D7       
    }lcd;
    //byte
    char port;
}_PCF_PORT;


_PCF_PORT pcf_port;

/*******************************************************************************
 *  PCF8574 After sending the data over the enable pin high-low makes.         *
 *******************************************************************************/
void pcf8574_write_wEnable() {

    #ifdef softI2C
    
    softi2c_start();
    softi2c_write(pcf_adres <<1);
    softi2c_write(lcd_data);
    pcf_port.lcd.E=1;
    softi2c_write(lcd_data);
    pcf_port.lcd.E=0;
    softi2c_write(lcd_data);
    softi2c_stop();  
    
#else
    
    I2C1_MESSAGE_STATUS status;
    uint8_t writeBuffer[3];
    uint16_t timeOut;

    writeBuffer[0] = lcd_data;
    pcf_port.lcd.E = 1;
    writeBuffer[1] = lcd_data;
    pcf_port.lcd.E = 0;
    writeBuffer[2] = lcd_data;
    
    timeOut = 0;
    while (status != I2C1_MESSAGE_FAIL) {
        
        //send 3 byte
        I2C1_MasterWrite(writeBuffer,
                3,
                pcf_adres,
                &status);

        // Wait until the data is sent.
        while (status == I2C1_MESSAGE_PENDING);

        //Successfully sent 
        if (status == I2C1_MESSAGE_COMPLETE)
            break;

        //Send data again if not sent.
        //If the number of unsuccessful attempts has been reached, get out.
        if (timeOut == pcf_again)
            break;
        else
            timeOut++;
    }
#endif
    
}

/*******************************************************************************
 *  The LCD screen is running. This function must be run first.                *
 *******************************************************************************/
void lcd_start(void) {

    pcf8574_write(0x00);
    lcd_data=0x00;

    lcd_data = 0x20 | (lcd_data & 0x0f); //4 bit mod

    pcf8574_write_wEnable();
    __delay_ms(5);
    
    lcd_command(0x28);
    __delay_us(40);
    lcd_command(0x0C); //display on
    __delay_us(40);
    lcd_command(0x06); //cursor forward
    __delay_ms(2);
        
    lcd_delete();
    lcd_lighting_ac();
}


/*******************************************************************************
 * Go to the desired position.                                                 *
 * Sample: lcd_go(2,5);                                                        *
 *******************************************************************************/
void lcd_go(char line, char column) {
    char temp;
    switch (line) {
        case 1:
            temp = column - 1 + 0x80;
            break;
        case 2:
            temp = column - 1 + 0xc0;
            break;
        case 3:
            temp = (column - 1 + 0x94);
            break;
        case 4:
            temp = (column - 1 + 0xd4);
            break;
    }
    lcd_command(temp);
    __delay_us(100);
}

/*******************************************************************************
 * Writes the text on the screen in the desired position.                      *
 * Sample: lcd_writemessage(1,4,"Trial");                                      *
 *******************************************************************************/
void lcd_writemessage(char line, char column, const char *message) {
    lcd_go(line, column);
    do {
        lcd_writeletter(*message++);
    } while (*message);
}

/*******************************************************************************
 * The LCD writes the text from the last position it was on the screen.        *
 * Sample : lcd_writemessage_cp("Trial");                                      *
 *******************************************************************************/
void lcd_writemessage_cp(const char *message) {
    do {
        lcd_writeletter(*message++);
    } while (*message);
}

/*******************************************************************************
 * The LCD writes the text from the last position it was on the screen...      *
 * The functions lcd_command lcd_writeletter use this function.                *
 *******************************************************************************/
void lcd_busy(void) {
    __delay_us(100); //wait
}

/*******************************************************************************
 * The lcd writes a single letter to the screen.                               *
 * Sample : lcd_writemessage('A');                                             *
 *******************************************************************************/
void lcd_writeletter(char letter) {
    lcd_busy();
    lcd_rs = 1;     //letter

    lcd_data = (lcd_data & 0x0f) | (letter & 0xf0); //Send high data
    pcf8574_write_wEnable();
    __delay_us(200);

    lcd_data = (lcd_data & 0x0f) | (letter << 4); //Send low data
    pcf8574_write_wEnable();
    __delay_us(200);
}


/*******************************************************************************
 * Sends lcd commands.                                                         *
 *******************************************************************************/
void lcd_command(char command) {
    lcd_busy();
    lcd_rs = 0;     //command

    lcd_data = (lcd_data & 0x0f) | (command & 0xf0); //Send high data
    pcf8574_write_wEnable();
    __delay_ms(5);
    
    lcd_data = (lcd_data & 0x0f) | (command << 4); //Send low data
    pcf8574_write_wEnable();
    __delay_ms(5);
}

/*******************************************************************************
 * Lcd creates special characters on the screen.                               *
 * Sample:
 * char heart[8]={
 * 0b00000,
 * 0b01010,
 * 0b10101,
 * 0b10001,
 * 0b01010,
 * 0b00100,
 * 0b00000,
 * 0b00000};
 * 
 * lcd_specialcharacter(0,heart);   //heart symbol on lcd screen.
 * lcd_writeletter(0); // Draws a heart symbol
 *******************************************************************************/
void lcd_specialcharacter(char character, char pattern[8]) {
    char temp, i;
    temp = 0x40 | (character << 3);
    lcd_command(temp);

    for (i = 0; i < 8; i++) {
        lcd_writeletter(pattern[i]);
    }
    lcd_carriageret();
}

/*******************************************************************************
 * Wipe out                                                                    *
 * Sample : lcd_delete();                                                       *
 *******************************************************************************/
void lcd_delete(void) {
    lcd_command(0x01);
}

/*******************************************************************************
 * 1. goes to the 1 st column                                                  *
 *******************************************************************************/
void lcd_carriageret(void) {
    lcd_command(0x02);
}

/*******************************************************************************
 * Makes the cursor active                                                     *
 *******************************************************************************/
void lcd_cursor_ac(void) {
    display_cursor_blink |= 0x02;
    lcd_command(display_cursor_blink);
}

/*******************************************************************************
 * Close the cursor.                                                           *
 *******************************************************************************/
void lcd_cursor_close(void) {
    display_cursor_blink &= 0xfd;
    lcd_command(display_cursor_blink);
}

/*******************************************************************************
 * The LCD Display opens.                                                      *
 *******************************************************************************/
void lcd_display_ac(void) {
    display_cursor_blink |= 0x04;
    lcd_command(display_cursor_blink);

}

/*******************************************************************************
 * Turn off the LCD Display.                                                   *
 *******************************************************************************/
void lcd_display_close(void) {
    display_cursor_blink &= 0xfb;
    lcd_command(display_cursor_blink);
}

/*******************************************************************************
 *The cursor blinks on the display                                             *
 *******************************************************************************/
void lcd_blink_ac(void) {
    display_cursor_blink |= 0x01;
    lcd_command(display_cursor_blink);
}

/*******************************************************************************
 * The cursor turns off the flashing on the display.                           *
 *******************************************************************************/
void lcd_blink_close(void) {
    display_cursor_blink &= 0xfe;
    lcd_command(display_cursor_blink);
}

/*******************************************************************************
 * Slide the LCD screen to the left.                                           *
 *******************************************************************************/
void lcd_left_record(void) {
    lcd_command(0x18);
}

/*******************************************************************************
 * LCD scrolls the screen right.                                               *
 *******************************************************************************/
void lcd_right_record(void) {
    lcd_command(0x1C);
}

/*******************************************************************************
 * 1. goes to line 1                                                           *
 *******************************************************************************/
void lcd_line1(void) {
    lcd_command(0x80);
}

/*******************************************************************************
 * 2. goes to line 2                                                           *
 *******************************************************************************/
void lcd_line2(void) {
    lcd_command(0xc0);
}

/*******************************************************************************
 * 3. goes to line 3                                                           *
 *******************************************************************************/
void lcd_line3(void) {
    lcd_command(0x94);
}

/*******************************************************************************
 * 4. goes to line 4                                                           *
 *******************************************************************************/
void lcd_line4(void) {
    lcd_command(0xd4);
}

/*******************************************************************************
 * For the printf function                                                     *
 *******************************************************************************/
#ifdef lcdprintf_active

void putch(unsigned char byte) {
    lcd_writeletter(byte);
}
#endif

/*******************************************************************************
 * LCD backlight on                                                            *
 *******************************************************************************/
void lcd_lighting_ac(void)
{
    pcf_port.lcd.LED=1;
    pcf8574_write(pcf_port.port);
}

/*******************************************************************************
 * LCD turn off background lighting                                            *
 *******************************************************************************/
void lcd_lighting_close(void)
{
    pcf_port.lcd.LED=0;
    pcf8574_write(pcf_port.port);
}
