#include "sens/adxl345/adxl345_i2c.h"
#include "hardware/gpio.h"


// ========================== INTERNAL FUNCTIONS ==========================
bool ADXL345_i2c_write_byte(adxl345_i2c_t *handler, uint8_t command, uint8_t data){
    uint8_t buffer_tx[2] = {command};
    buffer_tx[1] = data;

    i2c_write_blocking(handler->i2c_mod->i2c_mod, ADXL345_I2C_ADR, buffer_tx, sizeof(buffer_tx), false);
    sleep_us(50);
    return true;
};


bool ADXL345_i2c_write_block(adxl345_i2c_t *handler, uint8_t data[], uint8_t size){
    i2c_write_blocking(handler->i2c_mod->i2c_mod, ADXL345_I2C_ADR, data, size, false);
    return true;
};


uint64_t ADXL345_i2c_read_data(adxl345_i2c_t *handler, uint8_t command, uint8_t buffer_rx[], uint8_t size) { 
    uint8_t buffer_tx[1] = {command}; 

    // I2C-Read mit Wiederholung bei Fehlern 
    int ret = i2c_write_blocking(handler->i2c_mod->i2c_mod, ADXL345_I2C_ADR, buffer_tx, 1, true); 

    if (ret != 1) { 
        return 0; 
    } 

    ret = i2c_read_blocking(handler->i2c_mod->i2c_mod, ADXL345_I2C_ADR, buffer_rx, size, false); 

    if (ret != size) { 
        return 0; 
    } 

    // Kombiniere Bytes zu 16-Bit-Werten (Little-Endian) 
    uint64_t raw_data = 0;
    for(uint8_t idx = 0; idx < size; idx++) {
        raw_data |= (uint64_t)buffer_rx[idx] << (8 * idx);  // Little-Endian
    }
    return raw_data;
}; 


// ========================== CALLABLE FUNCTIONS ==========================
bool ADXL345_init(adxl345_i2c_t *handler) { 
    if(!handler->i2c_mod->init_done){
        configure_i2c_module(handler->i2c_mod); 
    }

    if(check_i2c_bus_for_device_specific(handler->i2c_mod, ADXL345_I2C_ADR)) { 
        // set data format to full resolution, right-justified, range = +/-2g
        ADXL345_i2c_write_byte(handler, ADXL345_DATA_FORMAT, 0x00); 
        // start monitoring sensor data
        ADXL345_i2c_write_byte(handler, ADXL345_POWER_CTL, 0x08); 
        // set data format to little endian + Tiefpassfilter 
        ADXL345_i2c_write_byte(handler, ADXL345_DATA_FORMAT, 0x0B);
        // BW_RATE auf 100 Hz + Tiefpassfilter (Bit D4=1) + disable Low Power Mode 
        ADXL345_i2c_write_byte(handler, ADXL345_BW_RATE, 0x0D); // 0x0D = 100 Hz + Filter

        handler->init_done = ADXL345_i2c_read_id(handler) == ADXL345_DEVID;
    } else {
        handler->init_done = false;
    }
    return handler->init_done; 
};


bool ADXL345_i2c_read_id(adxl345_i2c_t *handler){
    uint8_t buffer[1] = {0};
    ADXL345_i2c_read_data(handler, 0x00, buffer, 1);
    return (buffer[0] == 0xE5);
};


int ADXL345_get_acceleration(adxl345_i2c_t *handler, float *x, float *y, float *z) { 

    uint8_t buffer[6];
    uint64_t result = ADXL345_i2c_read_data(handler, ADXL345_DATAX0, buffer, sizeof(buffer));

    if (result == 0) { // Fehler beim Lesen
        return -1;
    }
  
    // Rohdaten zusammensetzen (Little-Endian) 
    int16_t raw_x = (buffer[1] << 8) | buffer[0]; 
    int16_t raw_y = (buffer[3] << 8) | buffer[2]; 
    int16_t raw_z = (buffer[5] << 8) | buffer[4]; 

    // Umrechnung in g-Einheiten 
    *x = raw_x / ADXL345_SCALE_FACTOR; 
    *y = raw_y / ADXL345_SCALE_FACTOR; 
    *z = raw_z / ADXL345_SCALE_FACTOR; 
    return 0;
};


void ADXL345_reset_offset(adxl345_i2c_t *handler){
    // Reset der Offset-Register
    ADXL345_i2c_write_byte(handler, ADXL345_OFSX, 0x00);
    ADXL345_i2c_write_byte(handler, ADXL345_OFSY, 0x00);
    ADXL345_i2c_write_byte(handler, ADXL345_OFSZ, 0x00);
};


bool ADXL345_do_calibration(adxl345_i2c_t *handler){
    /*********************************************
     * Sensor has to lay on a flat surface
     * and should not be moved during calibration
     **********************************************/
    const int num_samples = 200; 
    float sum_x = 0.0f, sum_y = 0.0f, sum_z = 0.0f;

    for (int i = 0; i < num_samples; i++) {
        float x, y, z;
        ADXL345_get_acceleration(handler, &x, &y, &z);
        sum_x += x;
        sum_y += y;
        sum_z += z;
        sleep_ms(10); 
    }

    handler->offset_x  = -sum_x / num_samples; 			// Sollte nahe 0 sein
    handler->offset_y = -sum_y / num_samples; 			// Sollte nahe 0 sein
    handler->offset_z = 1.0f -(sum_z / num_samples); 	// Ziel: 1g → Offset = Messung - 1g

    // Umrechnung der Offsets in das ADXL345-Format (15,6 mg pro LSB)
    int8_t ofs_x = (int8_t)(handler->offset_x * 64); // 1g = 64 LSB
    int8_t ofs_y = (int8_t)(handler->offset_y * 64);
    int8_t ofs_z = (int8_t)(handler->offset_z * 64);

    // Schreiben der Offsets in die entsprechenden Register
    ADXL345_i2c_write_byte(handler, ADXL345_OFSX, ofs_x);
    ADXL345_i2c_write_byte(handler, ADXL345_OFSY, ofs_y);
    ADXL345_i2c_write_byte(handler, ADXL345_OFSZ, ofs_z);
	return true;
};

