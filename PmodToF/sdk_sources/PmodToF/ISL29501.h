
/******************************************************************************/
/*                                                                            */
/* ISL29501.h --  ISL29501 I2C driver source                                  */
/*                                                                            */
/******************************************************************************/
/* Author: Ana-Maria-Eliza Balas                                              */
/*		   ana-maria.balas@digilent.ro										  */
/* Copyright 2019, Digilent Inc.                                              */
/*                                                                            */
/******************************************************************************/
/* Module Description:                                                        */
/*                                                                            */
/* This file contains the functions declarations of ISL29501.c for            */
/* communication over I2C with the ISL29501 chip.                             */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    09/23/2019(anamariabalas):   Created                                    */
/*    09/23/2019(anamariabalas): Validated for Vivado 2019.1                  */
/*                                                                            */
/******************************************************************************/



#ifndef ISL29501_H
#define ISL29501_H


/* -------------------------------------------------------------------------- */
/*					Include Files    						                  */
/* -------------------------------------------------------------------------- */
#include "xil_types.h"
#include "xstatus.h"
#include "xiic_l.h"
#include "xiic.h"
#include "xparameters.h"


#define bool u8
#define true 1
#define false 0

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
/*		7 bit Chip Address		*/
/* ------------------------------------------------------------ */
#define ISL29501_CHIP_ADDRESS  0x57

/* ------------------------------------------------------------ */
/*		 Register addresses Definitions	for calibration			*/
/* ------------------------------------------------------------ */
#define CROSSTALK_I_EXPONENT 0x24
#define CROSSTALK_I_MSB 0x25
#define CROSSTALK_I_LSB 0x26
#define CROSSTALK_Q_EXPONENT 0x27
#define CROSSTALK_Q_MSB 0x28
#define CROSSTALK_Q_LSB 0x29
#define CROSSTALK_GAIN_MSB 0x2A
#define CROSSTALK_GAIN_LSB 0x2B
#define MAGNITUDE_REFERENCE_EXP 0x2C
#define MAGNITUDE_REFERENCE_MSB 0x2D
#define MAGNITUDE_REFERENCE_LSB 0x2E
#define PHASE_OFFSET_MSB 0x2F
#define PHASE_OFFSET_LSB 0x30

/* ------------------------------------------------------------ */
/*		 Register addresses Definitions	for readout			*/
/* ------------------------------------------------------------ */
#define DEVICE_ID 0x00
#define STATUS_REGISTERS 0x02
#define DISTANCE_READOUT_MSB_REG 0xD1
#define DISTANCE_READOUT_LSB_REG 0xD2

/* ------------------------------------------------------------ */
/*				Bit masks Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

typedef struct ISL29501{
	XIic ISL29501Iic;
	u8 chipAddr;
	u8 currentRegister;
	u8 recvbytes;
	u8* recv;
#ifndef NO_IRPT
	INTC intc;
#endif
}ISL29501;

void ISL29501_begin(ISL29501* InstancePtr, u32 IIC_Address, u8 Chip_Address);
u8 ISL29501_ReadIIC(ISL29501* InstancePtr, u8 reg, u8 *Data, int nData);
u8 ISL29501_WriteIIC(ISL29501* InstancePtr, u8 reg, u8 *Data, int nData);



#endif // ISL29501_H
