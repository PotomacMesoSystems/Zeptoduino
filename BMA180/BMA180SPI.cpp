/*
\file	BMA180SPI.cpp
\version	1.0.0
\date	June 26, 2012
\purpose	Driver for SPI-connected BMA180.
\compiler	Arduino 1.0.1
\author  Nick Alexeev reconvolution@gmail.com.

This file is free software; you can redistribute it and/or modify it under the terms of either the 
GNU General Public License version 2 or the GNU Lesser General Public License version 2.1, both as
published by the Free Software Foundation.
*/

/* References
[1]  BMA180 datasheet  rev 2.5,  Dec 07 2010  (Dear Bosch, when you get around to it, please  write a more thorogh datasheet for this chip.)
*/

#include <Arduino.h>	// Arduino compiler 1.0 uses "Arduino.h" instead of "WConstants.h" or "wiring.h"
#include <SPIExternalDevice.h>
#include "BMA180SPI.h"


BMA180AccelerometerSPI::BMA180AccelerometerSPI(unsigned char pinCS_n, SPIClockDiv iSPIClockDiv)
	: SPIExternalDevice(
		pinCS_n,
		SPIExternalDevice::MODE0,		// Ch. 8.4.1 in [1] suggests SPI mode 2.  But mode 2 didn't work for me.  Mode 0 works.
		iSPIClockDiv)
{
}

BMA180AccelerometerSPI::~BMA180AccelerometerSPI()
{
}


byte BMA180AccelerometerSPI::readByte(byte iRegAddr)
{
	byte iRegContents;

	spiTransactionBegin();
	spiTransfer(_BV(RW_FLAG) | iRegAddr);
	iRegContents = spiTransfer(0x55);	// 0x55 is an arbitrary dummy
	spiTransactionEnd();

	return iRegContents;
}


void BMA180AccelerometerSPI::writeByte(byte iRegAddr, byte iNewRegContents)
{
	spiTransactionBegin();
	spiTransfer(iRegAddr);
	spiTransfer(iNewRegContents);
	spiTransactionEnd();
}


void BMA180AccelerometerSPI::writeRegisterBit(Registers iRegAddr, RegisterBits iBitNumber, bool bBitValue)
// PURPOSE:		Modify the bit in the BMA180 register.  Read-modify-write.
// PRECONDITIONS:	SPI master on the AVR has been initialized
{
	byte	iRegVal;

	iRegVal = readByte(iRegAddr);
	iRegVal = (bBitValue) ? (iRegVal | _BV(iBitNumber)) : (iRegVal & ~_BV(iBitNumber));
	writeByte(iRegAddr, iRegVal);
}


int BMA180AccelerometerSPI::readAcceleration(byte iAxis)
// PRECONDITIONS:	BMA180 is configured for 14-bit readings
{
	byte cLSByte = readByte(REG_ACC_LSB + 2*iAxis);
	byte cMSByte = readByte(REG_ACC_MSB + 2*iAxis);

	//* <debug/> */ Serial.print(cMSByte);   Serial.print(" ");
	
	int iAccel;		// return value
	iAccel = cMSByte;
	iAccel <<= 8;
	iAccel |= cLSByte;
	iAccel >>= 2;
	return iAccel;	// stub
}


void BMA180AccelerometerSPI::resetInterrupt()
{
	writeRegisterBit(CTRL_REG0, REG_BIT_RESET_INT, 1);
}


void BMA180AccelerometerSPI::softReset()
// See 7.10.6
{
	const byte SOFT_RESET_CODE = 0xB6;
	writeByte(SOFT_RESET, SOFT_RESET_CODE);
	delay(10);	// 1ms delay
}