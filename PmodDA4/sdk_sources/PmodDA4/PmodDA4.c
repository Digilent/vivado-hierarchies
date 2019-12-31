/******************************************************************************/
/*                                                                            */
/* PmodDA4.c -- Definitions for the PmodDA4 library                           */
/*                                                                            */
/******************************************************************************/
/* Author: Jon Peyron                                                         */
/* Copyright 2017, Digilent Inc.                                              */
/******************************************************************************/
/* File Description:                                                          */
/*                                                                            */
/* This file defines the DA4 library functions to initialize and use the      */
/* Hierarchical Block                                                         */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    06/20/2016(JonP):     Created                                           */
/*    04/28/2017(ArtVVB):   Validated                                         */
/*    02/13/2018(atangzwj): Validated for Vivado 2017.4                       */
/*                                                                            */
/******************************************************************************/

/***************************** Include Files *******************************/

#include "PmodDA4.h"
#include <stdio.h>
#include <stdbool.h>


/************************** Function Definitions ***************************/
XSpi_Config DA4Config =
{
   0,
   0,
   1,
   0,
   1,
   8,
   0,
   0,
   0,
   0,
   0
};

/* ------------------------------------------------------------ */
/*** void DA4_begin(PmodDA4 *InstancePtr, u32 SPI_Address)
**
**   Parameters:
**      InstancePtr: A PmodDA4 object to start
**      SPI_Address: The Base address of the PmodDA4 SPI
**
**   Return Value:
**      None
**
**   Errors:
**      None
**
**   Description:
**      Initialize the PmodDA4.
*/
void DA4_begin(PmodDA4 *InstancePtr, u32 SPI_Address) {
   DA4Config.BaseAddress = SPI_Address;
   DA4_SPIInit(&InstancePtr->DA4Spi);

}

/* ------------------------------------------------------------ */
/*** void DA4_end(PmodDA4 *InstancePtr)
**
**   Parameters:
**      InstancePtr - PmodDA4 object to stop
**
**   Return Value:
**      None
**
**   Errors:
**      None
**
**   Description:
**      Stops the device
*/
void DA4_end(PmodDA4 *InstancePtr) {
   XSpi_Stop(&InstancePtr->DA4Spi);
}

/* ------------------------------------------------------------ */
/*** DA4_SPIInit
**
**   Parameters:
**      None
**
**   Return Value:
**   	-XST_SUCCESS if the Spi is successfully initialized. Otherwise, returns
**   	-XST_FAILURE.

**   Errors:
**      None
**
**   Description:
**      Initializes the PmodDA4 SPI(mode 3).
*/
int DA4_SPIInit(XSpi *SpiInstancePtr) {
   int Status;

   Status = XSpi_CfgInitialize(SpiInstancePtr, &DA4Config,
         DA4Config.BaseAddress);
   if (Status != XST_SUCCESS) {
      return XST_FAILURE;
   }

   u32 options = (XSP_MASTER_OPTION
         | XSP_CLK_PHASE_1_OPTION) | XSP_MANUAL_SSELECT_OPTION;
   Status = XSpi_SetOptions(SpiInstancePtr, options);
   if (Status != XST_SUCCESS) {
      return XST_FAILURE;
   }

   Status = XSpi_SetSlaveSelect(SpiInstancePtr, 1);
   if (Status != XST_SUCCESS) {
      return XST_FAILURE;
   }

   /*
    * Start the SPI driver so that the device is enabled.
    */
   XSpi_Start(SpiInstancePtr);

   /*
    * Disable Global interrupt to use polled mode operation
    */
   XSpi_IntrGlobalDisable(SpiInstancePtr);

   return XST_SUCCESS;
}



/* ------------------------------------------------------------ */
/*** u8 DA4_WritePhysicalValue(PmodDA4 *InstancePtr, float dPhysicalValue, u8 channel)
**
**   Parameters:
**      InstancePtr    - A PmodDA4 object to start
**      dPhysicalValue - The physical value that must be reconstructed at the
**                       output of the DA converter
**      channel		   - output channel of DA4
**
**   Return Value:
**   	-XST_SUCCESS if the buffers are successfully handed off to the
**		driver for transfer. Otherwise, returns:
**		- XST_DEVICE_IS_STOPPED if the device must be started before
**		transferring data.
**		- XST_DEVICE_BUSY indicates that a data transfer is already in
**		progress. This is determined by the driver.
**		- XST_SPI_NO_SLAVE indicates the device is configured as a
**		master and a slave has not yet been selected.
**      - DA4_SPI_ERR_VAL_OUT_OF_RANGE (1) - The physical value is not inside
**                                              accepted range
**
**   Errors:
**      If module is not initialized (using begin), the function does nothing.
**      Also, see return value.
**
**   Description:
**      The function computes the integer value corresponding to the physical
**      value by considering the reference value as the one corresponding to the
**      maximum integer value (0xFF). If the integer value is within the
**      accepted range (0 - 0xFF), this function writes the 12- bit value to the
**      DA converter, by writing 8 bits to SPI, and returns the
**      DACSPI1_ERR_SUCCESS message. If the integer value is outside the allowed
**      range, the function does nothing and returns the
**      DA4_SPI_ERR_VAL_OUT_OF_RANGE message.
**
*/
int DA4_WritePhysicalValue(PmodDA4 *InstancePtr, float dPhysicalValue, u8 channel)
{
	uint8_t in_reg_value[4] = {0,0,0,0};
	float dReference = 2.5; // The value corresponding to the maximum converter
					   // value (reference voltage).

	u16 wValue = dPhysicalValue * (((float) (1 << DA4_SPI_NO_BITS)-1) / dReference);
	if (wValue < 0 || wValue >= (1 << DA4_SPI_NO_BITS))
	{
		xil_printf("DAC value out of range.\n\r");
		return DA4_SPI_ERR_VAL_OUT_OF_RANGE;
	}
	else
	{
		DA4_set_input_register(in_reg_value, DA4_CMD_WRITE_INPUT_N_UPDATE_N);
		xil_printf(" Physical Value = %d  \r\n",wValue);
		switch (channel)
		{
			case 0:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_A);
			break;
			case 1:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_B);
			break;
			case 2:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_C);
			break;
			case 3:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_D);
			break;
			case 4:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_E);
			break;
			case 5:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_F);
			break;
			case 6:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_G);
			break;
			case 7:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_H);
			break;
			case 0x0F:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_ALL);
			break;
			default:
				DA4_set_input_register(in_reg_value, DA4_ADDR_DAC_A);
		}

		DA4_set_input_register(in_reg_value, DA4_SET_DATA_FIELD(wValue));
		int status = XSpi_Transfer(&InstancePtr->DA4Spi, in_reg_value, NULL, 4);
		return status;
	}
}
/* ------------------------------------------------------------ */
/*** DA4_set_int_ref(PmodDA4 *InstancePtr)
**
**   Parameters:
**      InstancePtr - PmodDA4 object
**
**   Return Value:
**   	-XST_SUCCESS if the buffers are successfully handed off to the
**		driver for transfer. Otherwise, returns:
**		- XST_DEVICE_IS_STOPPED if the device must be started before
**		transferring data.
**		- XST_DEVICE_BUSY indicates that a data transfer is already in
**		progress. This is determined by the driver.
**		- XST_SPI_NO_SLAVE indicates the device is configured as a
**		master and a slave has not yet been selected.
**
**   Description:
**    Setting the internal reference register.
**    The AD5628 have an on-chip reference with an internal gain of 2.
**    The AD5628 have a 1.25 V internal reference, giving a full-scale output of 2.5 V;
*/
int DA4_set_int_ref(PmodDA4 *InstancePtr)
{
	uint8_t in_reg_value[4] = {0,0,0,0};
	DA4_set_input_register(in_reg_value, DA4_CMD_SET_INT_REF);
	DA4_set_input_register(in_reg_value, DA4_INT_REF_ON);
	int status = XSpi_Transfer(&InstancePtr->DA4Spi, in_reg_value, NULL, 4);
	return status;
}
/* ------------------------------------------------------------ */
/*** DA4_set_power_mode(PmodDA4 *InstancePtr, u8 mode_number, u8 H, u8 G,u8 F,u8 E,u8 D,u8 C,u8 B,u8 A)
**
**   Parameters:
**      InstancePtr - PmodDA4 object
**		mode_number - AD5628 modes  of operation
**		H - A       - Power-down/power-up channel selection
**
**   Return Value:
**   	-XST_SUCCESS if the buffers are successfully handed off to the
**		driver for transfer. Otherwise, returns:
**		- XST_DEVICE_IS_STOPPED if the device must be started before
**		transferring data.
**		- XST_DEVICE_BUSY indicates that a data transfer is already in
**		progress. This is determined by the driver.
**		- XST_SPI_NO_SLAVE indicates the device is configured as a
**		master and a slave has not yet been selected.
**
**   Description:
**    The AD5628 contain four separate modes  of operation.
**    Command 0100 is reserved for the power-down function.
**    These modes are software-programmable by setting two bits,
**    Bit DB9 and Bit DB8, in the control register.
*/
int DA4_set_power_mode(PmodDA4 *InstancePtr, u8 mode_number, u8 H, u8 G,u8 F,u8 E,u8 D,u8 C,u8 B,u8 A)
{
	uint8_t in_reg_value[4] = {0,0,0,0};
	DA4_set_input_register(in_reg_value, DA4_CMD_POWERDOWN);
	switch (mode_number)
	{
	    case 0:
	    	DA4_set_input_register(in_reg_value, DA4_PWRDN_NONE);
	        break;
	    case 1:
	    	DA4_set_input_register(in_reg_value, DA4_PWRDN_1K);
	        break;
	    case 2:
	    	DA4_set_input_register(in_reg_value, DA4_PWRDN_100K);
	    	        break;
	    case 3:
	    	DA4_set_input_register(in_reg_value, DA4_PWRDN_3STATE);
	    	        break;
	    default:
	    	DA4_set_input_register(in_reg_value, DA4_PWRDN_NONE);
	}
	DA4_set_input_register(in_reg_value, DA4_PWRDN_CH_SEL(H_OFF,G_OFF,F_OFF,E_OFF,D_OFF,C_OFF,B_OFF,A_ON));
	int status = XSpi_Transfer(&InstancePtr->DA4Spi, in_reg_value, NULL, 4);
	return status;
}
/* ------------------------------------------------------------ */
/*** DA4_reset(PmodDA4 *InstancePtr)
**
**   Parameters:
**      InstancePtr - PmodDA4 object
**
**   Return Value:
**   	-XST_SUCCESS if the buffers are successfully handed off to the
**		driver for transfer. Otherwise, returns:
**		- XST_DEVICE_IS_STOPPED if the device must be started before
**		transferring data.
**		- XST_DEVICE_BUSY indicates that a data transfer is already in
**		progress. This is determined by the driver.
**		- XST_SPI_NO_SLAVE indicates the device is configured as a
**		master and a slave has not yet been selected.
**
**
**   Description:
**    Software executable reset function that resets the DAC
**    to the power-on reset code.
**    Command 0111 is reserved for this reset function.
*/
int DA4_reset(PmodDA4 *InstancePtr)
{
	uint8_t in_reg_value[4] = {0,0,0,0};
	DA4_set_input_register(in_reg_value, DA4_CMD_RESET);
	int status = XSpi_Transfer(&InstancePtr->DA4Spi, in_reg_value, NULL, 4);
	return status;

}
/* ------------------------------------------------------------ */
/*** DA4_load_clear_mode(PmodDA4 *InstancePtr)
**
**   Parameters:
**      InstancePtr - PmodDA4 object
**
**   Return Value:
**   	-XST_SUCCESS if the buffers are successfully handed off to the
**		driver for transfer. Otherwise, returns:
**		- XST_DEVICE_IS_STOPPED if the device must be started before
**		transferring data.
**		- XST_DEVICE_BUSY indicates that a data transfer is already in
**		progress. This is determined by the driver.
**		- XST_SPI_NO_SLAVE indicates the device is configured as a
**		master and a slave has not yet been selected.
**
**
**   Description:
**    Bringing the CLR line low clears the contents of the input register
**    and the DAC registers to the data contained in the user-configurable
**    CLR register and sets the analog outputs to 0 V.
*/
int DA4_load_clear_mode(PmodDA4 *InstancePtr)
{
	uint8_t in_reg_value[4] = {0,0,0,0};
	DA4_set_input_register(in_reg_value, DA4_CMD_LOAD_CLEAR_CODE);
	DA4_set_input_register(in_reg_value, DA4_CODE_0X0000);
	int status = XSpi_Transfer(&InstancePtr->DA4Spi, in_reg_value, NULL, 4);
	return status;
}
/* ------------------------------------------------------------ */
/*** DA4_set_input_register(uint8_t *regValue, uint32_t value, uint8_t lsbBit, uint8_t noBits)
**
**   Parameters:
**      regValue - input register
**      value    - value corresponding to the field inside input register
**      lsbBit	 - LSB of value
**      noBits   - number of bits of the specific field
**
**   Return Value:
**      None
**
**   Errors:
**      None
**
**   Description:
**    This function write a number of bits(value) to the specific field inside the input register.
*/
void DA4_set_input_register(uint8_t *regValue, uint32_t value, uint8_t lsbBit, uint8_t noBits)
{
    uint32_t regMask = ((1 << noBits) - 1) << lsbBit;
    uint32_t tmpValue= (regValue[0] << 24)|(regValue[1]<<16)|(regValue[2]<<8)|regValue[3];
    // align value to bit lsb_bit
    value <<= lsbBit;
    // mask out any bits outside specified field
    value &= regMask;
    // mask out bits corresponding to the specified field
    tmpValue &= ~regMask;

    // set the values for the field bits
    tmpValue |= value;

    regValue[0]= (tmpValue >> 24) & 0xFF;
    regValue[1]= (tmpValue >> 16) & 0xFF;
    regValue[2]= (tmpValue >> 8) & 0xFF;
    regValue[3]= tmpValue & 0xFF;

}
