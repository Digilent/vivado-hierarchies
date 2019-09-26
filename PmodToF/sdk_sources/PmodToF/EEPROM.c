/******************************************************************************/
/*                                                                            */
/* EEPROM.c --  EEPROM I2C driver source                                        */
/*                                                                            */
/******************************************************************************/
/* Author: Ana-Maria-Eliza Balas                                              */
/*		   ana-maria.balas@digilent.ro										  */
/* Copyright 2019, Digilent Inc.                                              */
/*                                                                            */
/******************************************************************************/
/* Module Description:                                                        */
/*                                                                            */
/* This file contains the functions for communication over I2C with           */
/* the Atmel® AT24C04D EEPROM memory chip.                                    */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    09/23/2019(anamariabalas):   Created                                    */
/*    09/23/2019(anamariabalas): Validated for Vivado 2019.1                  */
/*                                                                            */
/******************************************************************************/


/* -------------------------------------------------------------------------- */
/*					Include Files    						                  */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#include "EEPROM.h"
#include "PmodToF.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/************************** Local Functions Prototypes ************************/

u8 EPROM_ReadIIC(EPROM* InstancePtr, u8 reg, u8 *Data, int nData);
u8 EPROM_WriteIIC(EPROM* InstancePtr, u8 reg, u8 *Data, int nData);
int EPROM_IICInit(XIic *IicInstancePtr);
u8 EPROM_GetDevAddr(EPROM* InstancePtr, u8 a8);
void EPROM_end(EPROM* InstancePtr);

/******************************************************************************/
XIic_Config EPROM_Config =
{
	0,
	0,
	0,
	2
};
/************************** Function Definitions ***************************/

/* ------------------------------------------------------------ */
/***	EPROM_begin
**
**	Parameters:
**		InstancePtr: A EPROM object to start
**		IIC_Address: The Base address of the EPROM IIC
**		Chip_Address: 7 bit Pmod device address
**
**	Return Value:
**		none
**
**	Description:
**		Initializes the EEPROM module.
*/
void EPROM_begin(EPROM* InstancePtr, u32 IIC_Address, u8 Chip_Address)
{
	InstancePtr->chipAddr=Chip_Address;
	EPROM_Config.BaseAddress=IIC_Address;
	EPROM_IICInit(&InstancePtr->EPROMIic);

}


/* ------------------------------------------------------------ */
/***	EPROM_end
**
**	Parameters:
**		InstancePtr		- EPROM object to stop
**
**	Return Value:
**		none
**	Description:
**		Stops the EEPROM device.
*/
void EPROM_end(EPROM* InstancePtr){
	XIic_Stop(&InstancePtr->EPROMIic);
}

/* ------------------------------------------------------------ */
/***	EPROM_IICInit
**
**	Parameters:
**		InstancePtr		- EPROM object
**
**	Return Value:
**	int	  	XST_FAILURE		- fail to initialize Xilinx I2C device
**	int		XST_SUCCESS 	- success
**			
**	Description:
**		Initializes the EPROM IIC.

*/

int EPROM_IICInit(XIic *IicInstancePtr){
	int Status;

	Status = XIic_CfgInitialize(IicInstancePtr, &EPROM_Config, EPROM_Config.BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}



	/*
	 * Start the IIC driver so that the device is enabled.
	 */
	XIic_Start(IicInstancePtr);

	/*
	 * Disable Global interrupt to use polled mode operation
	 */
	XIic_IntrGlobalDisable(IicInstancePtr);

	return XST_SUCCESS;

}

/* ------------------------------------------------------------ */
/***	EPROM_ReadIIC
**
**	Parameters:
**		InstancePtr - EPROM object to initialize
**		addr		- Address to read from
**		Data		- Pointer to receive buffer
**		nData		- Number of data values to read
**
**	Return Value:
**	u8	ERRVAL_EPROM_READ	- failed to read EEPROM over I2C communication
**	u8	ERRVAL_SUCCESS		- success
**
**	Description:
**		Reads nData data bytes from the consecutive registers starting at the specified register address.
**
*/
u8 EPROM_ReadIIC(EPROM* InstancePtr, u8 addr, u8 *Data, int nData)
{
	int Status;
	Status = XIic_Start(&InstancePtr->EPROMIic);
	if (Status != XST_SUCCESS) {
		return ERRVAL_EPROM_READ;
	}

	XIic_Send(InstancePtr->EPROMIic.BaseAddress, InstancePtr->chipAddr, &addr, 1, XIIC_STOP);
	while(XIic_IsIicBusy(&InstancePtr->EPROMIic)) {
		//xil_printf("waiting to write eeprom for read operation\r\n");
	}

	XIic_Recv(InstancePtr->EPROMIic.BaseAddress, InstancePtr->chipAddr, Data, nData, XIIC_STOP);
	while(XIic_IsIicBusy(&InstancePtr->EPROMIic)) {
		//xil_printf("waiting to read eeprom\r\n");
	}
	Status = XIic_Stop(&InstancePtr->EPROMIic);
	if (Status != XST_SUCCESS) {
		return ERRVAL_EPROM_READ;
	}
	return ERRVAL_SUCCESS;
}

/* ------------------------------------------------------------ */
/***	EPROM_WriteIIC
**
**	Parameters:
**		InstancePtr - EPROM object to initialize
***		addr		- Address to send to
**		Data		- Pointer to data buffer to send
**		nData		- Number of data values to send
**
**	Return Value:
**	u8	ERRVAL_EPROM_WRITE  - failed to write EEPROM over I2C communication
**	u8	ERRVAL_SUCCESS		- success
**	Description:
**		Writes nData data bytes to the consecutive registers starting at the specified register address.
**
*/
u8 EPROM_WriteIIC(EPROM* InstancePtr, u8 addr, u8 *Data, int nData)
{

	u8 out[17];
	int Status;
	int Sent;
	if(nData > 16)
	{
		nData = 16;
	}

	out[0]=addr;

	for(int i = 0; i< nData; i++)
	{
		out[i+1] = Data[i];
	}

	Status = XIic_Start(&InstancePtr->EPROMIic);
	if (Status != XST_SUCCESS) {
		return ERRVAL_EPROM_WRITE;
	}

	Sent = XIic_Send(InstancePtr->EPROMIic.BaseAddress, InstancePtr->chipAddr, out, nData+1, XIIC_STOP);
	if (Sent == 0) {
		//xil_printf("failed to write eeprom\r\n");
	}

	while(XIic_IsIicBusy(&InstancePtr->EPROMIic)) {
		//xil_printf("waiting to write eeprom\r\n");
	}

	Status = XIic_Stop(&InstancePtr->EPROMIic);
	if (Status != XST_SUCCESS) {
		return ERRVAL_EPROM_WRITE;
	}
	return ERRVAL_SUCCESS;
}
/* ------------------------------------------------------------ */
/***	EPROM_PageWrite
**
**	Parameters:
**		InstancePtr - EPROM object to initialize
***		addr		- Address to send to
**		Data		- Pointer to data buffer to send
**		cntdata		- Number of data values to send
**
**	Return Value:
** u8 result :
**				ERRVAL_EPROM_READ	- failed to read EEPROM over I2C communication
**				ERRVAL_SUCCESS		- success
**	Description:
**		Gets the EEPROM device address and writes cntdata data bytes to the consecutive registers 
**		starting at the specified register address.
*/
uint8_t EPROM_PageWrite(EPROM* InstancePtr, u16 addr, u8* data, u8 cntdata)
{
	u8 result;
	u8 epromDevAddr = EPROM_GetDevAddr(InstancePtr, (addr & 0x100)>>9);
	XIic_SetAddress(&InstancePtr->EPROMIic, XII_ADDR_TO_SEND_TYPE, epromDevAddr);
	result=EPROM_WriteIIC(InstancePtr, (addr & 0xFF), data, cntdata);
	return result;
}
/* ------------------------------------------------------------ */
/***	EPROM_PageRead
**
**	Parameters:
**		InstancePtr - EPROM object to initialize
***		addr		- Address to send to
**		Data		- Pointer to data buffer to send
**		cntdata		- Number of data values to send
**
**	Return Value:
** u8 result :
**				ERRVAL_EPROM_READ	- failed to read EEPROM over I2C communication
**				ERRVAL_SUCCESS		- success
**	Description:
**		Gets the EEPROM device address and reads cntdata data bytes from the consecutive registers 
**		starting at the specified register address.
*/
uint8_t EPROM_PageRead(EPROM* InstancePtr, u16 addr, u8* data, u8 cntdata)
{
	u8 result;
	u8 epromDevAddr = EPROM_GetDevAddr(InstancePtr, (addr & 0x100)>>9);
	XIic_SetAddress(&InstancePtr->EPROMIic, XII_ADDR_TO_SEND_TYPE, epromDevAddr);
	result=EPROM_ReadIIC(InstancePtr, (addr & 0xFF), data, cntdata);
	return result;
}

/* ------------------------------------------------------------ */
/***	EPROM_GetDevAddr
**
**	Parameters:
**		InstancePtr - EPROM object to initialize
***		a8			- the 9th-bit of the memory array word address
**
**	Return Value:
**	u8	devAddr     - EEPROM device address
**
**	Description:
**		Returns the EEPROM device address.
**
*/
u8 EPROM_GetDevAddr(EPROM* InstancePtr, u8 a8)
{
	u8 devAddr = InstancePtr->chipAddr|(a8 & 1);
	return devAddr;
}

