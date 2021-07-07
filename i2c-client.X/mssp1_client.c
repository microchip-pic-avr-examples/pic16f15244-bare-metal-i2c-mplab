#include "mssp1_client.h"

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// Temp register
static volatile uint8_t temp = 0;

//Functions to call on specific events
static void (*storeByte)(uint8_t) = NULL;
static uint8_t (*getByte)(void) = NULL;
static void (*I2CStopHandler) (void) = NULL;

void initI2CPins(void)
{
    //RB4 = SDA, RB6 = SCL
    TRISBbits.TRISB4 = 0b1;
    TRISBbits.TRISB6 = 0b1;
    
    //Disable analog mode
    ANSELBbits.ANSB4 = 0b0;
    ANSELBbits.ANSB6 = 0b0;
    
    //Enable Open-Drain Outputs
    ODCONBbits.ODCB4 = 0b1;
    ODCONBbits.ODCB6 = 0b1;
    
#ifdef USE_INTERNAL_PULLUPS
    
    //Settings for I2C Pins
    //Standard Slew Rate, 2x standard pull-up current, I2C Thresholds
    
    RB4I2C = 0b00010001;
    RB6I2C = 0b00010001;
    
#endif
    
    //PPS Settings
    RB4PPS = 0x08;
    RB6PPS = 0x07;
    
    SSP1DATPPS = 0b01100;
    SSP1CLKPPS = 0b01110;
}

void MSSP_ClientInit(uint8_t address)
{
    //Reset Registers
    SSP1CON1 = 0x00;
    SSP1CON2 = 0x00;
    SSP1CON3 = 0x00;
    SSP1STAT = 0x00;
        
    //Disable slew control for Standard mode
    SSP1STATbits.SMP = 1;
    
    //Set MSSP Operating Mode (7-bit Client)
    SSP1CON1bits.SSPM = 0b0110;
    
    //Enable clock stretching
    SSP1CON2bits.SEN = 1;
    
    //Enable bus collision interrupts
    SSP1CON3bits.SBCDE = 1;
    
    //Enable STOP Interrupts
    SSP1CON3bits.PCIE = 1;
    
    //Load client address
    SSP1ADD = (address << 1);
    
    //Clear Bus Collision interrupt flag
    PIR1bits.BCL1IF = 0;
    
    //Clear the SSP interrupt flag
    PIR1bits.SSP1IF = 0;
    
    //Enable BCLIF
    PIE1bits.BCL1IE = 1;
    
    //Enable SSPIF
    PIE1bits.SSP1IE = 1;
    
    //Enable the module
    SSP1CON1bits.SSPEN = 1;
}

void _MSSP_ProcessInterrupt(void)
{
    //Check for SSPIF
    if(PIR1bits.SSP1IF)                                                         
    {
        if (SSP1STATbits.P)
        {
            //Stop Condition
            
            if (I2CStopHandler != NULL)
            {
                //If valid, call the I2C Stop Event Function
                I2CStopHandler();
            }
        }
        else if(SSP1STATbits.R_nW)
        {
            //Host wants to read (client transmit)
            if (getByte != NULL)
            {
                //Get the byte to send
                SSP1BUF = getByte();
            }
            else
            {
                //Write a default value
                SSP1BUF = 0x00;
            }
        }   
        else if(!SSP1STATbits.R_nW)
        {
            // Host wants to write (client receive)
            if(!SSP1STATbits.D_nA)                                              
            {
                //Last byte was an address
                
                //Clear the Buffer Full (BF) flag
                temp = SSP1BUF;
            }
            if(SSP1STATbits.D_nA)
            {
                //Last byte was data
                
                if (storeByte != NULL)
                {
                    //Store the byte from the host
                    storeByte(SSP1BUF);
                }
                else
                {
                    //Clear the Buffer Full (BF) flag
                    temp = SSP1BUF;
                }
            }
        }
    }
    if(PIR1bits.BCL1IF)
    {
        //Clear the Buffer Full (BF) flag
        temp = SSP1BUF;
        
        // Clear BCLIF
        PIR1bits.BCL1IF = 0;
    }
    
    //Release Clock Stretch
    SSP1CON1bits.CKP = 1;
    
    //Clear SSP1IF
    PIR1bits.SSP1IF = 0;
}

void assignMSSPByteWriteHandler(void (*writeHandler)(uint8_t))
{
    storeByte = writeHandler;
}

void assignMSSPByteReadHandler(uint8_t (*readHandler)(void))
{
    getByte = readHandler;
}

void assignMSSPStopHandler(void (*stopHandler)(void))
{
    I2CStopHandler = stopHandler;
}
