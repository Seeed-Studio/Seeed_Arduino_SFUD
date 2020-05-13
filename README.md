# Seeed_Arduino_SFUD  [![Build Status](https://travis-ci.com/Seeed-Studio/SFUD.svg?branch=master)](https://travis-ci.com/Seeed-Studio/SFUD)

---

## Waht's Seeed_Arduino_SFUD?

Seeed_Arduino_SFUD is a combination of Arduino and [SFUD](https://github.com/armink/SFUD).Since there are many types of serial Flash on the market and the specifications and commands of each Flash are different, SFUD is designed to solve these differences, so that our products can support different brands and specifications of Flash, improve the reusability and scalability of software related to Flash functions, and at the same time, avoid the risk of Flash out of stock or out of production.


## Features

- Avoid the risks of Flash stockouts, Flash production disruptions, or product expansions that may result from the project.
- More and more projects are storing firmware in serial Flash, such as firmware in ESP8266, BIOS in motherboards and other common electronics, but the various Flash specifications and commands are not uniform. The use of SFUD avoids the problem of not being able to adapt to different Flash types of hardware platforms based on the same functional software platform and improves the reusability of the software.
- Can be used to make Flash programmers/burners

## API

### sfud_device_init

Initializing the SFUD library


**Note**: The initialized SPI Flash is **write-protected** by default. To enable write-protection, use the sfud_write_status function to modify the SPI Flash status.

```C
sfud_err sfud_init(void)
```

### sfud_device_init

Initialize the specified Flash device

```C
sfud_err sfud_device_init(sfud_flash *flash)
```

|Parameters |Description|
|:--- |:---|
|Flash - to be initialized Flash device|

### sfud_qspi_fast_read_enable

Enables fast read mode (only available when SFUD has QSPI mode on)

```C
sfud_err sfud_qspi_fast_read_enable(sfud_flash *flash, uint8_t data_line_width)
```

| Parameters | Description| 
|:------|:------|
| flash | flash devices | flash|
| Data_line_width | Maximum width of data lines supported by the QSPI bus, e.g., 1, 2, 4 |

### sfud_get_device

Getting Flash Device Objects

```C
sfud_flash *sfud_get_device(size_t index)
```

|Parameters |Description|
|:--- |:---|
|index|The index value of the FLash device in the FLash device table|

### sfud_read

Reading Flash Data

```C
sfud_err sfud_read(const sfud_flash *flash, uint32_t addr, size_t size, uint8_t *data)
```

|Parameters |Description|
|:--- |:---|
|Flash |Flash Device Object|
|Addr | Start Address|
|size | total size of data read from the start address|
|data | readings|

### sfud_erase

Erase Flash Data

**Note**: The erase operation will follow the erase granularity of the Flash chip (see Flash datasheet for details, generally block size). After the initialization is complete, it can be aligned by `sfud_flash->chip.erase_gran` view, please make sure that the starting address and erase data size are aligned according to the erasure granularity of the Flash chip, otherwise other data will be lost after performing the erase operation.

```C
sfud_err sfud_erase(const sfud_flash *flash, uint32_t addr, size_t size)
```

|Parameters |Description|
|:--- |:---|
|Flash |Flash Device Object|
|Addr | Start Address|
|size | total size of data erased from the start address|

### sfud_chip_erase

Erase all Flash data

```C
sfud_err sfud_chip_erase(const sfud_flash *flash)
```

|Parameters |Description|
|:--- |:---|
|Flash |Flash Device Object|

### sfud_write

Write data to Flash

```C
sfud_err sfud_write(const sfud_flash *flash, uint32_t addr, size_t size, const uint8_t *data)
```

|Parameters |Description|
|:--- |:---|
|Flash |Flash Device Object|
|Addr | Start Address|
|size | total size of data written from the start address|
|data to be written | data to be written | data to be written|

### sfud_erase_write

Erase before writing data to Flash

**Note**: The erase operation will follow the erase granularity of the Flash chip (see Flash datasheet for details, generally block size). After the initialization is complete, it can be aligned by `sfud_flash->chip.erase_gran` view, please make sure that the starting address and erase data size are aligned according to the erasure granularity of the Flash chip, otherwise other data will be lost after performing the erase operation.

```C
sfud_err sfud_erase_write(const sfud_flash *flash, uint32_t addr, size_t size, const uint8_t *data)
```

|Parameters |Description|
|:--- |:---|
|Flash |Flash Device Object|
|Addr | Start Address|
|size | total size of data written from the start address|
|data to be written | data to be written | data to be written|


### sfud_read_status

Read Flash Status

```C
sfud_err sfud_read_status(const sfud_flash *flash, uint8_t *status)
```

|Parameters |Description|
|:--- |:---|
|Flash |Flash Device Object|
|status |current status register value|

### sfud_write_status

Write (Modify) Flash Status

```C
sfud_err sfud_write_status(const sfud_flash *flash, bool is_volatile, uint8_t status)
```

|Parameters |Description|
|:--- |:---|
|Flash |Flash Device Object|
is_volatile |is_volatile |is_volatile, true: is_volatile, and will be lost after power failure|
|status |current status register value|
