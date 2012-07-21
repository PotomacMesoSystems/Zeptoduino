/*!
 *  \file    CC2500.cpp
 *  \version 1.1
 *  \date    Dec 23, 2011
<<<<<<< HEAD
 *  \author  Nick Alexeev reconvolution@gmail.com.  Based on George Mathijssen, george.knutsel@gmail.com
 *	\purpose Low level library for CC2500.  Anaren A2500R24A was used as the test hardware, although the library is generic.
=======
 *	\purpose Low level library for CC2500.  Anaren A2500R24A was used as the test hardware, although the library is generic.
 *	\compiler	Arduino 1.0.1
 *  \author  Nick Alexeev reconvolution@gmail.com.  Based on George Mathijssen, george.knutsel@gmail.com
 *
>>>>>>> refactoring.  new driver for BMA180.
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  \internal
 *    Created: 02-10-2008
 */

/*	References:
[1]	CC2500 datasheet.  Texas Instruments SWRS040C.
*/

<<<<<<< HEAD
#include "CC2500.h"
#include <inttypes.h>
#include "Arduino.h"	// Arduino compiler 1.0 uses "Arduino.h" instead of "WConstants.h" or "wiring.h"

CC2500xcvr::CC2500xcvr(unsigned char pinCS, unsigned char pinSCK, unsigned char pinMOSI, unsigned char pinMISO)
    : m_pinCS(pinCS)
    , m_pinSCK(pinSCK)
    , m_pinMOSI(pinMOSI)
    , m_pinMISO(pinMISO)
{
}

CC2500xcvr::~CC2500xcvr()
{
}

void CC2500xcvr::init()
{
    // setup pin mode
    pinMode(m_pinMOSI, OUTPUT);
    pinMode(m_pinMISO, INPUT);
    pinMode(m_pinSCK, OUTPUT);
    pinMode(m_pinCS, OUTPUT);

    // disable device
    digitalWrite(m_pinCS, HIGH);

    // setup SPI control register: SPCR = 01010000
    // interrupt disabled, spi enabled, msb 1st, master, clk low when idle,
    // sample on rising edge of clk, system clock rate fastest
    SPCR = (1<<SPE) | (1<<MSTR);

    // clear data registers
    byte clr = SPSR;
    clr = SPDR;
=======
#include <Arduino.h>	// Arduino compiler 1.0 uses "Arduino.h" instead of "WConstants.h" or "wiring.h"
#include <SPIExternalDevice.h>
#include "CC2500.h"


CC2500xcvr::CC2500xcvr(unsigned char pinCS_n, SPIClockDiv iSPIClockDiv)
	: SPIExternalDevice(
		pinCS_n, 
		SPIExternalDevice::MODE0,		// sclk low when idle (CPOL=0), sample on rising edge of sclk (CPHA=0).  This is  SPI Mode 0.
		iSPIClockDiv)
{
}

CC2500xcvr::~CC2500xcvr()
{
>>>>>>> refactoring.  new driver for BMA180.
}

void CC2500xcvr::reset()
// REFERENCES:	19.1.2 "Manual Reset" in [1]
{
    // enable device
<<<<<<< HEAD
    digitalWrite(m_pinCS, LOW);
    delayMicroseconds(1);

    // disable device and wait at least 40 microseconds
    digitalWrite(m_pinCS, HIGH);
    delayMicroseconds(41);

    // enable device
    digitalWrite(m_pinCS, LOW);

    // wait for device
    while (digitalRead(m_pinMISO) == HIGH) {
    };

    // send reset command (SRES)
    spiTransfer(0x30);

    // disable device
    digitalWrite(m_pinCS, HIGH);
=======
    digitalWrite(m_pinCS_n, LOW);
    delayMicroseconds(1);

    // disable device and wait at least 40 microseconds
    digitalWrite(m_pinCS_n, HIGH);
    delayMicroseconds(41);
    
	spiTransactionBegin();	// enable device
    spiTransfer(0x30);	// send reset command (SRES)
    spiTransactionEnd(); 	// disable device
}

void CC2500xcvr::spiTransactionBegin()
{
	SPIExternalDevice::spiTransactionBegin();
	while ( digitalRead(MISO) == HIGH ) {;}	// wait for device
>>>>>>> refactoring.  new driver for BMA180.
}

unsigned char CC2500xcvr::sendByte(unsigned char data)
{
<<<<<<< HEAD
    // enable device
    digitalWrite(m_pinCS, LOW);

    // wait for device
    while (digitalRead(m_pinMISO) == HIGH) {
    };

    // send byte
    unsigned char result = spiTransfer(data);

    // disable device
    digitalWrite(m_pinCS, HIGH);

    // return result
=======
    spiTransactionBegin();	// enable device
    unsigned char result = spiTransfer(data);	// send byte
    spiTransactionEnd(); 	// disable device
>>>>>>> refactoring.  new driver for BMA180.
    return result;
}

unsigned char CC2500xcvr::sendCommand(unsigned char command, unsigned char data)
{
<<<<<<< HEAD
    // enable device
    digitalWrite(m_pinCS, LOW);

    // wait for device
    while (digitalRead(m_pinMISO) == HIGH) {
    };

    // send command byte
    spiTransfer(command);

    // send data byte
    unsigned char result = spiTransfer(data);

    // disable device
    digitalWrite(m_pinCS, HIGH);

    // return result
    return result;
=======
	spiTransactionBegin();	// enable device
    spiTransfer(command);	// send command byte
    unsigned char result = spiTransfer(data);	// send data byte
    spiTransactionEnd(); 	// disable device
    return result;		// return result
>>>>>>> refactoring.  new driver for BMA180.
}

unsigned char CC2500xcvr::sendStrobeCommand(unsigned char command)
{
<<<<<<< HEAD
    // send command
    return sendByte(command);
=======
    return sendByte(command);	// send command
>>>>>>> refactoring.  new driver for BMA180.
}

unsigned char CC2500xcvr::sendBurstCommand(unsigned char command, unsigned char* data, unsigned char length)
{
<<<<<<< HEAD
    // enable device
    digitalWrite(m_pinCS, LOW);

    // wait for device
    while (digitalRead(m_pinMISO) == HIGH) {
    };

    // send command byte
    spiTransfer(command);			    // this is a burst command

    unsigned char result = 0;

    // send/recv data bytes
    for (int i=0; i<length; ++i) {
=======
    spiTransactionBegin();	// enable device

    // send command byte
    spiTransfer(command);			    // this is a burst command
    unsigned char result = 0;

    // send/recv data bytes
    for (int i=0; i<length; ++i)
	{
>>>>>>> refactoring.  new driver for BMA180.
        result = spiTransfer(data[i]);	// send
        data[i] = result;				// receive into the same buffer
    }

<<<<<<< HEAD
    // disable device
    digitalWrite(m_pinCS, HIGH);

    // return result
    return result;
}

unsigned char CC2500xcvr::spiTransfer(volatile unsigned char data)
{
    // start transmission
    SPDR = data;

    // wait for end of transmission
    while (!(SPSR & (1<<SPIF))) {
    };

    // return received byte
    return SPDR;
}
=======
    spiTransactionEnd(); 	// disable device
    return result;	// return result
}

>>>>>>> refactoring.  new driver for BMA180.
