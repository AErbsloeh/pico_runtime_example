#include "sens/sht21/sht21.h"
#include "hardware/gpio.h"


// ======================================== FUNCTIONS ===============================================
bool SHT21_init(sht21_t *handler){
    init_i2c_module(handler->i2c_mod);

    if(check_i2c_bus_for_device_specific(handler->i2c_mod, 0x40)){
        // --- Doing a soft reset
        for(uint8_t cnt=0; cnt < 5; cnt++){
            SHT21_do_soft_reset(handler);
        }

        // --- Do init of sensor       
        // Load write command and current user register content to buffer to be written
        uint8_t register_data[2] = {0};
        register_data[0] = SHT21_USER_REG_W;
        register_data[1] = 0x02;

        // Set user register data to be written
        if (handler->otp_enable) {
            register_data[1] &= ~0x02;
        } else {
            register_data[1] |= 0x02; 
        }
        if (handler->heater_enable) {
            register_data[1] |= 0x04; // Mask bit 2 to 1
        } else {
            register_data[1] &= ~0x04; // Mask bit 2 to 0
        }    

        register_data[1] &= 0xFE; // Mask bit 0 to 0...
        switch (handler->resolution) {
            // Measurement resolution is set in bit 0 (T) and bit 7 (RH) in the user register
            case SHT21_RESOLUTION_08_12:
                register_data[1] |= 0x01; // ... and bit 0 to 1
                break;
            case SHT21_RESOLUTION_10_13:
                register_data[1] |= 0x80; // ... and bit 7 to 1
                break;
            case SHT21_RESOLUTION_11_11:
                register_data[1] |= 0x81; // Mask bits 0 and 7 to 1
                break;
            default:
                // Default measurement resolution is 12 bit RH and 14 bit T
                register_data[1] &= 0x7E;
                break;
        }

        // Send user resgister data to sensor
        if (i2c_write_blocking(handler->i2c_mod->i2c_mod, 0x40, register_data, 2, false) == PICO_ERROR_GENERIC) {
            handler->init_done = false;        
        }else{
            handler->init_done = true;
        }
    } else {
        handler->init_done = false; 
    }
    return handler->init_done;
}


void SHT21_do_soft_reset(sht21_t *handler){
    uint8_t buffer[1] = {SHT21_RESET};
    i2c_write_blocking(handler->i2c_mod->i2c_mod, 0x40, buffer, 1, true);
    sleep_ms(10);
}


uint8_t SHT21_read_user_register(sht21_t *handler){
    uint8_t buffer[1] = {SHT21_USER_REG_R};
    i2c_write_blocking(handler->i2c_mod->i2c_mod, 0x40, buffer, 1, false);
    i2c_read_blocking(handler->i2c_mod->i2c_mod, 0x40, buffer, 1, false);
    return buffer[0];
}


uint16_t SHT21_read_data(sht21_t *handler, uint8_t command){
    uint8_t  buffer[3] = {command};

    i2c_write_blocking(handler->i2c_mod->i2c_mod, 0x40, buffer, 1, false);
    sleep_us(50);
    while (i2c_read_blocking(handler->i2c_mod->i2c_mod, 0x40, buffer, 3, false) == PICO_ERROR_GENERIC){
        sleep_ms(2);
    }    
    // Taking data and ignore last two status bits and checksum
    uint16_t raw_data = ((buffer[0] << 8) + buffer[1]) & 0xFFFC; 
    return raw_data;
}


float SHT21_get_humidity_float(sht21_t *handler){
    uint16_t raw_data     = SHT21_read_data(handler, SHT21_RH_NO_HOLD);
    float    rel_humidity = -6 + (125 * raw_data) / 65536;
    return rel_humidity;
}


uint16_t SHT21_get_humidity_uint(sht21_t *handler){
    return SHT21_read_data(handler, SHT21_RH_NO_HOLD);
}


float SHT21_get_temperature_float(sht21_t *handler){
    uint16_t raw_data    = SHT21_read_data(handler, SHT21_T_NO_HOLD);
    float    temperature = 226.3 + (175.72 * raw_data) / 65536;
    return temperature;
}


uint16_t SHT21_get_temperature_uint(sht21_t *handler){
    return SHT21_read_data(handler, SHT21_T_NO_HOLD);     
}
