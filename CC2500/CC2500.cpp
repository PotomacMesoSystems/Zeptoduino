/*!
 *  \file    CC2500.cpp
 *  \version 1.1
 *  \date    Dec 23, 2011
 *	\purpose Low level library for CC2500.  Anaren A2500R24A was used as the test hardware, although the library is generic.
 *	\compiler	Arduino 1.0.1
 *  \author  Nick Alexeev reconvolution@gmail.com.  Based on George Mathijssen, george.knutsel@gmail.com
 *
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
}

void CC2500xcvr::reset()
// REFERENCES:	19.1.2 "Manual Reset" in [1]
{
    // enable device
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
}

unsigned char CC2500xcvr::sendByte(unsigned char data)
{
    spiTransactionBegin();	// enable device
    unsigned char result = spiTransfer(data);	// send byte
    spiTransactionEnd(); 	// disable device
    return result;
}

unsigned char CC2500xcvr::sendCommand(unsigned char command, unsigned char data)
{
	spiTransactionBegin();	// enable device
    spiTransfer(command);	// send command byte
    unsigned char result = spiTransfer(data);	// send data byte
    spiTransactionEnd(); 	// disable device
    return result;		// return result
}

unsigned char CC2500xcvr::sendStrobeCommand(unsigned char command)
{
    return sendByte(command);	// send command
}

unsigned char CC2500xcvr::sendBurstCommand(unsigned char command, unsigned char* data, unsigned char length)
{
    spiTransactionBegin();	// enable device

    // send command byte
    spiTransfer(command);			    // this is a burst command
    unsigned char result = 0;

    // send/recv data bytes
    for (int i=0; i<length; ++i)
	{
        result = spiTransfer(data[i]);	// send
        data[i] = result;				// receive into the same buffer
    }

    spiTransactionEnd(); 	// disable device
    return result;	// return result
}

