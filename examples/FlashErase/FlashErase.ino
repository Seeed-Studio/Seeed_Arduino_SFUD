#include <sfud.h>

#define SFUD_DEMO_TEST_BUFFER_SIZE                     1024
static uint8_t sfud_demo_test_buf[SFUD_DEMO_TEST_BUFFER_SIZE];
static void sfud_demo(uint32_t addr, size_t size, uint8_t *data);

void setup()
{
    while(!(sfud_init() == SFUD_SUCCESS));
}

void loop()
{   
    /* erase test */
    const sfud_flash *flash = sfud_get_device_table() + 0;
    uint32_t addr = 0;
    size_t size = sizeof(sfud_demo_test_buf);
    uint8_t result = sfud_erase(flash, addr, size);
    if (result == SFUD_SUCCESS) {
        printf("Erase the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr,
                size);
    } else {
        printf("Erase the %s flash data failed.\r\n", flash->name);
    }
    delay(5000);    
}