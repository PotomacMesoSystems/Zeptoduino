/*
\file	SPIExternalDevice.h
\version	1.0.0
\date	Summer Solstice, year 2012
\purpose	Parent class for SPI external devices.  I'm planning to inherit classes for CC2500 and BMA180 from SPIExternalDevice.
\compiler	Arduino 1.0.1
\author  Nick Alexeev reconvolution@gmail.com.

SPI Master library for Arduino by Cristian Maglie <c.maglie@bug.st> was used as a reference.

This file is free software; you can redistribute it and/or modify it under the terms of either the 
GNU General Public License version 2 or the GNU Lesser General Public License version 2.1, both as
published by the Free Software Foundation.
*/

#ifndef SPIEXTERNALDEVICE_H_INCLUDED
#define SPIEXTERNALDEVICE_H_INCLUDED

#include <Arduino.h>


class SPIExternalDevice
{
public:
	enum SPIMode		{ MODE0 = 0x00, MODE1 = 0x04, MODE2 = 0x08, MODE3 = 0x0C };
	enum SPIClockDiv	{ DIV4 = 0x00, DIV16 = 0x01, DIV64 = 0x02, DIV128 = 0x03, DIV2 = 0x04, DIV8 = 0x05, DIV32 = 0x06 };

	SPIExternalDevice(unsigned char pinCS_n, SPIMode iSPIMode, SPIClockDiv iSPIClockDiv = DIV4, unsigned char uiBitOrder = MSBFIRST);

	static void spiMasterInit();	// initialize the master SPI peripheral on Atmega
	static void spiMasterStop();	// uninitialize

protected:
	inline static void attachInterrupt() { SPCR |= _BV(SPIE); }
	inline static void detachInterrupt() { SPCR &= ~_BV(SPIE); }

	inline static byte spiTransfer(byte bData);

	void spiTransactionBegin();	// Actions needed for beginning a transaction (e.g. assert CS_n)
	void spiTransactionEnd();	// Actions needed for ending a transaction (e.g. deassert CS_n)

	enum Mask
	{
		MODE = 0x0C,	// CPOL = bit 3, CPHA = bit 2 on SPCR
		CLOCK = 0x03,	// SPR1 = bit 1, SPR0 = bit 0 on SPCR
		X2CLOCK = 0x01	// SPI2X = bit 0 on SPSR
	};

	unsigned char	m_pinCS_n;		// Number of the active-low slave select pin (CS#).
	SPIMode			m_iSPIMode;		// SPI mode (this external device)
	SPIClockDiv		m_iSPIClockDiv;	// SPI clock divider (this external device)
	unsigned char	m_iBitOrder;	// LSBFIRST of MSBFIRST
};


byte SPIExternalDevice::spiTransfer(byte bData)
{
  SPDR = bData;
  while ( !(SPSR & _BV(SPIF)) ) { ; }
  return SPDR;
}


#endif