/********************************************************************************************/
/*                                                                           				*/
/* main.c --                                          					*/
/*                                                                            				*/
/********************************************************************************************/
/* Author:  Ana-Maria-Eliza Balas                                                   		*/
/*		    ana-maria.balas@digilent.ro										  				*/
/*                                                                            				*/
/* Copyright 2019, Digilent Inc.                                              				*/
/*                                                                            				*/
/********************************************************************************************/
/* Module Description:                                                        				*/
/*                                                                            				*/
/* This file contains source code for running a demonstration of the Pmod ToF device		*/
/* when used with the Pmod ToF Hierarchical Block. 											*/
/* More informations at :																	*/
/*  https://reference.digilentinc.com/reference/pmod/pmodtof/libraryuserguide             	*/
/*																							*/
/* The user will have to use a terminal connected    										*/
/* to the USB port corresponding to the connected board,           							*/
/* configured with the appropriate Baud rate. 115200 for Zynq systems, and    				*/
/* whatever the AXI UARTLITE IP is configured with for MicroBlaze systems,    				*/
/* typically 9600 or 115200 Baud.                                             				*/
/*                                                                            				*/
/*                                                                            				*/
/********************************************************************************************/
/* Revision History:                                                          				*/
/*                                                                            				*/
/*    09/25/2019(anamariabalas):   Created                                    				*/
/*    09/25/2019(anamariabalas): Validated for Vivado 2019.1                  				*/
/*                                                                            				*/
/********************************************************************************************/
/* ---------------------------------------------------------------------------------------- */
/*					Include Files    						   							 	*/
/* ---------------------------------------------------------------------------------------- */


#include <stdio.h>
#include "PmodToF/PmodToF.h"


int main(void)
{
	u8 ErrorCode;
	char SerialNo[12];
	int N = 100, sum = 0;
	int distance_val, distance_val_avg;

	PmodToF_Initialize();

	// 100 distance values that are measured will be averaged into a final distance value
	for(int j=0;j<N;j++)
	{
		distance_val = 1000 * PmodToF_perform_distance_measurement(); // the distance value is in millimeters
		sum = sum + distance_val;
	}
	distance_val_avg = sum/N;
	xil_printf("Distance = %d mm \r\n", distance_val_avg);
	
/*  Please uncomment the needed functionality
/*

    //start calibration for a setup distance of 30 cm
	ErrorCode =  PmodToF_start_calibration(0.3);
	if (ErrorCode != ERRVAL_SUCCESS)
		return ErrorCode;
	else
		xil_printf("%s \r\n",szMsg);

	//save calibration data to EEPROM user area
	ErrorCode = PmodToF_WriteCalibsToEPROM_User();
	if (ErrorCode != ERRVAL_SUCCESS)
		return ErrorCode;
	else
		xil_printf("%s \r\n",szMsg);

	//read calibration data from EEPROM user area
	ErrorCode = PmodToF_ReadCalibsFromEPROM_User();
	if (ErrorCode != ERRVAL_SUCCESS)
		return ErrorCode;
	else
		xil_printf("Calibration read from EEPROM user area. \r\n");

	//restore factory calibration
	ErrorCode = PmodToF_RestoreAllCalibsFromEPROM_Factory();
	if (ErrorCode != ERRVAL_SUCCESS)
		return ErrorCode;
	else
		xil_printf("%s \r\n",szMsg);

	//read Serial Number
	ErrorCode = PmodToF_ReadSerialNoFromEPROM(SerialNo);
	if (ErrorCode != ERRVAL_SUCCESS)
		return ErrorCode;
	else
		xil_printf("Serial Number : %s \r\n",SerialNo);

*/
	return ERRVAL_SUCCESS;
}




