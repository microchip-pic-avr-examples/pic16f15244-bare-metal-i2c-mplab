<!-- Please do not change this logo with link -->

[![MCHP](images/microchip.png)](https://www.microchip.com)

# Bare Metal I<sup>2</sup>C Drivers for the PIC16F15244 Family of MCUs

In this example, bare metal I<sup>2</sup>C drivers for the MSSP peripheral on the PIC16F15244 family of MCUs will be created.

## Software Used

- [MPLAB® X IDE v5.50.0 or newer](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-i2c-mplab-github)
  - [PIC16F1xxxx_DFP v1.8.149](https://packs.download.microchip.com/)
- [MPLAB XC8 v2.31.0 or newer compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-i2c-mplab-github)

## Hardware Used

To run this project, a PIC16F15243/PIC16F15244/PIC16F15245 is required, but other devices with an MSSP should also function with *adjustments to the pin assignments*.

This project was tested and built on a PIC16F15244 Curiosity Nano.

- [PIC16F15244 Curiosity Nano (EV09Z19A)](https://www.microchip.com/Developmenttools/ProductDetails/EV09Z19A?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-i2c-mplab-github)
- [Curiosity Nano Base for Click boards™ (AC164162)](https://www.microchip.com/Developmenttools/ProductDetails/AC164162?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-i2c-mplab-github)

Alternatively, these tools should also be compatible:

- [Curiosity LPC board (DM164137)](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM164137?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-i2c-mplab-github)
- [PIC16F15243/PIC16F15244/PIC16F15245 MCU in a 20-pin DIP package](https://www.microchipdirect.com/Chart.aspx?branchId=10135&mid=10&treeid=1&utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-i2c-mplab-github)

**Important! External Pull-up resistors were used. Internal Pull-up resistors can be enabled by uncommenting `#define USE_INTERNAL_PULLUPS` in either `mssp1_client.h` or `mssp1_host.h`.**

### Host Mode Testing

To use the I<sup>2</sup>C host driver, an I<sup>2</sup>C client device is required. This can be created using another MCU in client mode, or by using a Click adapter board. A number of compatible Click boards are available from [mikroE](https://www.mikroe.com/click).

For testing, a PIC16F15245 running the [Advanced I/O Expander](https://github.com/microchip-pic-avr-examples/pic16f15244-family-advanced-i2c-io-expander) example was used to test host mode operation.

### Client Mode Testing

To use the I<sup>2</sup>C client driver, a device capable of generated I<sup>2</sup>C host communication is required. This can be another MCU configured as a client, or this can be a stand-alone device, such as an [MCP2221A USB-I<sup>2</sup>C Breakout Module (ADM00559)](https://www.microchip.com/DevelopmentTools/ProductDetails/adm00559), which was used for testing.

## Pin Setup

This example uses pins RB4 and RB6 for I<sup>2</sup>C communication. These are the default pins used on the Curiosity Nano Adapter board for I<sup>2</sup>C.

| Pin | Function |
| --- | --------
| RB4 | SDA
| RB6 | SCL

## Using the I<sup>2</sup>C Host Driver  

The I<sup>2</sup>C Host Driver is a *polled* driver that can initiate communication with I<sup>2</sup>C clients. This driver is composed of 2 files: *mssp1_host.h* and *mssp1_host.c*  

### Initializing the Driver

The driver is initialized by calling the functions:

```
//Init I/O Settings  
initI2CPins();

//Init MSSP Module in I2C Host Mode
MSSP_HostInit();
```  

### Writing to Clients

There are 2 functions that write data to the client device:

```
bool MSSP_WriteByte(uint8_t deviceADDR, uint8_t data);
bool MSSP_WriteBlock(uint8_t deviceADDR, uint8_t* blockMem, uint8_t memSize);
```

The `MSSP_WriteByte` function sends the 7-bit address in `deviceADDR` to the client. If the client ACKs this address, then  `data` is sent.

Similiarly, the `MSSP_WriteBlock` function sends the 7-bit address in `deviceADDR` to the client. If the client ACKs this address, then `memSize` bytes are sent from `blockMem`.

If the client does not ACK, then the function returns false and no data is sent.

### Reading Data from Clients

There are 3 functions that are designed to read data from the client.

```
bool MSSP_RegisterSelectAndRead(uint8_t deviceADDR, uint8_t registerADDR, uint8_t* blockMem, uint8_t memSize);
bool MSSP_ReadByte(uint8_t deviceADDR, uint8_t* data);
uint8_t MSSP_ReadByteNoWarn(uint8_t deviceAddr);
bool MSSP_ReadBlock(uint8_t deviceADDR, uint8_t* blockMem, uint8_t memSize);
```

#### Register Select and Read

`MSSP_RegisterSelectAndRead` is a special function in this driver. Unlike the other read functions, this one starts by starting an I<sup>2</sup>C write to the device at `deviceADDR`. Then, `registerADDR` is written to the device to select the address to read from. After writing that byte, the I<sup>2</sup>C bus is restarted, and the client is readdressed, but in read mode. `memSize` bytes of data are read from the device and stored in `blockMem`.

If the client NACKs at either of the addressing steps in this function, the operation will be aborted, and the function will return false.

#### Read Byte and Block

```
bool MSSP_ReadByte(uint8_t deviceADDR, uint8_t* data);
uint8_t MSSP_ReadByteNoWarn(uint8_t deviceADDR);
bool MSSP_ReadBlock(uint8_t deviceADDR, uint8_t* blockMem, uint8_t memSize);
```

These functions all operate on a nearly identical basis. A client device is addressed with `deviceADDR` in read mode. Then, either a single byte or `memSize` bytes of data are read from the device.

No read occurs if the client NACKs communication. Boolean functions return false in this case. The function `MSSP_ReadByteNoWarn` is a wrapper for the function `MSSP_ReadByte`. If the client NACKs, then 0x00 is returned, rather than true or false values.

### API Functions

| Function Definition | Description
| ------------------- | --------
| void initI2CPins(void) | Initializes the I<sup>2</sup>C pins for the MSSP. Uses RB4 and RB6.
| void MSSP_HostInit(void) | Initializes the driver in I<sup>2</sup>C host mode.
| bool MSSP_RegisterSelectAndRead(uint8_t deviceADDR, uint8_t registerADDR, uint8_t* blockMem, uint8_t memSize) | Addresses a client device at `deviceADDR`, writes 1 byte of data (`registerADDR`), RESTARTs the bus, readdresses the client device, and reads `memSize` bytes back. Returns true if successful or false if the client NACKed.
| bool MSSP_WriteByte(uint8_t deviceADDR, uint8_t data) | Addresses a client device at `deviceADDR` and writes 1 byte (`data`) to the client. Returns true if successful or false if the client NACKed.
| bool MSSP_ReadByte(uint8_t deviceADDR, uint8_t* data) | Addresses a client device at `deviceADDR` and reads a single byte of data from this device into `data`. Returns true if successful or false if the client NACKed.
| uint8_t MSSP_ReadByteNoWarn(uint8_t deviceADDR) | Addresses a client device at `deviceADDR` and returns a single byte of data read from the device. If the client NACKed, the byte returned will be 0x00.
| bool MSSP_WriteBlock(uint8_t deviceADDR, uint8_t* blockMem, uint8_t memSize) | Addresses a client device at `deviceADDR` and writes `memSize` bytes of data from `blockMem` to it. Returns true if successful or false if the client NACKed.
| bool MSSP_ReadBlock(uint8_t deviceADDR, uint8_t* blockMem, uint8_t memSize) | Addresses a client device at `deviceADDR` and reads `memSize` bytes from the client into `blockMem`. Returns true if successful or false if the client NACKed.

## Using the I<sup>2</sup>C Client Driver

I<sup>2</sup>C clients are devices that respond to a read/write request from an I<sup>2</sup>C host. Since a host does not communicate continuously, *interrupt* driven operation is crucial for a client device.

This driver can be used either in byte mode or block mode (via an included middleware library).

Byte Driver &lrarr; Application Code

Byte Driver &lrarr; Block Memory Middleware &lrarr; Application Code  

### Byte Mode Driver

Byte mode is designed to call a user-defined function for all byte read/write events and for the I<sup>2</sup>C stop event. If one of these user defined functions is not set (or set to NULL), the driver will perform a default action, as shown below, to maintain functionality.

| Event | Default Action
| ----- | --------------
| Read  | Writes 0x00 to the Host
| Write | Discards Received Byte
| Stop  | Does Nothing

To write a function that can be called for these events, they must have the following definition:

| Event | Required Function Definition |
| ----- | ----------------  
| Read  | uint8_t myReadFunction(void)
| Write | void myWriteFunction(uint8_t data)
| Stop  | void myStopFunction(void)

Like all interrupt handlers, the functions associated should be as small as possible and ***non-blocking***.

*Note: For Read Events, n + 1 events will occur, due to the host ACKing the communication before the STOP event. The block mode driver corrects it's index for this event.*

#### API Functions (mssp1_client.h)

| Function Definition | Description
| ------------------- | --------
| void initI2CPins(void) | Initializes the I<sup>2</sup>C pins for the MSSP. Uses RB4 and RB6.
| void MSSP_ClientInit(uint8_t address) | Initializes the driver in I<sup>2</sup>C client mode.
| void _MSSP_ProcessInterrupt(void) | This function is called from the ISR function to process MSSP interrupts.
| void assignMSSPByteWriteHandler(void (*writeHandler)(uint8_t)) | This sets the function that is called for I<sup>2</sup>C write events.
| void assignMSSPByteReadHandler(uint8_t (*readHandler)(void)) | This sets the function that is called for I<sup>2</sup>C read events.
| void assignMSSPStopHandler(void (*stopHandler)(void)) | This sets the function that is called for I<sup>2</sup>C stop events.

### Block Mode Middleware

Block mode simplifies development by implementing block memory transfers, with support for both incremental transfers (byte 0, 1, 2, etc...) and addressed transfers (set index to 4, RESTART/STOP, read 4, read 5, read 6, etc...).

To disable addressed transfers, comment out `#define FIRST_BYTE_ADDR` in *mssp1_blockData.h*.

#### Setting up Block Mode

To use the block mode middleware, it must be attached to the byte mode driver, shown below:

~~~
//Init the I2C Pins on the Device
initI2CPins();

//Initialize the I2C Driver
MSSP_ClientInit(0x60);

//Assign ISRs for block mode operation
assignMSSPByteWriteHandler(&_MSSP_StoreByte);
assignMSSPByteReadHandler(&_MSSP_RequestByte);
assignMSSPStopHandler(&_onMSSPStop);
~~~

The functions `_MSSP_StoreByte`, `_MSSP_RequestByte`, and `_onMSSPStop` are functions provided in the library. These functions are responsible for storing and retrieving bytes of data from the user defined memory blocks. The stop function (`_onMSSPStop`) is used to adjust (or reset) the memory access index.

*Note: In the event that the memory blocks are not initialized, or a read/write overflow occurs, the driver will discarded any further received bytes (write) or will return 0x00 to the driver (read).*

#### Attaching Memory Blocks

This library supports separated read and write memory blocks for I<sup>2</sup>C with varying sizes between each block. The functions in the library verify the I<sup>2</sup>C does not exceed the size of each memory block.

Two functions are provided to set the blocks, as shown below.

~~~
//Setup a Read Buffer
setupReadBuffer(&buffer[0], 8);

//Setup a Write Buffer. (For testing, set to smaller than read.)
setupWriteBuffer(&buffer[0], 4);
~~~

In the above example, both the read and write buffers point to the same memory pool, however separate arrays can also be used. If the write function should only modify a select number of values, the address of the buffer can be modified as such:

~~~
//Setup a Read Buffer
setupReadBuffer(&buffer[0], 8);

//Setup a Write Buffer
setupWriteBuffer(&buffer[4], 4);
~~~

In this configuration, memory writes will occur from the 4th byte to the 7th byte, rather than starting at 0 and going to 3.

#### API Functions

| Function Definition | Description
| ------------------- | --------
| void _MSSP_StoreByte(uint8_t data) | Called by the byte mode driver to handle bytes received. **Do not call this function.**
| uint8_t _MSSP_RequestByte(void) | Called by the byte mode driver to get the next byte to send. **Do not call this function.**
| void _onMSSPStop(void) | Called by the byte mode driver on an I<sup>2</sup>C stop to adjust or reset the memory indexes. **Do not call this function.**
| void setupReadBuffer(volatile uint8_t* buffer, uint8_t size) | This function sets the read buffer to **SEND data from the client to the host**.
| void setupWriteBuffer(volatile uint8_t* buffer, uint8_t size) | This function sets the write buffer to **RECEIVE data from the host**.

## Summary

This example provides a simple bare-metal I<sup>2</sup>C driver for the MSSP peripheral to integrate into other projects.
