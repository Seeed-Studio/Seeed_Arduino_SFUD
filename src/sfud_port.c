/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>
#include <stdarg.h>
#include"warpper_SPI.h"

static char log_buf[256];

int chipSelectPin = 1;

void sfud_log_debug(const char *file, const long line, const char *format, ...);
/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    sfud_spi *spidev = (sfud_spi *)spi;
    uint8_t send_data, read_data;
    if (write_size) {
        SFUD_ASSERT(write_buf);
    }
    if (read_size) {
        SFUD_ASSERT(read_buf);
    }
    
#ifdef SFUD_USING_QSPI

    if (read_size)
    {
        if (write_size > 4)
        {
            QSPIReadSFDP(write_buf[0],write_buf + 1,write_size - 1,read_buf,read_size + 1);
            for (uint8_t index = 0 ; index < read_size; index++)
            {
            read_buf[index] = read_buf[index + 1];
            }
        }else if (1 == write_size)
        {
            QSPIReadCommand(write_buf[0],read_buf,read_size);
        }
    }else
    {
        if (1 == write_size){
            QSPIRunCommand(write_buf[0]);
        }
        else if (write_size > 4)
        {
            uint32_t Address = (write_buf[1] << 16) | (write_buf[2] << 8) | (write_buf[3]);
            QSPIWriteMemory(Address,&write_buf[4],write_size - 4);         
        }
        else{
            uint32_t Address = (write_buf[1] << 16) | (write_buf[2] << 8) | (write_buf[3]);
            QSPIEraseCommand(write_buf[0],Address);      
        }
    }

#else
    SPICsControl(chipSelectPin, LOW);
    for (size_t i = 0, retry_times; i < write_size + read_size; i++) {
        if (i < write_size) {
            send_data = *write_buf++;
            SPITransfer(send_data);
        } else {
            read_data = SPITransfer(SFUD_DUMMY_DATA);
        }
        if (i >= write_size) {
            *read_buf++ = read_data;
        }        
    }
    SPICsControl(chipSelectPin, HIGH);
#endif /* SFUD_USING_QSPI */  
    /**
     * add your spi write and read code
     */

    return result;
}

#ifdef SFUD_USING_QSPI
/**
 * read flash data by QSPI
 */
static sfud_err qspi_read(const struct __sfud_spi *spi, uint32_t addr, sfud_qspi_read_cmd_format *qspi_read_cmd_format,
        uint8_t *read_buf, size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    sfud_spi *spidev = (sfud_spi *)spi;
    
    /**
     * add your qspi read flash data code
     */
    SFUD_INFO("QSPIReadMemory");
    if (qspi_read_cmd_format->instruction_lines == 1&& qspi_read_cmd_format->address_lines == 1 \
        && qspi_read_cmd_format->data_lines == 2){
        QSPIReadMemory(addr,read_buf,read_size);

    }else{
        result = SFUD_ERR_READ;
        return result;
    }

    return result;

}
#endif /* SFUD_USING_QSPI */
/*1s delay */
static void retry_delay_1s(void) {
    delay(1000);
}
sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your port spi bus and device object initialize code like this:
     * 1. rcc initialize
     * 2. gpio initialize
     * 3. spi device initialize
     * 4. flash->spi and flash->retry item initialize
     *    flash->spi.wr = spi_write_read; //Required
     *    flash->spi.qspi_read = qspi_read; //Required when QSPI mode enable
     *    flash->spi.lock = spi_lock;
     *    flash->spi.unlock = spi_unlock;
     *    flash->spi.user_data = &spix;
     *    flash->retry.delay = null;
     *    flash->retry.times = 10000; //Required
     */
#ifdef SFUD_USING_QSPI
    QSPIBegin();
    QSPISetClockSpeed(min(SFUD_W25Q32_MAX_SPEED,VARIANT_GCLK2_FREQ));
#else
    SPIBegin();
    SPICsInit(chipSelectPin, OUTPUT);
#endif
    flash->spi.wr = spi_write_read;
#ifdef SFUD_USING_QSPI
    flash->spi.qspi_read = qspi_read; 
#endif
    flash->retry.delay = retry_delay_1s;
    flash->retry.times = 30;
    return result;
}

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);

    printf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);

    printf("%s\n", log_buf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}
