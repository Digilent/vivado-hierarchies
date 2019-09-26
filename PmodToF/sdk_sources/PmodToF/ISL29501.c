/******************************************************************************/
/*                                                                            */
/* ISL29501.c --  ISL29501 I2C driver source                                  */
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
/* the ISL29501 chip.                                                         */
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
#include "ISL29501.h"
#include "PmodToF.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/************************** Local Functions Prototypes ************************/
void ISL29501_end(ISL29501* InstancePtr);
int ISL29501_IICInit(XIic *IicInstancePtr);

/******************************************************************************/
XIic_Config ISL29501_Config =
{

	0,
	0,
	0,
	2

};
/************************** Function Definitions ***************************/


/* ------------------------------------------------------------ */
/***	void ISL29501_begin
**
**	Parameters:
**		InstancePtr: A ISL29501 object to start
**		IIC_Address: The Base address of the ISL29501 IIC
**		Chip_Address: 7 bit Pmod device address
**
**	Return Value:
**		none
**
**	Description:
**		Initializes the ISL29501.
*/
void ISL29501_begin(ISL29501* InstancePtr, u32 IIC_Address, u8 Chip_Address)
{
	ISL29501_Config.BaseAddress=IIC_Address;
	InstancePtr->chipAddr=Chip_Address;
	ISL29501_IICInit(&InstancePtr->ISL29501Iic);
	XIic_SetAddress(&InstancePtr->ISL29501Iic, XII_ADDR_TO_SEND_TYPE, InstancePtr->chipAddr);
}


/* ------------------------------------------------------------ */
/***	ISL29501_end
**
**	Parameters:
**		InstancePtr		- ISL29501 object to stop
**
**	Return Value:
**		none
**
**	Description:
**		Stops the ISL29501 device.
*/
void ISL29501_end(ISL29501* InstancePtr){
	XIic_Stop(&InstancePtr->ISL29501Iic);
}

/* ------------------------------------------------------------ */
/***	ISL29501_IICInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Description:
**		Initializes the ISL29501 IIC.

*/

int ISL29501_IICInit(XIic *IicInstancePtr){
	int Status;

	Status = XIic_CfgInitialize(IicInstancePtr, &ISL29501_Config, ISL29501_Config.BaseAddress);
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
/***	ISL29501_ReadIIC
**
**	Parameters:
**		InstancePtr - ISL29501 object to initialize
**		reg			- Register to read from
**		Data		- Pointer to recieve buffer
**		nData		- Number of data values to read
**
**	Return Value:
**	u8	ERRVAL_ToF_READ - failed to read ISL29501 registers over I2C communication
**	u8	ERRVAL_SUCCESS 	- success
**	Description:
**		Reads nData data bytes from the consecutive registers starting at the specified register address.
**
*/
u8 ISL29501_ReadIIC(ISL29501* InstancePtr, u8 reg, u8 *Data, int nData)
{
	int Status;
	int Sent;
	Status = XIic_Start(&InstancePtr->ISL29501Iic);
	if (Status != XST_SUCCESS) {
		return ERRVAL_ToF_READ;
	}

	Sent = XIic_Send(InstancePtr->ISL29501Iic.BaseAddress, InstancePtr->chipAddr, &reg, 1, XIIC_STOP);
	if (Sent == 0) {
		//xil_printf("failed to write iic for read operation\r\n");
	}
	while(XIic_IsIicBusy(&InstancePtr->ISL29501Iic)) {
		//xil_printf("waiting to write iic for read operation\r\n");
	}

	XIic_Recv(InstancePtr->ISL29501Iic.BaseAddress, InstancePtr->chipAddr, Data, nData, XIIC_STOP);
	while(XIic_IsIicBusy(&InstancePtr->ISL29501Iic)) {
		//xil_printf("waiting to read iic\r\n");
	}

	Status = XIic_Stop(&InstancePtr->ISL29501Iic);
	if (Status != XST_SUCCESS) {
		return ERRVAL_ToF_READ;
	}
	return ERRVAL_SUCCESS;
}

/* ------------------------------------------------------------ */
/***	ISL29501_WriteIIC
**
**	Parameters:
**		InstancePtr - ISL29501 object to initialize
**		reg			- Register to send to
**		Data		- Pointer to data buffer to send
**		nData		- Number of data values to send
**
**	Return Value:
**	u8	ERRVAL_ToF_WRITE - failed to write ISL29501 registers over I2C communication
**	u8	ERRVAL_SUCCESS 	- success
**	Description:
**		Writes nData data bytes to the consecutive registers starting at the specified register address.
**
*/
u8 ISL29501_WriteIIC(ISL29501* InstancePtr, u8 reg, u8 *Data, int nData)
{
	u8 out[10];
	out[0]=reg;
//	out[1]=*Data;
	for(int i = 0; i < nData; i++)
	{
		out[i+1] = Data[i];
	}
	int Status;
	int Sent;

	Status = XIic_Start(&InstancePtr->ISL29501Iic);
	if (Status != XST_SUCCESS) {
		return ERRVAL_ToF_WRITE;
	}

	Sent = XIic_Send(InstancePtr->ISL29501Iic.BaseAddress, InstancePtr->chipAddr, out, nData+1, XIIC_STOP);
	if (Sent == 0) {
		//xil_printf("failed to write iic\r\n");
	}

	while(XIic_IsIicBusy(&InstancePtr->ISL29501Iic)) {
		//xil_printf("waiting to write iic\r\n");
	}

	Status = XIic_Stop(&InstancePtr->ISL29501Iic);
	if (Status != XST_SUCCESS) {
		return ERRVAL_ToF_WRITE;
	}
	return ERRVAL_SUCCESS;
}
