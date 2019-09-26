/*********************************************************************************/
/*                                                                               */
/* PmodToF.c -- PmodToF Driver Source                                            */
/*                                                                               */
/*********************************************************************************/
/* Author: Ana-Maria-Eliza Balas                                                 */
/*		   ana-maria.balas@digilent.ro										     */
/* Copyright 2019, Digilent Inc.                                                 */
/*                                                                               */
/*********************************************************************************/
/* Module Description:                                                           */
/*                                                                               */
/* This file implements Pmod ToF device related functions.					     */
/* These functions configure and initialize the Pmod ToF device over I2C and     */
/* access Pmod ToF functionality such as performing calibrations and 		     */
/* measurements, R/W operations from EEPROM and ISL29501 registers.              */
/* In order to implement the I2C communication, the PmodToF module accesses      */
/* functions implemented in ISL29501 and EEPROM modules.                         */
/* This file contains library functions for performing calibration and           */
/* measurement. Calibration ensure the measurement accuracy by making            */
/* adjustments to correct the measurements error.                                */
/*                                                                               */
/* More details can be found at:												 */
/*                                                                               */
/* https://reference.digilentinc.com/reference/pmod/pmodtof/zynqlibraryuserguide */
/*                                                                               */
/*                                                                               */
/*********************************************************************************/
/* Revision History:                                                             */
/*                                                                               */
/*    09/23/2019(anamariabalas):   Created                                       */
/*    09/23/2019(anamariabalas): Validated for Vivado 2019.1                     */
/*                                                                               */
/*********************************************************************************/


/* ----------------------------------------------------------------------------- */
/*					Include Files    						                     */
/* ----------------------------------------------------------------------------- */
#include "PmodToF.h"
#include "xgpio.h"
#include "xparameters.h"
#include "sleep.h"
#include <stdio.h>
#include "math.h"
#include "EEPROM.h"
#include "ISL29501.h"


#define GPIO_CHANNEL 1		 //first channel
#define GPIO_DIRMASK 0x01	//set first channel tristate buffers: bit 1 = output (SS), bit 0 = input(IRQ)
#define GPIO_DATA_RDY_MSK 0x01 
ISL29501 myToFDevice;
EPROM myEPROMDevice;
XGpio gpio;
CALIBDATA calib;
SERIALNODATA serialNo ;


/* ------------------------------------------------------------ */
/*					Local functions used for calibration        */
/* ------------------------------------------------------------ */
void CALIB_initiate_calibration_measurement();
void CALIB_perform_magnitude_calibration();
void CALIB_perform_crosstalk_calibration();
void CALIB_perform_distance_calibration(double actual_dist);
uint8_t CALIB_WriteCalibsToEPROM_Raw(uint8_t baseAddr, uint8_t skip_read_regs);
uint8_t CALIB_ReadCalibsFromEPROM_Raw(CALIBDATA *pCalib, uint8_t baseAddr, uint8_t skip_write_regs);
uint8_t CALIB_Read_ISL29501_Regs();
uint8_t CALIB_Write_ISL29501_Regs();
uint8_t CALIB_WriteCalibsToEPROM_User(u8 msg_flag);


/* ------------------------------------------------------------ */
/*					 Utils		    						    */
/* ------------------------------------------------------------ */
unsigned char GetBufferChecksum(unsigned char *pBuf, int len);
double _3bytes_to_double(u8 EXP, u8 MSB, u8 LSB);
double _2bytes_to_double(u8 MSB, u8 LSB);
void double_to_3bytes(double AVG, u8* EXP, u8* MSB, u8* LSB);
void double_to_2bytes(double AVG, u8* MSB, u8* LSB);

/* ------------------------------------------------------------ */
/** void  PmodToF_Initialize()
**  Return Value:
**      none
**
**  Description:
** This function initializes the Pmod ToF device. This function initialize the EEPROM and ISL29501.
** ISL29501_begin 	- sets the ISL29501 chip address for communication over I2C protocol and initializes
**             	the registers as described in the Firmware Routines documentation( an1724.pdf ) for Chip Initialization
** EPROM_begin 	- sets the EEPROM chip address for communication over I2C protocol
** XGpio_Initialize and XGpio_SetDataDirection configure the GPIO communication for IRQ and SS pin
** 				of the ISL29501 chip.
**/
void PmodToF_Initialize()
{
	/* ------------------------------------------------------------- */
	/* ---------------------------!!!------------------------------- */
	/* ---------------------------!!!------------------------------- */
	/* ---------------------------!!!------------------------------- */

	//These values are the standard values chosen by Digilent for this device.The user must be very carefully if he wishes to change them.
	//Please read ISL29501 documentation before proceeding.
	//Check https://reference.digilentinc.com/reference/pmod/pmodtof/zynqlibraryuserguide for additional details
	//The user must modify these values before performing manual calibration,
	//but the user should make a backup of them, in case it is needed to be restored
	u8 reg0x10_data = 0x04;
	u8 reg0x11_data = 0x6E;
	u8 reg0x13_data = 0x71;
	u8 reg0x60_data = 0x01;
	u8 reg0x18_data = 0x22;
	u8 reg0x19_data = 0x22;
	u8 reg0x90_data = 0x0F;
	u8 reg0x91_data = 0xFF;
	/* ------------------------------------------------------------ */

	ISL29501_begin(&myToFDevice, XPAR_IIC_0_BASEADDR,ISL29501_CHIP_ADDRESS);
    EPROM_begin(&myEPROMDevice, XPAR_IIC_0_BASEADDR, EPROM_CHIP_ADDRESS);
    XGpio_Initialize(&gpio, XPAR_GPIO_0_DEVICE_ID); //initialize input XGpio variable
    XGpio_SetDataDirection(&gpio, 1, GPIO_DIRMASK);

    //steps for ISL29501 Chip Initialization as described in an1724.pdf :
    ISL29501_WriteIIC(&myToFDevice, 0x10, &reg0x10_data, 1);
	ISL29501_WriteIIC(&myToFDevice, 0x11, &reg0x11_data, 1);
	ISL29501_WriteIIC(&myToFDevice, 0x13, &reg0x13_data, 1);
	ISL29501_WriteIIC(&myToFDevice, 0x60, &reg0x60_data, 1);
	ISL29501_WriteIIC(&myToFDevice, 0x18, &reg0x18_data, 1);
	ISL29501_WriteIIC(&myToFDevice, 0x19, &reg0x19_data, 1);
	ISL29501_WriteIIC(&myToFDevice, 0x90, &reg0x90_data, 1);
	ISL29501_WriteIIC(&myToFDevice, 0x91, &reg0x91_data, 1);

	PmodToF_ReadCalibsFromEPROM_User();
}



/* ------------------------------------------------------------ */
/** double PmodToF_perform_distance_measurement()
**  Parameters:
**      none
**
**  Return Value:
**      distance - the distance in meters, measured by the PmodToF device
**
**  Description:
**  Function for performing a distance measurement.
**  Before running this command, it is important that a manual calibration 
**  was performed or that a calibration was stored in then imported
**  from the EEPROM user area or that factory calibration was restored from EEPROM.
**  It follows the steps as described in the Firmware Routines documentation (an1724.pdf)
**  for making a distance measurement.
**  As a result of measurement, the 0xD1 and 0xD2 registers are set with the values
**  corresponding to DistanceMSB and DistanceLSB.
**  The distance is computed starting from the values of these 2 registers using the 
**  formula provided in the in the Firmware Routines documentation(an1724.pdf).
**/
double PmodToF_perform_distance_measurement()
{
    /* WRITE REG */
    u8 reg0x13_data = 0x7D;
    u8 reg0x60_data = 0x01;
    /* READ REG */
    u8 unused;
    u8 DistanceMSB;
    u8 DistanceLSB;

    double distance = 1;
    ISL29501_WriteIIC(&myToFDevice, 0x13, &reg0x13_data, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x60, &reg0x60_data, 1);
    ISL29501_ReadIIC(&myToFDevice, 0x69, &unused, 1);
    CALIB_initiate_calibration_measurement();
	//waits for IRQ
    while((XGpio_DiscreteRead(&gpio, GPIO_CHANNEL) & GPIO_DATA_RDY_MSK) != 0 );
    ISL29501_ReadIIC(&myToFDevice, 0xD1, &DistanceMSB, 1);
    ISL29501_ReadIIC(&myToFDevice, 0xD2, &DistanceLSB, 1);
    distance =(((double)DistanceMSB * 256 + (double)DistanceLSB)/65536) * 33.31;
    return  distance;
}
/* ------------------------------------------------------------ */
/** void CALIB_initiate_calibration__measurement()
**  Parameters:
**      none
**
**  Return Value:
**      none
**
**  Description:
**  Function for initiating calibration measurements. Puts Sample Start(SS) pin in low logic to trigger a measurement cycle.
**  SS will be hold to low logic for 5.600 ms then to high logic for 14.400 ms, resulting a SS period of 20 ms.
**/
void CALIB_initiate_calibration_measurement()
{
    XGpio_DiscreteWrite(&gpio, GPIO_CHANNEL, 0x0); //write to the Channel 1, SS -> "0";
    usleep(5600); //5600 microseconds
    XGpio_DiscreteWrite(&gpio, GPIO_CHANNEL, 0x1<<1); //write to the Channel 1, SS -> "1";
    usleep(14400);
}

/* ------------------------------------------------------------ */
/** uint8_t PmodToF_start_calibration(double actual_distance)
**  Parameters:
**      actual_distance - the actual measuring distance (in meters) corresponding to the calibration setup. This distance must be larger than 0.05m.
**
**  Return Value:
**      ERRVAL_INCORRRECT_CALIB_DISTACE 	0xED	// incorrect calibration distance; it has to be more than 5 cm(0.05 m)
**		ERRVAL_FAILED_STARTING_CALIB		0xFC    // failed to start calibration, EEPROM or ISL29501 device is busy
**
**  Description:
** Function for performing a manual calibration of the device, for the actual distance provided as parameter. 
** It calls the function implementing the required  calibration sequence: CALIB_perform_magnitude_calibration, 
** CALIB_perform_crosstalk_calibration, CALIB_perform_distance_calibration, as described in the Firmware Routines documentation(an1724.pdf)
** The function returns ERRVAL_INCORRECT_CALIB_DISTACE if the provided parameter is not larger than 0.05.
** The function returns ERRVAL_FAILED_STARTING_CALIB if the calibration cannot be started (at least one of ISL29501 or EPROM are busy).
**/
uint8_t PmodToF_start_calibration(double actual_distance)
{
	u8 bErrCode = ERRVAL_SUCCESS;
	if (actual_distance == 0 || actual_distance < 0.05)
	{
		bErrCode = ERRVAL_INCORRECT_CALIB_DISTACE;
		return bErrCode;
	}
	else
	{
		xil_printf("OK,Starting calibration...\r\n");
		sleep(2);
		//verifying that the ISL29501 and EEPROM device is not busy
		if(!XIic_IsIicBusy(&myToFDevice.ISL29501Iic) && !XIic_IsIicBusy(&myEPROMDevice.EPROMIic))
		{
			xil_printf("Starting magnitude calibration... You have 5 sec to prepare the device\r\n");
			sleep(5);
			CALIB_perform_magnitude_calibration();

			xil_printf("Starting crosstalk calibration... You have 10 sec to prepare the device \r\n");
			sleep(10);
			CALIB_perform_crosstalk_calibration();

			xil_printf("Starting distance calibration...  You have 10 sec to prepare the device \r\n");
			sleep(10);
			CALIB_perform_distance_calibration(actual_distance);
			sprintf(szMsg,"Calibration done.\r\n");
			return bErrCode;
		}
		else
		{
			bErrCode = ERRVAL_FAILED_STARTING_CALIB;
			return bErrCode;
		}
	}
}


/* ------------------------------------------------------------ */
/** CALIB_perform_magnitude_calibration()
**  Parameters:
**      none
**  Return Value:
**      none
**
**  Description:
**  Function for performing magnitude calibration. It implements the steps described in the Firmware Routines documentation(an1724.pdf).
**  It is called by PmodToF_start_calibration function.
**/
void CALIB_perform_magnitude_calibration()
{
    /* WRITE REG */
    u8 reg0x13_data = 0x61;
    u8 reg0x60_data = 0x01;
    /* READ REG */
    u8 regs[3];
    u8 unused;

    ISL29501_WriteIIC(&myToFDevice, 0x13, &reg0x13_data, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x60, &reg0x60_data, 1);
    ISL29501_ReadIIC(&myToFDevice, 0x69, &unused, 1);
    CALIB_initiate_calibration_measurement();
	//waits for IRQ
    while((XGpio_DiscreteRead(&gpio, GPIO_CHANNEL) & GPIO_DATA_RDY_MSK) != 0 );
    ISL29501_ReadIIC(&myToFDevice, 0xF6, regs, 3);
    ISL29501_WriteIIC(&myToFDevice, 0x2C, regs, 3);
    reg0x13_data= 0x7D;
    ISL29501_WriteIIC(&myToFDevice, 0x13, &reg0x13_data, 1);
    reg0x60_data=0x00;
    ISL29501_WriteIIC(&myToFDevice, 0x60, &reg0x60_data, 1);
}

/* ------------------------------------------------------------ */
/** CALIB_perform_crosstalk_calibration()
**  Parameters:
**      none
**  Return Value:
**      none
**
**  Description:
**  Function for performing crosstalk calibration.
**  It implements the steps described in the Firmware Routines documentation(an1724.pdf).
**  It is called by PmodToF_start_calibration function.
**/
void CALIB_perform_crosstalk_calibration()
{
    int N= 100;
    /* WRITE REG */
    u8 reg0x13_data = 0x7D;
    u8 reg0x60_data = 0x01;
    /* READ REG */
    u8 regs[14];
    u8 unused;
    u8 i_exp_calib;
    u8 i_msb_calib;
    u8 i_lsb_calib;
    double i_sum=0;
    double i_avg;
    u8 q_exp_calib;
    u8 q_msb_calib;
    u8 q_lsb_calib;
    double q_sum=0;
    double q_avg;
    u8 gain_msb_calib;
    u8 gain_lsb_calib;
    double gain_sum=0;
    double gain_avg;


    ISL29501_WriteIIC(&myToFDevice, 0x13, &reg0x13_data, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x60, &reg0x60_data, 1);

    for(int i=0; i < N;i++)
    {
        ISL29501_ReadIIC(&myToFDevice, 0x69, &unused, 1);
        CALIB_initiate_calibration_measurement();
		//waits for IRQ
        while((XGpio_DiscreteRead(&gpio, GPIO_CHANNEL) & GPIO_DATA_RDY_MSK) != 0 );
        ISL29501_ReadIIC(&myToFDevice, 0xDA, regs, 14);
        double I = _3bytes_to_double(regs[0],regs[1],regs[2]);
        i_sum += I;
        double Q = _3bytes_to_double(regs[3],regs[4],regs[5]);
        q_sum += Q;
        double gain = _2bytes_to_double(regs[12],regs[13]);
        gain_sum += gain;

    }
    i_avg= i_sum / N;
    q_avg= q_sum / N;
    gain_avg= gain_sum / N;

    double_to_3bytes(i_avg, &i_exp_calib, &i_msb_calib, &i_lsb_calib);
    double_to_3bytes(q_avg, &q_exp_calib, &q_msb_calib, &q_lsb_calib);
    double_to_2bytes(gain_avg, &gain_msb_calib, &gain_lsb_calib);
    ISL29501_WriteIIC(&myToFDevice, 0x24, &i_exp_calib, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x25, &i_msb_calib, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x26, &i_lsb_calib, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x27, &q_exp_calib, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x28, &q_msb_calib, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x29, &q_lsb_calib, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x2A, &gain_msb_calib, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x2B, &gain_lsb_calib, 1);

}


/* ------------------------------------------------------------ */
/** void CALIB_perform_distance_calibration(double actual_dist)
**  Parameters:
**      actual_distance - the actual measuring distance (in meters) corresponding to the calibration setup.
**  Return Value:
**      none
**
**  Description:
**  Function for performing distance calibration.
**  It implements the steps described in the Firmware Routines documentation(an1724.pdf).
**  It is called by PmodToF_start_calibration function.
**/
void CALIB_perform_distance_calibration(double actual_dist)
{
    int N= 100;
    /* WRITE REG */
    u8 reg0x13_data = 0x7D;
    u8 reg0x60_data = 0x01;


    /* READ REG */
    u8 regs[2];
    u8 unused;
    double phase_sum=0;
    double phase_avg;
    double dist_calib;
    u8 dist_msb_calib;
    u8 dist_lsb_calib;


    ISL29501_WriteIIC(&myToFDevice, 0x13, &reg0x13_data, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x60, &reg0x60_data, 1);
    for(int i=0; i < N;i++)
    {
        ISL29501_ReadIIC(&myToFDevice, 0x69, &unused, 1);
        CALIB_initiate_calibration_measurement();
		//waits for IRQ
        while((XGpio_DiscreteRead(&gpio, GPIO_CHANNEL) & GPIO_DATA_RDY_MSK) != 0 );
        ISL29501_ReadIIC(&myToFDevice, 0xD8, regs, 2);
        double phase = _2bytes_to_double(regs[0],regs[1]);
        phase_sum += phase;

    }
    phase_avg= phase_sum / N;
    dist_calib = phase_avg - (actual_dist/33.31*65536);
    double_to_2bytes(dist_calib, &dist_msb_calib, &dist_lsb_calib);
    ISL29501_WriteIIC(&myToFDevice, 0x2F, &dist_msb_calib, 1);
    ISL29501_WriteIIC(&myToFDevice, 0x30, &dist_lsb_calib, 1);

}


/***    CALIB_WriteCalibsToEPROM_User(u8 msg_flag)
**
**  Parameters:
**      msg_flag - flag that enables display over UART the notification message
**
**  Return Value:
**      uint8_t ERRVAL_SUCCESS          0
**				ERRVAL_EPROM_WRITE		0xFA
**				ERRVAL_ToF_READ			0xF6
**
**  Description:
**      This local function writes calibration data in the user calibration area of EPROM. It calls CALIB_WriteCalibsToEPROM_Raw providing
**      user calibration area EPROM address as parameter. 
**		It returns ERRVAL_SUCCESS for success or the error codes provided by CALIB_WriteCalibsToEPROM_Raw function.
*/
uint8_t CALIB_WriteCalibsToEPROM_User(u8 msg_flag)
{
    uint8_t bErrCode;
    bErrCode = CALIB_WriteCalibsToEPROM_Raw((uint8_t)ADR_EPROM_CALIB, 0);  // write calibration to EPROM
    if(bErrCode == ERRVAL_SUCCESS)
    {
    	if(msg_flag == 1)
    	{
			sprintf(szMsg,"Calibration stored to EEPROM user space.");

    	}
    	return bErrCode;
    }
    else
    {

		return bErrCode;
    }
}

/***    PmodToF_WriteCalibsToEPROM_User
**
**  Parameters:
**     none
**
**  Return Value:
**      uint8_t ERRVAL_SUCCESS          0
**				ERRVAL_EPROM_WRITE		0xFA
**				ERRVAL_ToF_READ			0xF6
**
**  Description:
**      This function writes calibration data in the user calibration area of EEPROM.
**      It calls the local function CALIB_WriteCalibsToEPROM_User.
**      This function should be called after changes were made in calibration data(after a manual calibration),
**      in order to save them in the non-volatile memory.
**		It returns ERRVAL_SUCCESS for success or the error codes provided by CALIB_WriteCalibsToEPROM_Raw function.
**
*/
uint8_t PmodToF_WriteCalibsToEPROM_User()
{
    return CALIB_WriteCalibsToEPROM_User(1);
}

/***    PmodToF_RestoreAllCalibsFromEPROM_Factory
**
**  Parameters:
**      none
**
**  Return Value:
**      uint8_t
**          ERRVAL_SUCCESS                  0       // success
**          ERRVAL_EPROM_MAGICNO            0xFD    // wrong magic number when reading data from EEPROM.
**          ERRVAL_EPROM_CRC                0xFE    // wrong checksum when reading data from EPROM
**          ERRVAL_EPROM_WRITE				0xFA    // failed to write EPROM over I2C communication
**          ERRVAL_ToF_READ					0xF6    // failed to read ISL29501 registers over I2C communication
**
**  Description:
**      This function restores the factory calibration data from EPROM.
**      This function reads factory calibration data from EPROM using CALIB_ReadCalibsFromEPROM_Raw local function 
**      and writes the calibration data into the user calibration area of EPROM using CALIB_WriteCalibsToEPROM_User local function.
**      As the "skip_write_regs" parameter from CALIB_ReadCalibsFromEPROM_Raw function is 0, calibration data will be written
**      into ToF registers. If "skip_write_regs"= 1, calibration data will not be written into ToF registers.
**      The function returns ERRVAL_SUCCESS for success or the error codes provided by  
**		CALIB_ReadCalibsFromEPROM_Raw and CALIB_WriteCalibsToEPROM_Raw.
**
*/
uint8_t PmodToF_RestoreAllCalibsFromEPROM_Factory()
{
    uint8_t bErrCode;
    bErrCode = CALIB_ReadCalibsFromEPROM_Raw(&calib, (uint8_t)ADR_EPROM_FACTCALIB, 0);
    if(bErrCode == ERRVAL_SUCCESS)
    {
    	CALIB_WriteCalibsToEPROM_User(0);
        sprintf(szMsg, "Factory calibration restored.");
        return bErrCode;
    }
    else
    {
		return bErrCode;
    }
}



/***    PmodToF_ReadCalibsFromEPROM_User
**
**  Parameters:
**      none
**
**  Return Value:
**      uint8_t
**          ERRVAL_SUCCESS                  0       // success
**          ERRVAL_EPROM_MAGICNO            0xFD    // ERROR, wrong magic number when reading data from EEPROM.
**          ERRVAL_EPROM_CRC                0xFE    // wrong checksum  when reading data from EEPROM.
**
**  Description:
**      This function reads the user calibration data from EEPROM.
**      It calls the local function CALIB_ReadAllCalibsFromEPROM_Raw providing the address of user calibration area in EPROM,
**      The function returns ERRVAL_SUCCESS for success or the error codes provided by CALIB_ReadCalibsFromEPROM_Raw function.
**
*/
uint8_t PmodToF_ReadCalibsFromEPROM_User()
{
    return CALIB_ReadCalibsFromEPROM_Raw(&calib, (uint8_t)ADR_EPROM_CALIB,0);

}

/***    PmodToF_ReadSerialNoFromEPROM
**
**  Parameters:
**      char *pSzSerialNo - the pointer used to store the retrieved serial number.
**
**  Return Value:
**      uint8_t
**          ERRVAL_SUCCESS              0       // success
**          ERRVAL_EPROM_CRC            0xFE    // wrong CRC when reading data from EPROM
**          ERRVAL_EPROM_MAGICNO        0xFD    // wrong Magic No. when reading data from EPROM
**
**  Description:
**      This function reads the serial number data from EPROM and stores the data in the serialNo global data structure.
**		It copies the 12 serial number characters into the pSzSerialNo parameter.
**      The function returns ERRVAL_SUCCESS for success.
**      The function returns ERRVAL_EPROM_MAGICNO when a wrong magic number was detected in the data read from EPROM.
**      The function returns ERRVAL_EPROM_CRC when the checksum is wrong for the data read from EEPROM.
**
*/
uint8_t PmodToF_ReadSerialNoFromEPROM(char *pSzSerialNo)
{
    uint8_t bCrc, bCrcRead=0;
    // read calibration structure
    EPROM_PageRead(&myEPROMDevice, ADR_EPROM_SERIALNO, (uint8_t *)&serialNo, sizeof(SERIALNODATA));


    // check CRC
    bCrcRead = serialNo.crc;

    serialNo.crc = 0;

    bCrc = GetBufferChecksum((uint8_t *)&serialNo, sizeof(SERIALNODATA));

    serialNo.crc = bCrcRead;

    if(serialNo.magic != EPROM_MAGIC_NO)
    {
        // missing magic number
        return ERRVAL_EPROM_MAGICNO;
    }

    if(serialNo.crc != bCrc)
    {
        // CRC error
        return ERRVAL_EPROM_CRC;
    }
    strncpy(pSzSerialNo, serialNo.rgchSN, SERIALNO_SIZE);   // copy 12 chars of serial number from serialNo to the destination string
    pSzSerialNo[SERIALNO_SIZE] = 0; // terminate string
    return ERRVAL_SUCCESS;
}

/***    CALIB_WriteAllCalibsToEPROM_Raw
**
**  Parameters:
**      uint8_t baseAddr        - the address where the calibration data  will be written in EPROM
**		uint8_t skip_read_regs - Enables reading current data from 13 ToF calibration registers into the calib global structure
**
**  Return Value:
**      uint8_t
**          ERRVAL_SUCCESS                  
**			ERRVAL_EPROM_WRITE
**			ERRVAL_ToF_READ
**
**
**  Description:
**      This local function writes calibration data to a specific location in EPROM. 
**      Depending on skip_read_regs it reads current data from 13 ToF calibration registers into the calib global structure using ISL29501_ReadIIC function.
**      The 16 bytes payload to be written to EPROM also stores a signature byte called magic number (0xEB), one dummy byte 
**      and the checksum of the active payload (previous 15 bytes).
**      The function calls EPROM_PageWrite in order to write the 16 bytes to EPROM address specified in baseAddr parameter.
**      This function is called by PmodToF_WriteAllCalibsToEPROM_User, which provides user calibration area EPROM address as parameter.
**      This function shouldn't be called by user, instead, the user should call PmodToF_WriteAllCalibsToEPROM_User.
**      The function returns ERRVAL_SUCCESS for success or the error codes provided by ISL29501_ReadIIC and EPROM_PageWrite functions.
**
*/
uint8_t CALIB_WriteCalibsToEPROM_Raw(uint8_t baseAddr, uint8_t skip_read_regs)
{
    uint8_t bResult;
    // fill the calib register values
    if (skip_read_regs == 0 )
    	bResult=CALIB_Read_ISL29501_Regs();
    if(bResult!= ERRVAL_SUCCESS)
    	return bResult;

    calib.magic = EPROM_MAGIC_NO;
    calib.dummy = 0;
    calib.crc = 0;  // neutral value for the checksum
    calib.crc = GetBufferChecksum((uint8_t *)&calib, sizeof(calib));
    bResult = EPROM_PageWrite(&myEPROMDevice, baseAddr, (uint8_t *)&calib, sizeof(calib));
    usleep(10000);   // 10 ms
    return bResult;
}

/***    CALIB_ReadCalibsFromEPROM_Raw
**
**  Parameters:
**      CALIBDATA *pCalib   - pointer to CALIB structure where data will be read from EPROM
**      uint8_t baseAddr    - the EPROM address from where the calibration data read
**                  This will distinguish between user and factory calibration areas
**
**
**  Return Value:
**      uint8_t
**          ERRVAL_SUCCESS                  0       // success
**          ERRVAL_EPROM_MAGICNO            0xFD    // wrong Magic No. when reading data from EEPROM
**          ERRVAL_EPROM_CRC                0xFE    // wrong CRC when reading data from EEPROM
**			ERRVAL_EPROM_READ 				0xF9    // failed to read EEPROM over I2C communication
**
**  Description:
**      This local function reads calibration data from a specific location in EPROM.
**      It is called by PmodToF_ReadAllCalibsFromEPROM_User and PmodToF_RestoreAllCalibsFromEPROM_Factory,
**      which provide proper address in EPROM for user and factory calibration areas.
**      This function shouldn't be called by user, instead, the user should call the specified functions.
**		The function checks the signature byte called magic number and the checksum of the retrieved content.
**      The function returns ERRVAL_SUCCESS for success.
**      The function returns ERRVAL_EPROM_MAGICNO when a wrong magic number was detected in the data read from EPROM.
**      The function returns ERRVAL_EPROM_CRC when the checksum is wrong for the data read from EPROM.
**		The function also may return the error codes provided by EPROM_ReadIIC function.
**
*/
uint8_t CALIB_ReadCalibsFromEPROM_Raw(CALIBDATA *pCalib, uint8_t baseAddr, uint8_t skip_write_regs)
{
    uint8_t bCrc, bCrcRead,result;
    // read calibration structure
    result=EPROM_PageRead(&myEPROMDevice, baseAddr, (uint8_t *)pCalib, sizeof(CALIBDATA));
    if(result != ERRVAL_SUCCESS)
    {
        return result;
    }
    // check CRC
    bCrcRead = pCalib->crc;
    pCalib->crc = 0;

    bCrc = GetBufferChecksum((uint8_t *)pCalib, sizeof(CALIBDATA));

    pCalib->crc = bCrcRead;

    if(pCalib->magic != EPROM_MAGIC_NO)
    {
        // missing magic number
        return ERRVAL_EPROM_MAGICNO;
    }

    if(pCalib->crc != bCrc)
    {
        // CRC error
        return ERRVAL_EPROM_CRC;
    }
    if (skip_write_regs == 0)
    	CALIB_Write_ISL29501_Regs();

    return ERRVAL_SUCCESS;
}
/***    CALIB_Read_ISL29501_Regs
**
**  Parameters:
**			none
**
**  Return Value:
**	uint8_t		ERRVAL_SUCCESS      0       // success 
**	uint8_t		ERRVAL_ToF_READ		0xF6    // failed to read ISL29501 registers over I2C communication
**  Description:
**      This local function reads calibration data from the 13 ISL29501 calibration registers starting at 0x24 address 
**		into the calib global structure, using the ISL29501_ReadIIC function.
**      The function returns ERRVAL_SUCCESS for success or the error codes provided by ISL29501_ReadIIC function.
**
*/
uint8_t CALIB_Read_ISL29501_Regs()
{
	 uint8_t result;
     result = ISL29501_ReadIIC(&myToFDevice, ADR_OFFSET_CALIB_REG_ISL29501, calib.regs, 13);
     return result;
}

/***    CALIB_Write_ISL29501_Regs
**
**  Parameters:
**			none
**
**  Return Value:
**	uint8_t		ERRVAL_SUCCESS		0       // success
**	uint8_t		ERRVAL_ToF_WRITE	0xF8    // failed to write ISL29501 registers over I2C communication
**  Description:
**      This local function writes calibration data from the calib global structure 
**		to the 13 ISL29501 calibration registers starting at 0x24 address, using the ISL29501_WriteIIC function.
**      The function returns ERRVAL_SUCCESS for success or the error codes provided by ISL29501_WriteIIC function.
**
*/
uint8_t CALIB_Write_ISL29501_Regs()
{
	uint8_t result=ERRVAL_SUCCESS;
    for(int i = 0; i < 13; i++)
    {
        result=ISL29501_WriteIIC(&myToFDevice, ADR_OFFSET_CALIB_REG_ISL29501 + i, calib.regs + i, 1);
        if(result != ERRVAL_SUCCESS)
        return result;
    }
    return result;
}


/***    GetBufferChecksum
**
**  Synopsis:
**      GetBufferChecksum(*pBuf, len)
**
**  Parameters:
**      pBuf - buffer for which the checksum is computed
**      len - buffer length on which the checksum is computed
**
**  Return Values:
**      returns the value of checksum, computed for the specified pBuf, on the specified len
**
**  Description:
**      This function computes the one byte checksum by adding the specified number of consecutive bytes from the specified location.
**      Returns the value of checksum.
**
*/

unsigned char GetBufferChecksum(unsigned char *pBuf, int len)
{
    int i;
    unsigned char checksum = 0;
    for(i= 0; i < len; i++)
    {
        checksum += pBuf[i];
    }
    return checksum;
}

/***
** _3bytes_to_double(u8 exp, u8 msb, u8 lsb)
**
**  Parameters:
**      exp - the exponent for Double-precision binary floating-point format
**      msb - most significant byte for Double-precision binary floating-point format
**		lsb - least significant byte for Double-precision binary floating-point format
**  Return Values:
**      Double number
**  Description:
**      This function converts 3 bytes into double format number.
**      Returns a double format number.
**
*/
double _3bytes_to_double(u8 exp, u8 msb, u8 lsb)
{
    double result = 0;
    int iMantissa = 0;
    // flag for negative numbers
    uint8_t negative = 0u;

    // negative number
    if (msb > 127)
        negative = 1u;

    iMantissa = msb << 8;
    iMantissa |= lsb;
    if (negative) {
        // convert from 2's complement
        iMantissa = ((iMantissa - 1) ^ 0xFFFF);
        // combine mantissa and exponent
        result = -iMantissa * pow(2, exp);
    } else {
        result = iMantissa * pow(2, exp);
    }

    return result;
}
/***
** _2bytes_to_double(u8 msb, u8 lsb)
**
**  Parameters:
**      msb - most significant byte
**		lsb - least significant byte
**  Return Values:
**      Double number
**
**  Description:
**      This function converts 2 bytes into double format number.
**      Returns a double format number.
**
*/
double _2bytes_to_double(u8 msb, u8 lsb)
{
    return (double)(unsigned short)(((int)(msb) << 8) | (int)(lsb));
}
/***
** double_to_3bytes(double dNum, u8* exp, u8* msb, u8* lsb)
**
**  Parameters:
**  	dNum - double number which will be converted to 3 bytes
**      exp - the exponent for Double-precision binary floating-point format
**      msb - most significant byte for Double-precision binary floating-point format
**		lsb - least significant byte for Double-precision binary floating-point format
**  Return Values:
**		none
**  Description:
**      This function converts double number to 3 bytes.
**
*/
void double_to_3bytes(double dNum, u8* exp, u8* msb, u8* lsb)
{
    double dNumLog = 0;
    double dMantissa = 0;
    int iMantissa = 0;
    uint8_t negative = 0u;
    uint8_t first_exp, new_exp = 0;
    uint8_t a;

    // handle negative numbers
    if (dNum < 0) {
        // set negative flag
        negative = 1u;
        // convert to positive
        dNum = fabs(dNum);
    }

    // log base 2 of input
    dNumLog = log2(dNum);
    // exponent of the double
    first_exp = (uint8_t) dNumLog;
    // log of mantissa
    dMantissa = (dNumLog - (double) first_exp);
    // convert mantissa to double
    dMantissa = pow(2, dMantissa);
    // start new exponent as the original exponent
    new_exp = first_exp;

    // it might seem like 15 shifts is the correct number but it's 14.
    // Doing 15 shifts into the sign bit making it a negative number.
    // convert mantissa to whole number
    for (a = 1; a <= first_exp && a < 15; ++a) {
        // double the mantissa
        dMantissa = dMantissa * 2;
        // decrement the exponent
        --new_exp;
    }

    if (negative) {
        // take 2's complement, convert to short
        iMantissa = (int) (-dMantissa);
    } else {
        // convert to short
        iMantissa = (int) (dMantissa);
    }

    *exp = new_exp;
    *msb = (uint8_t) ((iMantissa & 0xFF00) >> 8);
    *lsb = (uint8_t) (iMantissa & 0x00FF);
}
/***
** double_to_2bytes(double dNum, u8* msb, u8* lsb)
**
**  Parameters:
**  	dNum - double number which will be converted to 3 bytes
**      msb - most significant byte for Double-precision binary floating-point format
**		lsb - least significant byte for Double-precision binary floating-point format
**  Return Values:
**		none
**
**  Description:
**      This function converts double number to 2 bytes.
**
*/
void double_to_2bytes(double dNum, u8* msb, u8* lsb)
{
    int iNum = (int) dNum;
    *msb = (uint8_t) ((iNum & 0x0000FF00) >> 8);
    *lsb = (uint8_t) (iNum & 0x000000FF);
}
