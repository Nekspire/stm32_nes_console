#include <string.h>
#include "nes_controller.h"

const uint8_t NES_CONTROLLER_ID[] = {0x01, 0x00, 0xa4, 0x20, 0x01, 0x01};

NES_Controller_Status nes_controller_init(I2C_HandleTypeDef *i2c) {
    uint8_t init_addr = 0x55;
    uint8_t id_addr = 0xFA;
    uint8_t id[6] = {0};

    HAL_I2C_Mem_Write(i2c, NES_I2C_ADDRESS_WRITE, 0xF0, 1, &init_addr, 1, 100);
    HAL_I2C_Master_Transmit(i2c, NES_I2C_ADDRESS_WRITE,  &id_addr, 1, 100);
    HAL_I2C_Master_Receive(i2c, NES_I2C_ADDRESS_READ, id, 6 , 100);
    HAL_Delay(10);

    if (memcmp(id, NES_CONTROLLER_ID, sizeof(NES_CONTROLLER_ID)) == 0)
        return NES_CONTROLLER_OK;
    else
        return NES_CONTROLLER_ERR;
}

uint16_t nes_controller_read_code(I2C_HandleTypeDef *i2c) {
    uint8_t zero_addr = 0x00;
    uint8_t controller_bytes[6];
    uint16_t state = 0;

    HAL_I2C_Master_Transmit(i2c, NES_I2C_ADDRESS_WRITE, &zero_addr, 1, 100);
    HAL_I2C_Master_Receive(i2c, NES_I2C_ADDRESS_READ, controller_bytes, 6, 100);

    controller_bytes[4] ^= 0xFF;
    controller_bytes[5] ^= 0xFF;

    state = (controller_bytes[4] << 8) | controller_bytes[5];

    return state;
}

bool nes_match_button(uint16_t code, uint16_t NES_Controller_Button) {
    bool state = false;

    if (code == NES_Controller_Button)
        state = true;

    return state;
}