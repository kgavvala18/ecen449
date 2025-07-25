/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"
#include "multiply.h"

#define SLAVE_BASE_ADDR XPAR_MULTIPLY_0_S00_AXI_BASEADDR
//#define SLV_1_ADDR XPAR_PS7_AFI_1_S_AXI_BASEADDR
//#define SLV_2_ADDR XPAR_PS7_AFI_2_S_AXI_BASEADDR


int main()
{
    init_platform();

    //write to slv1 and slv0

    u32 input1;
    u32 input2;

    for ( input1 = 0x00000001; input1 < 0x00000011; input1++ ){
    	for (input2= 0x00000001; input2 < 0x00000011; input2++){


    		MULTIPLY_mWriteReg(SLAVE_BASE_ADDR, 0, input1);
			MULTIPLY_mWriteReg(SLAVE_BASE_ADDR, 4, input2);

			u32 val1 = MULTIPLY_mReadReg(SLAVE_BASE_ADDR, 0);

			u32 val2 = MULTIPLY_mReadReg(SLAVE_BASE_ADDR, 4);


			//read from slv_reg2
			u32 value = MULTIPLY_mReadReg(SLAVE_BASE_ADDR, 8);


			xil_printf("%u times %u = result: %u\n\r", val1,val2, value);

    	}


    }

    print("Hello World\n\r");
    print("Successfully ran Hello World application");
    cleanup_platform();
    return 0;
}
