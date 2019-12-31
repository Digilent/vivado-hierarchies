/******************************************************************************/
/*                                                                            */
/* PmodDA4.h -- Declaration for PmodDA4 library                               */
/*                                                                            */
/******************************************************************************/
/* Author: Jon Peyron                                                         */
/* Copyright 2017, Digilent Inc.                                              */
/******************************************************************************/
/* File Description:                                                          */
/*                                                                            */
/* This file declares the DA4 library functions.                              */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    06/20/2016(JonP):     Created                                           */
/*    04/28/2017(ArtVVB):   Validated                                         */
/*    02/13/2018(atangzwj): Validated for Vivado 2017.4                       */
/*                                                                            */
/******************************************************************************/

#ifndef PMODDA4_H
#define PMODDA4_H

/****************** Include Files ********************/

#include "xil_types.h"
#include "xspi.h"
#include "xspi_l.h"
#include "xstatus.h"

/* ------------------------------------------------------------ */
/*                  Definitions                                 */
/* ------------------------------------------------------------ */

#define CH_A              0  // Select DAC A to be written
#define CH_B              1  // Select DAC B to be written
#define CH_C              2  // Select DAC C to be written
#define CH_D              3  // Select DAC D to be written
#define CH_E              4  // Select DAC E to be written
#define CH_F              5  // Select DAC F to be written
#define CH_G              6  // Select DAC G to be written
#define CH_H              7  // Select DAC H to be written
#define ALL_CH         0x0F  // Select all DACs to written
/*                                     AD5628 Input Register */

/* AD5628 COMMAND options */
/*										value   |LSB offset | no. of bits  */
#define DA4_CMD_WRITE_INPUT_N             0x00,      24,       4 // Write to Input Register n.
#define DA4_CMD_UPDATE_DAC_N              0x01,      24,       4 // Update DAC Register n.
#define DA4_CMD_WRITE_INPUT_N_UPDATE_ALL  0x02,      24,       4 // Write to Input Register n, update all.
#define DA4_CMD_WRITE_INPUT_N_UPDATE_N    0x03,      24,       4 // Write to and update DAC Channel n.
#define DA4_CMD_POWERDOWN                 0x04,      24,       4 // Power down/power up DAC.
#define DA4_CMD_LOAD_CLEAR_CODE           0x05,      24,       4 // Load clear code register.
#define DA4_CMD_LOAD_LDAC_REG             0x06,      24,       4 // Load LDAC register.
#define DA4_CMD_RESET                     0x07,      24,       4 // Reset (power-on reset)
#define DA4_CMD_SET_INT_REF               0x08,      24,       4 // Set up internal REF register.

/* AD5628 ADDRESS options */
/*										value   |LSB offset | no. of bits  */
#define DA4_ADDR_DAC_A                    CH_A,      20,       4  // Select DAC A
#define DA4_ADDR_DAC_B      		      CH_B,      20,       4  // Select DAC B
#define DA4_ADDR_DAC_C      			  CH_C,      20,       4  // Select DAC C
#define DA4_ADDR_DAC_D      			  CH_D,      20,       4  // Select DAC D
#define DA4_ADDR_DAC_E      			  CH_E,      20,       4  // Select DAC E
#define DA4_ADDR_DAC_F      			  CH_F,      20,       4  // Select DAC F
#define DA4_ADDR_DAC_G      			  CH_G,      20,       4  // Select DAC G
#define DA4_ADDR_DAC_H                    CH_H,      20,       4  // Select DAC H
#define DA4_ADDR_DAC_ALL                ALL_CH,      20,       4  // Select all DACs

/* AD5628 DATA FIELD */
/*										value   |LSB offset | no. of bits  */
#define DA4_SET_DATA_FIELD(val)            val, 	  8,      12

/* Internal Reference Register */
/*										value   |LSB offset | no. of bits  */
#define DA4_INT_REF_OFF     			0x00,         0,       1
#define DA4_INT_REF_ON                  0x01,         0,       1

/* Power-Down Modes of Operation */
/*										value   |LSB offset | no. of bits  */
#define DA4_PWRDN_NONE                  0x00,         8,       2 // Normal operation
#define DA4_PWRDN_1K                    0x01,         8,       2 // 1 KOhm to GND    (Power-down mode)
#define DA4_PWRDN_100K                  0x02,         8,       2 // 100 KOhm to GND  (Power-down mode)
#define DA4_PWRDN_3STATE                0x03,         8,       2 // Three-state      (Power-down mode)

/* Power-down/power-up channel selection—set bit to 1 to select  */
#define A_OFF     			0
#define A_ON                 1
#define B_OFF     			0
#define B_ON                 1
#define C_OFF     			0
#define C_ON                 1
#define D_OFF     			0
#define D_ON                 1
#define E_OFF     			0
#define E_ON                 1
#define F_OFF     			0
#define F_ON                 1
#define G_OFF     			0
#define G_ON                 1
#define H_OFF     			0
#define H_ON                 1

/*										             value                                        |LSB offset | no. of bits  */
#define DA4_PWRDN_CH_SEL(H,G,F,E,D,C,B,A)    (H<<7)|(G<<6)|(F<<5)|(E<<4)|(D<<3)|(C<<2)|(B<<1)|A, 	    0,         8
#define DA4_PWRDN_CH_SEL_ALL                                                               0xFF,		0,         8

/* Clear Code Function */
/*										value   |LSB offset | no. of bits  */
#define DA4_CODE_0X0000                    0x00,      0,        2
#define DA4_CODE_0X8000                    0x01,      0,	    2
#define DA4_CODE_0XFFFF                    0x02,      0,		2
#define DA4_CODE_NOP                       0x03,      0,		2




#define DA4_SPI_NO_BITS                       12    // Number of data bits per transaction
#define DA4_SPI_ERR_SUCCESS                    0  // The action completed successfully
#define DA4_SPI_ERR_VAL_OUT_OF_RANGE        0xEB  // The value is out of range




typedef struct PmodDA4 {
   XSpi DA4Spi;
} PmodDA4;

/* ------------------------------------------------------------ */
/*                  Procedure Declarations                      */
/* ------------------------------------------------------------ */

void DA4_begin(PmodDA4 *InstancePtr, u32 SPI_Address);
void DA4_end(PmodDA4 *InstancePtr);
int DA4_SPIInit(XSpi *SpiInstancePtr);
int DA4_set_int_ref(PmodDA4 *InstancePtr);
int DA4_set_power_mode(PmodDA4 *InstancePtr, u8 mode_number, u8 H, u8 G,u8 F,u8 E,u8 D,u8 C,u8 B,u8 A);
int DA4_reset(PmodDA4 *InstancePtr);
int DA4_load_clear_mode(PmodDA4 *InstancePtr);
void DA4_set_input_register(uint8_t *regValue, uint32_t value, uint8_t lsbBit, uint8_t noBits);
int DA4_WritePhysicalValue(PmodDA4 *InstancePtr, float dPhysicalValue, u8 channel);

#endif // PMODDA4_H
