#include <SPI.h>
#include "Arduino_QSPI_Hal.h"
#include "warpper_SPI.h"

#ifdef SFUD_USING_QSPI
Adafruit_FlashTransport_QSPI QSPIdev;
#define QSPIDEV QSPIdev
void QSPIBegin(){
    QSPIDEV.begin();
}

void QSPISetClockSpeed(uint32_t clock_hz){
    QSPIDEV.setClockSpeed(clock_hz);
}

bool QSPIReadMemory(uint32_t addr, uint8_t *data, uint32_t len){
    return QSPIDEV.readMemory(addr,data,len);
}

bool QSPIWriteMemory(uint32_t addr, uint8_t *data, uint32_t len){
    return QSPIDEV.writeMemory(addr,data,len);
}

bool QSPIEraseCommand(uint8_t command,uint32_t address){
    return QSPIDEV.eraseCommand(command,address);
}

bool QSPIRunCommand(uint8_t command){
    return QSPIDEV.runCommand(command);
}

bool QSPIReadCommand(uint8_t command, uint8_t *response, uint32_t len){
    return QSPIDEV.readCommand(command,response,len);
}

bool QSPIWriteCommand(uint8_t command, uint8_t const *data, uint32_t len){
    return QSPIDEV.writeCommand(command,data,len);
}
#else
#define SPIDEV SPI
void SPIBegin(){
    SPIDEV.begin();
}

uint8_t SPITransfer(uint8_t data){
    return SPIDEV.transfer(data);
}

void SPICsInit(uint8_t pin, uint8_t mode){
    pinMode(pin, mode);
}

void SPICsControl(uint8_t pin, uint8_t val){
    digitalWrite(pin, val);
}
#endif