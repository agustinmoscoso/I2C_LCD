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


// The pcf_adres value and the chip in address code should be entered. Address information is on the Datasheet
// The address codes of PCF8574 and PCF8574A are different. In this way
// PCF8574 and PCF8574A can be connected on the same line. So in total
// 16 port replicators can be connected.

//v0.4 softI2C has been edited so that it can work in the background.

#include "mcc_generated_files/mcc.h"
#include "pcf8574.h"

#ifdef softI2C
#include "softi2c.h"
#endif

void pcf8574_write(char data) {
    
#ifdef softI2C
    
    softi2c_start();
    softi2c_write(pcf_adres <<1);
    softi2c_write(data);
    softi2c_stop();   
#else
    
    I2C1_MESSAGE_STATUS status;
    uint8_t writeBuffer[1];
    uint16_t timeOut;

    writeBuffer[0] = data;
    timeOut = 0;
    while (status != I2C1_MESSAGE_FAIL) {
        
        //Send 1 byte
        I2C1_MasterWrite(writeBuffer,
                1,
                pcf_adres,
                &status);

        // Wait until the data is sent.
        while (status == I2C1_MESSAGE_PENDING);

        //Successfully sent 
        if (status == I2C1_MESSAGE_COMPLETE)
            break;

        // Send data again if not sent.
        // Exit if the number of failed repetitions has been reached.
        if (timeOut == pcf_again)
            break;
        else
            timeOut++;
    }
#endif

}

