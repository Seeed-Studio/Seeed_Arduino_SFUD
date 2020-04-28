#include <SPI.h>
#include "qspi/Arduino_QSPI_Hal.h"
#include "warpper_SPI.h"


void SPIBegin(){
    SPI.begin();
}

uint8_t SPITransfer(uint8_t data){
    return SPI.transfer(data);
}

void SPICsInit(uint8_t pin, uint8_t mode){
    pinMode(pin, mode);
}

void SPICsControl(uint8_t pin, uint8_t val){
    digitalWrite(pin, val);
}

#ifdef SFUD_USING_QSPI

Adafruit_FlashTransport_QSPI QSPIdev;

void QSPIBegin(){
    QSPIdev.begin();
}

void QSPISetClockSpeed(uint32_t clock_hz){
    QSPIdev.setClockSpeed(clock_hz);
}

bool QSPIReadMemory(uint32_t addr, uint8_t *data, uint32_t len){
    return QSPIdev.readMemory(addr,data,len);
}

bool QSPIWriteMemory(uint32_t addr, uint8_t *data, uint32_t len){
    return QSPIdev.writeMemory(addr,data,len);
}

bool QSPIEraseCommand(uint8_t command,uint32_t address){
    return QSPIdev.eraseCommand(command,address);
}

bool QSPIRunCommand(uint8_t command){
    return QSPIdev.runCommand(command);
}

bool QSPIReadCommand(uint8_t command, uint8_t *response, uint32_t len){
    return QSPIdev.readCommand(command,response,len);
}

bool QSPIWriteCommand(uint8_t command, uint8_t const *data, uint32_t len){
    return QSPIdev.writeCommand(command,data,len);
}
#endif