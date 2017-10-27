/*******************************************************************************
 * Library            : I2C LCD                                                *
 * Author             : gencmucitler                                           *
 * Web                : https://github.com/gencmucitler                        *
 * Creation date      : 02/04/17                                               *
 * Arrangement        : 23/10/17                                               *
 * Versiyon           : 0.4                                                    *
 *                                                                             *
 * PCF8574 input / output port multiplier                                      *
 ******************************************************************************/

// The pcf_adres value and the chip in address code should be entered. Address information from Datasheet
// The address codes of PCF8574 and PCF8574A are different. In this way
// 8 PCF8574 and 8 PCF8574A can be connected on the same line. So in total
// 16 port replicators can be connected.
//v0.2 softI2C has been edited so that it can work in the background.


#ifndef PCF8574_H
#define	PCF8574_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#ifndef pcf_adres
#define pcf_adres   0x3F    //Slave address
#endif

//#define softI2C             //Use the software I2C functions.

#define pcf_again   10     //number of retries in missed submission

//Function prototypes

void pcf8574_write(char data);


#endif	/* PCF8574_H */

