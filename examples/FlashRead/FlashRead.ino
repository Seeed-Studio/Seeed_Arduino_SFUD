#include <sfud.h>

#define SFUD_DEMO_TEST_BUFFER_SIZE                     1024
static uint8_t sfud_demo_test_buf[SFUD_DEMO_TEST_BUFFER_SIZE] = {0};
static void sfud_demo(uint32_t addr, size_t size, uint8_t *data);
	
#define SERIAL Serial
#ifdef ARDUINO_ARCH_SAMD
    #undef SERIAL Serial
    #define SERIAL SerialUSB
#endif
void setup()
{
    SERIAL.begin(115200);
    while(!SERIAL) {};
    while(!(sfud_init() == SFUD_SUCCESS));
    #ifdef SFUD_USING_QSPI
    sfud_qspi_fast_read_enable(sfud_get_device(SFUD_W25Q32_DEVICE_INDEX), 2);
    #endif
    const sfud_flash *flash = sfud_get_device_table() + 0;
    uint32_t addr = 0;
    size_t size = sizeof(sfud_demo_test_buf);
    uint8_t result = sfud_read(flash, addr, size, sfud_demo_test_buf);
    if (result == SFUD_SUCCESS) {
        printf("Read the %s flash data success. Start from 0x%08X, size is %ld. The data is:\r\n", flash->name, addr,
                size);
        printf("Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
        for (size_t i = 0; i < size; i++) {
            if (i % 16 == 0) {
                printf("[%08X] ", addr + i);
            }
            printf("%02X ", sfud_demo_test_buf[i]);
            if (((i + 1) % 16 == 0) || i == size - 1) {
                printf("\r\n");
            }
        }
        printf("\r\n");
    } else {
        printf("Read the %s flash data failed.\r\n", flash->name);
    }
}

void loop()
{     
}