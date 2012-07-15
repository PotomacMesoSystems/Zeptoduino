/*
\file	BMA180SPI.h
\version	1.0.0
\date	June 26, 2012
\purpose	Driver for SPI-connected BMA180.
\compiler	Arduino 1.0.1
\author  Nick Alexeev reconvolution@gmail.com.

This file is free software; you can redistribute it and/or modify it under the terms of either the 
GNU General Public License version 2 or the GNU Lesser General Public License version 2.1, both as
published by the Free Software Foundation.
*/

/* References: see BMA180SPI.cpp */

#ifndef BMA180SPI_H_INCLUDED
#define BMA180SPI_H_INCLUDED


class BMA180AccelerometerSPI : public SPIExternalDevice
{
public:
	BMA180AccelerometerSPI(
		unsigned char pinCS_n, 
		SPIExternalDevice::SPIClockDiv iSPIClockDiv = SPIExternalDevice::DIV4);

	~BMA180AccelerometerSPI();

	byte readByte(byte iRegAddr);
	void writeByte(byte iRegAddr, byte iNewRegContents);

	enum Axes	{ X_AXIS = 0, Y_AXIS = 1, Z_AXIS = 2 };

	enum Registers		// addresses of the internal registers inside BMA180
	{
		REG_GAIN_T			= 0x31,
		REG_SLOPE_TH		= 0x2B,
		REG_TAPSENS_TH		= 0x28,
		REG_HIGH_DUR		= 0x27,
		CTRL_REG3			= 0x21,
		SOFT_RESET			= 0x10,
		CTRL_REG0			= 0x0D,
		REG_ACC_MSB			= 0x03,
		REG_ACC_LSB			= 0x02,
		REG_CHIP_MODEL_ID	= 0x00
	};

	enum RegisterBits	// bit numbers 
	{
		REG_BIT_EE_W			= 4,
		REG_BIT_DIS_I2C			= 0,
		REG_BIT_RESET_INT		= 6,
		
		REG_BIT_LOW_INT			= 4,
		REG_BIT_TAPSENSE_INT	= 3,
		REG_BIT_ADV_INT			= 2,
		REG_BIT_NEW_DATA_INT	= 1,
		REG_BIT_LAT_INT			= 0
	};
	/* TODO:	Bit is always in the same register.  It makes sense to combine them in a structure.
				typedef struct {Register r, Bit b} RegisterBit;  const RegisterBit = {REG_HIGH_DUR, REG_BIT_DIS_I2C};  */

	void writeRegisterBit(Registers iRegAddr, RegisterBits iBitNumber, bool bBitValue);
	signed int readAcceleration(byte iAxis);
	void resetInterrupt();
	void softReset();
	
	static const byte CHIP_MODEL_ID = 0x03;		// Value of chip model ID, which is hard-wired in the silicon.  It can be used for checking the SPI wiring.

protected:
	static const byte RW_FLAG = 7;		// R/W# flag.  Set for reading, clear for writing.  7th bit, don't confuse with flag
};

#endif