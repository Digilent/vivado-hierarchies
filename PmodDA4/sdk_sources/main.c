/******************************************************************************/
/*                                                                            */
/* main.c -- Definition for PmodDA4 demo                                      */
/*                                                                            */
/******************************************************************************/
/* Author: Ana-Maria Balas                                                    */
/* Copyright 2019, Digilent Inc.                                              */
/******************************************************************************/
/* File Description:                                                          */
/*                                                                            */
/* This file defines a demonstration for the use of the PmodDA4 Hierarchical  */
/* Block. A 2.5 V peak-to-peak triangle wave of an unspecified frequency is   */
/* output at channel A.										                  */
/*                                                                            */
/* To connect to UART, use a serial terminal such as TeraTerm attached to the */
/* programmed FPGA's port at the proper Baud rate.                            */
/*                                                                            */
/*    UART TYPE BAUD RATE                                                     */
/*    uartns550 9600                                                          */
/*    uartlite  Configurable only in HW design                                */
/*    ps7_uart  115200 (configured by bootrom/bsp)                            */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    06/20/2016(JonP):     Created                                           */
/*    04/28/2017(ArtVVB):   Validated                                         */
/*    02/13/2018(atangzwj): Validated for Vivado 2017.4                       */
/*                                                                            */
/******************************************************************************/

#include "PmodDA4/PmodDA4.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "xparameters.h"

void DemoInitialize();
void DemoRun();
void DemoCleanup();
void EnableCaches();
void DisableCaches();

PmodDA4 myDevice;

int main(void) {
//   xil_printf("Starting Pmod DA4 demo...\n\r");
   DemoInitialize();
   DemoRun();
   DemoCleanup();
   return 0;
}

void DemoInitialize() {
   EnableCaches();
   DA4_begin(&myDevice, XPAR_SPI_0_BASEADDR);
   DA4_reset(&myDevice);
   DA4_set_int_ref(&myDevice);
   DA4_load_clear_mode(&myDevice);
}

void DemoRun() {
   float dMaxValue = 2.5;
   float dMinValue = 0.0;
   float dStep = 0.05;
   float dValue;

   xil_printf("Starting Pmod DA4 demo...\n\r");
   while (1) {
      // Increase physical value from minimum to maximum value
      for (dValue = dMinValue; dValue <= dMaxValue; dValue += dStep) {
         // Send value to the DA converter
         DA4_WritePhysicalValue(&myDevice, dValue, CH_A);
      }

      // Decrease physical value from maximum to minimum value
      for (dValue = dMaxValue; dValue >= dMinValue; dValue -= dStep) {
         // Send value to the DA converter
         DA4_WritePhysicalValue(&myDevice, dValue, CH_A);
      }
   }
}

void DemoCleanup() {
   DA4_end(&myDevice);
   DisableCaches();
}

void EnableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheEnable();
#endif
#endif
}

void DisableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheDisable();
#endif
#endif
}
