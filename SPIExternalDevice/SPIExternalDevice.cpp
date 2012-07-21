/*
\file	SPIExternalDevice.cpp
\version	1.0
\date	Summer Solstice, year 2012
\purpose	Parent class for SPI external devices.  I'm planning to inherit classes for CC2500 and BMA180 from SPIExternalDevice.
\compiler	Arduino 1.0.1
\author  Nick Alexeev reconvolution@gmail.com.

SPI Master library for Arduino by Cristian Maglie <c.maglie@bug.st> was used as a reference and as a source of code.

This file is free software; you can redistribute it and/or modify it under the terms of either the 
GNU General Public License version 2 or the GNU Lesser General Public License version 2.1, both as
published by the Free Software Foundation.
*/


#include <Arduino.h>
#include "SPIExternalDevice.h"


SPIExternalDevice::SPIExternalDevice(unsigned char pinCS_n, SPIMode iSPIMode, SPIClockDiv iSPIClockDiv, unsigned char uiBitOrder)
{
	m_pinCS_n = pinCS_n;
	pinMode(m_pinCS_n, OUTPUT);  digitalWrite(m_pinCS_n, HIGH);

	m_iSPIMode = iSPIMode;
	m_iSPIClockDiv = iSPIClockDiv;
	m_iBitOrder = uiBitOrder;
}


void SPIExternalDevice::spiMasterInit()
{
  // Set direction register for SCK and MOSI pin.
  // MISO pin automatically overrides to INPUT.
  // When the SS pin is set as OUTPUT, it can be used as
  // a general purpose output port (it doesn't influence
  // SPI operations).

  pinMode(SCK, OUTPUT);		digitalWrite(SCK, LOW);
  pinMode(MOSI, OUTPUT);	digitalWrite(MOSI, LOW);
  pinMode(SS, OUTPUT);		digitalWrite(SS, HIGH);
  
  // Warning: if the SS pin ever becomes a LOW INPUT then SPI 
  // automatically switches to Slave, so the data direction of 
  // the SS pin MUST be kept as OUTPUT.
  SPCR |= _BV(MSTR);
  SPCR |= _BV(SPE);

  // clear data registers
  byte b = SPSR;
  b = SPDR;
}


void SPIExternalDevice::spiMasterStop()
{
  SPCR &= ~_BV(SPE);
}


void SPIExternalDevice::spiTransactionBegin()
{
	// 1. make sure that SPI parameters are set for this particular external device (i.e. instance of a subclass)
	
	// 1.a SPI mode
	SPCR = (SPCR & ~MODE) | m_iSPIMode;

	// 1.b order of bits
	if(m_iBitOrder == LSBFIRST)
		SPCR |= _BV(DORD);
	else
		SPCR &= ~(_BV(DORD)); //*/

	// 1.c SPI clock divider
	SPCR = (SPCR & ~CLOCK)	| ( m_iSPIClockDiv & CLOCK);
	SPSR = (SPSR & ~X2CLOCK) | ((m_iSPIClockDiv >> 2) & X2CLOCK);

	// 2. assert CS_n
	digitalWrite(m_pinCS_n, LOW);
}


void SPIExternalDevice::spiTransactionEnd()
{
	digitalWrite(m_pinCS_n, HIGH);	// de-assert CS_n
}