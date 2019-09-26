/******************************************************************************/
/*                                                                            */
/* EEPROM.h --  EEPROM I2C driver source                                        */
/*                                                                            */
/******************************************************************************/
/* Author: Ana-Maria-Eliza Balas                                              */
/*		   ana-maria.balas@digilent.ro										  */
/* Copyright 2019, Digilent Inc.                                              */
/*                                                                            */
/******************************************************************************/
/* Module Description:                                                        */
/*                                                                            */
/* This file contains the functions declarations of EEPROM.c for              */
/* communication over I2C with the Atmel® AT24C04D EEPROM memory chip.        */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    09/23/2019(anamariabalas):   Created                                    */
/*    09/23/2019(anamariabalas): Validated for Vivado 2019.1                  */
/*                                                                            */
/******************************************************************************/

#ifndef EPROM_H
#define EPROM_H


/* -------------------------------------------------------------------------- */
/*					Include Files    						                  */
/* -------------------------------------------------------------------------- */

#include "xil_types.h"
#include "xstatus.h"
#include "xiic_l.h"
#include "xiic.h"
#include "xparameters.h"



#ifdef XPAR_XINTC_NUM_INSTANCES
 #include "xintc.h"
 #define INTC		XIntc
 #define INTC_HANDLER	XIntc_InterruptHandler
#else
#ifdef XPAR_SCUGIC_0_DEVICE_ID
 #include "xscugic.h"
 #define INTC		XScuGic
 #define INTC_HANDLER	XScuGic_InterruptHandler
#else
#define NO_IRPT 1
#endif
#endif



/* ------------------------------------------------------------ */
/*		7 bit Chip Address		                                */
/* ------------------------------------------------------------ */

#define EPROM_CHIP_ADDRESS  0x50    // The LSB bit of the EPROM_CHIP_ADDRESS is the MSB
									 // of the 9-bit memory array word address

/* ------------------------------------------------------------ */
/*		EEPROM Address		                                    */
/* ------------------------------------------------------------ */
#define ADR_EPROM_SERIALNO 	0					//Serial Number  base address : 0x00h
#define ADR_EPROM_CALIB     16					//User calibration base address : 0x10
#define ADR_EPROM_FACTCALIB	32					//Factory calibration base address : 0x20




/* ------------------------------------------------------------ */
/*				Constants  						            	*/
/* ------------------------------------------------------------ */

#define bool u8
#define true 1
#define false 0

/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

typedef struct Eprom{
	XIic EPROMIic;
	u8 chipAddr;
}EPROM;

void EPROM_begin(EPROM* InstancePtr, u32 IIC_Address, u8 Chip_Address);
uint8_t EPROM_PageRead(EPROM* InstancePtr, u16 addr, u8* data, u8 cntdata);
uint8_t EPROM_PageWrite(EPROM* InstancePtr, u16 addr, u8* data, u8 cntdata);


#endif // EPROM_H
