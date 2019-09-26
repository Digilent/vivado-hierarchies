/******************************************************************************/
/*                                                                            */
/* PmodToF.h -- PmodToF Driver Source                                         */
/*                                                                            */
/******************************************************************************/
/* Author: Ana-Maria-Eliza Balas                                              */
/*		   ana-maria.balas@digilent.ro										  */
/* Copyright 2019, Digilent Inc.                                              */
/*                                                                            */
/******************************************************************************/
/* Module Description:                                                        */
/*                                                                            */
/* This file contain functions declarations for PmodToF.c source file.        */
/* These functions configure and initialize the Pmod ToF device over I2C and  */
/* access Pmod ToF functionality such as performing calibrations and 		  */
/* measurements, R/W operations from EEPROM and from ISL29501 registers.      */
/* In order to implement the I2C communication, the PmodToF module accesses   */
/* functions implemented in ISL29501.h and EEPROM.h files.                    */
/* This file contains declarations of library functions for performing        */
/* calibration and measurement.                                               */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    09/23/2019(anamariabalas):   Created                                    */
/*    09/23/2019(anamariabalas): Validated for Vivado 2019.1                  */
/*                                                                            */
/******************************************************************************/



#ifndef PmodToF_H
#define PmodToF_H

/* -------------------------------------------------------------------------- */
/*					Include Files    						                  */
/* -------------------------------------------------------------------------- */
#include "xil_types.h"
#include "xstatus.h"
#include "xiic_l.h"
#include "xiic.h"
#include "xparameters.h"


/* ------------------------------------------------------------ */
/*		7 bit Chip Addresses		                            */
/* ------------------------------------------------------------ */

//EEPROM chip address
#define EPROM_CHIP_ADDRESS     0x50    // The LSB bit of the EPROM_CHIP_ADDRESS is the MSB
									 // of the 9-bit memory array word address
// ISL29501 chip address
#define ISL29501_CHIP_ADDRESS  0x57

/* ------------------------------------------------------------ */
/*		EEPROM Address		                                    */
/* ------------------------------------------------------------ */
#define ADR_EPROM_SERIALNO 	0					//Serial Number  base address : 0x00h
#define ADR_EPROM_CALIB     16					//User calibration base address : 0x10
#define ADR_EPROM_FACTCALIB	32					//Factory calibration base address : 0x20

/* ------------------------------------------------------------ */
/*		Global variables	                                    */
/* ------------------------------------------------------------ */
char szMsg[400];

/* ------------------------------------------------------------ */
/*				Constants						            	*/
/* ------------------------------------------------------------ */

#define EPROM_MAGIC_NO 0xEB
#define SERIALNO_SIZE	12
#define ADR_OFFSET_CALIB_REG_ISL29501 0x24			//ISL29501s offset calibration register;
												//there are 13 calibration registers: from 0x24 to 0x30
#define bool u8
#define true 1
#define false 0

/* ------------------------------------------------------------ */
/*				ERROR Definitions							    */
/* ------------------------------------------------------------ */

#define ERRVAL_SUCCESS                  0       // success

#define ERRVAL_EPROM_WRITE              0xFA    // failed to write EPROM over I2C communication
#define ERRVAL_EPROM_READ               0xF9    // failed to read EPROM over I2C communication
#define ERRVAL_ToF_WRITE              	0xF8    // failed to write ISL29501 registers over I2C communication
#define ERRVAL_ToF_READ              	0xF6    // failed to read ISL29501 registers over I2C communication

#define ERRVAL_EPROM_CRC                0xFE    // wrong CRC when reading data from EPROM
#define ERRVAL_EPROM_MAGICNO            0xFD    // wrong Magic No. when reading data from EPROM
#define ERRVAL_FAILED_STARTING_CALIB    0xFC    // failed to start calibration, EEPROM or ISL29501 device is busy
#define ERRVAL_INCORRECT_CALIB_DISTACE  0xED	// incorrect calibration distance; it has to be more than 5 cm(0.05 m)
#define ERRVAL_FAILED_STARTING_MEASURE	0xEC	// failed to start measurement


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
/*		 Register addresses Definitions	for readout			    */
/* ------------------------------------------------------------ */
#define STATUS_REGISTERS 0x02
#define DISTANCE_READOUT_MSB_REG 0xD1
#define DISTANCE_READOUT_LSB_REG 0xD2


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

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef struct _CALIBDATA{    //
    uint8_t magic;
    uint8_t  regs[13];    // 13 registers
    uint8_t crc;
    uint8_t dummy;	// align to 16 bytes
}  __attribute__((__packed__)) CALIBDATA;


typedef struct _SERIALNODATA{    //
    uint8_t magic;
    char rgchSN[SERIALNO_SIZE];	// 12 chars string
    uint8_t crc;
    uint8_t dummy[2];	// align to 16 bytes
}  __attribute__((__packed__)) SERIALNODATA;


/* ------------------------------------------------------------ */
/*					Public functions    						    */
/* ------------------------------------------------------------ */

void PmodToF_Initialize();
double PmodToF_perform_distance_measurement();
uint8_t PmodToF_start_calibration(double actual_distance);
uint8_t PmodToF_WriteCalibsToEPROM_User();
uint8_t PmodToF_ReadCalibsFromEPROM_User();
uint8_t PmodToF_RestoreAllCalibsFromEPROM_Factory();
uint8_t PmodToF_ReadSerialNoFromEPROM(char *pSzSerialNo);


#endif // PmodToF_H
