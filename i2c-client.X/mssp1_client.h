/*
Copyright (c) [2012-2020] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/

#ifndef MSSP1_CONFIG_H
#define	MSSP1_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
#include <stdbool.h>
    
//If set, the I2C pins are set to use the pull-up resistors
//#define USE_INTERNAL_PULLUPS
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> initI2CPins(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * Initializes the pins used by the MSSP for I2C communication.
     */
    void initI2CPins(void);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> MSSP_ClientInit(<FONT COLOR=BLUE>uint8_t</FONT> address)</B>
     * @param address (uint8_t) - Sets the address the I2C client listens to
     * 
     * Initializes the MSSP in I2C client mode, with the address as set.
     * This driver is interrupt driven. 
     */
    void MSSP_ClientInit(uint8_t address);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> _MSSP_ProcessInterrupt(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * Process the interrupt from the ISR.
     */
    void _MSSP_ProcessInterrupt(void);

    /**
     * <b><FONT COLOR=BLUE>void</FONT> assignMSSPByteWriteHandler(<FONT COLOR=BLUE>void</FONT> (*writeHandler)(uint8_t))</B>
     * @param writeHandler (void (*writeHandler)(uint8_t)) - Pointer to write handler
     * 
     * Sets the function that handles write data from the host.
     * To properly handle writes, this function must be set.
     * 
     * If not set, bytes written to this device will be discarded.
     */
    void assignMSSPByteWriteHandler(void (*writeHandler)(uint8_t));
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> assignMSSPByteReadHandler(<FONT COLOR=BLUE>uint8_t</FONT> (*readHandler)(uint8_t))</B>
     * @param readHandler (uint8_t (*readHandler)(void)) - Pointer to read handler
     * 
     * Sets the function that handles read requests from the host. 
     * To properly handle reads, this function must be set. 
     * 
     * If not set, bytes sent to the host will be the default value (0x00).
     */
    void assignMSSPByteReadHandler(uint8_t (*readHandler)(void));
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> assignMSSPStopHandler(<FONT COLOR=BLUE>void</FONT> (*stopHandler)(void))</B>
     * @param stopHandler (void (*stopHandler)(void)) - Pointer to stop condition handler
     * 
     * Sets the function that is called on a STOP condition.
     * This function is not required for driver functionality.
     */
    void assignMSSPStopHandler(void (*stopHandler)(void));

#ifdef	__cplusplus
}
#endif

#endif	/* MSSP1_CONFIG_H */

