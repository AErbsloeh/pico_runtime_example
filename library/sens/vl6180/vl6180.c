#include "sens/vl6180/vl6180.h"
#include <stdio.h>


// ======================================== INTERNAL READ/WRITE COMMANDS ===============================================
bool VL6180_write_data(vl6180_t *handler, uint16_t command, uint8_t data){
    uint8_t buffer_tx[3] = {0x00};
    buffer_tx[0] = (command & 0xFF00) >> 8;
    buffer_tx[1] = (command & 0x00FF) >> 0;
    buffer_tx[2] = data;

    bool error = construct_i2c_write_data(handler->i2c_mod, VL6180X_I2C_ADR, buffer_tx, 3);
    if(!error){
        printf("Error: VL6180 does not respond\n");
    };
    return error;
}


bool VL6180_write_bytes(vl6180_t *handler, uint16_t command, uint16_t data){
    uint8_t buffer_tx[4] = {0x00};
    buffer_tx[0] = (command & 0xFF00) >> 8;
    buffer_tx[1] = (command & 0x00FF) >> 0;
    buffer_tx[2] = (data & 0xFF00) >> 8;
    buffer_tx[3] = (data & 0x00FF) >> 0;

    bool error = construct_i2c_write_data(handler->i2c_mod, VL6180X_I2C_ADR, buffer_tx, 4);
    if(!error){
        printf("Error: VL6180 does not respond\n");
    };
    return error;
}


bool VL6180_read_data(vl6180_t *handler, uint16_t command, uint8_t buffer_rx[], size_t length){
    uint8_t buffer_tx[2] = {0x00};
    buffer_tx[0] = (command & 0xFF00) >> 8;
    buffer_tx[1] = (command & 0x00FF) >> 0;

    bool error = construct_i2c_read_data(handler->i2c_mod, VL6180X_I2C_ADR, buffer_tx, 2, buffer_rx, length);
    if(!error){
        printf("Error: VL6180 does not respond\n");
    };
    return error;
}


// ======================================== FUNCTIONS ===============================================
bool VL6180_read_id(vl6180_t *handler, bool print_id){
    uint8_t buffer_rx[1] = {0x00};
    VL6180_read_data(handler, 0x0000, buffer_rx, 1); 
    return buffer_rx[0] == 0xB4;
}


bool VL6180_soft_reset(vl6180_t *handler){
    VL6180_write_data(handler, 0x0016, 0x01);
    return true;
}


bool VL6180_disable_reset(vl6180_t *handler){
    VL6180_write_data(handler, 0x0016, 0x00);
    return true;
}


void VL6180_set_scaling(vl6180_t *handler, uint16_t new_scaling_value){
    int scalerValues[] = {0, 253, 127, 84};
    if (new_scaling_value < 1 || new_scaling_value > 3) { return; }

    uint8_t buffer_tx[1] = {0x00};
    VL6180_read_data(handler, 0x0024, buffer_tx, 1);
    int8_t ptp_offset = buffer_tx[0];

    VL6180_write_bytes(handler, 0x0096, scalerValues[new_scaling_value]);
    VL6180_write_data(handler, 0x0024, ptp_offset / new_scaling_value);
    VL6180_write_data(handler, 0x0021, 20 / new_scaling_value);
    
    VL6180_read_data(handler, 0x002d, buffer_tx, 1);
    int8_t rce = buffer_tx[0];
    VL6180_write_data(handler, 0x002d, (rce & 0xFE) | (new_scaling_value == 1));
}



bool VL6180_init(vl6180_t *handler){
    init_i2c_module(handler->i2c_mod);

    if(!VL6180_read_id(handler, false)){
        printf("Wrong sensor on bus!");
        return false;
    } else{
        // Do soft reset
        VL6180_soft_reset(handler);
        sleep_ms(1);

        uint8_t buffer_tx[1] = {0x00};
        VL6180_read_data(handler, 0x0016, buffer_tx, 1);

        if(buffer_tx[0] == 0x01){
            // Private Settings from Page 24 of AppNote
            VL6180_write_data(handler, 0x0207, 0x01);
            VL6180_write_data(handler, 0x0208, 0x01);
            VL6180_write_data(handler, 0x0096, 0x00);
            VL6180_write_data(handler, 0x0097, 0xfd);
            VL6180_write_data(handler, 0x00e3, 0x00);
            VL6180_write_data(handler, 0x00e4, 0x04);
            VL6180_write_data(handler, 0x00e5, 0x02);
            VL6180_write_data(handler, 0x00e6, 0x01);
            VL6180_write_data(handler, 0x00e7, 0x03);
            VL6180_write_data(handler, 0x00f5, 0x02);
            VL6180_write_data(handler, 0x00d9, 0x05);
            VL6180_write_data(handler, 0x00db, 0xce);
            VL6180_write_data(handler, 0x00dc, 0x03);
            VL6180_write_data(handler, 0x00dd, 0xf8);
            VL6180_write_data(handler, 0x009f, 0x00);
            VL6180_write_data(handler, 0x00a3, 0x3c);
            VL6180_write_data(handler, 0x00b7, 0x00);
            VL6180_write_data(handler, 0x00bb, 0x3c);
            VL6180_write_data(handler, 0x00b2, 0x09);
            VL6180_write_data(handler, 0x00ca, 0x09);
            VL6180_write_data(handler, 0x0198, 0x01);
            VL6180_write_data(handler, 0x01b0, 0x17);
            VL6180_write_data(handler, 0x01ad, 0x00);
            VL6180_write_data(handler, 0x00ff, 0x05);
            VL6180_write_data(handler, 0x0100, 0x05);
            VL6180_write_data(handler, 0x0199, 0x05);
            VL6180_write_data(handler, 0x01a6, 0x1b);
            VL6180_write_data(handler, 0x01ac, 0x3e);
            VL6180_write_data(handler, 0x01a7, 0x1f);
            VL6180_write_data(handler, 0x0030, 0x00);

            // Recommended : Public registers - See data sheet for more detail
            VL6180_write_data(handler, 0x0011, 0x10); // Enables polling for 'New Sample ready' when measurement completes
            VL6180_write_data(handler, 0x010a, 0x30); // Set the averaging sample period (compromise between lower noise and increased execution time)
            VL6180_write_data(handler, 0x003f, 0x46); // Sets the light and dark gain (upper nibble). Dark gain should not be changed.
            VL6180_write_data(handler, 0x0031, 0xFF); // sets the # of range measurements after which auto calibration of system is performed
            VL6180_write_data(handler, 0x0041, 0x63); // Set ALS integration time to 100ms
            VL6180_write_data(handler, 0x002e, 0x01); // perform a single temperature calibration of the ranging sensor
            VL6180_write_data(handler, 0x0019, 0xFF); // Apply highest values for threshold high register
            VL6180_write_data(handler, 0x001A, 0x00); // Apply highest values for threshold low register

            // Disable GPIO0 and GPIO1
            VL6180_write_data(handler, 0x0010, 0x60);
            VL6180_write_data(handler, 0x0011, 0x20);

            // Mode selection
            VL6180_write_data(handler, 0x0018, 0x20);

            // Setting max convergence time
            VL6180_write_data(handler, 0x001C, handler->max_convergence_ms & 0x3F);

            // Optional: Public registers - See data sheet for more detail
            VL6180_write_data(handler, 0x001B, 0x09); // Set default ranging inter-measurement period to 100ms
            VL6180_write_data(handler, 0x003e, 0x31); // Set default ALS inter-measurement period to 500ms
            VL6180_write_data(handler, 0x0014, 0x24); // Configures interrupt on 'New Sample Ready threshold event'

            VL6180_disable_reset(handler);
        }

        // Scaling value 
        VL6180_set_scaling(handler, 1);

        sleep_ms(1);

        handler->init_done = true;        
        return true;
    }    
}

uint8_t VL6180_get_range_error(vl6180_t *handler){
    uint8_t buffer_rx[1] = {0x00};
    VL6180_read_data(handler, 0x004D, buffer_rx, 1); 
    uint8_t error_code = ((buffer_rx[0] >> 4) & 0x0F);
    return error_code;
}


uint8_t VL6180_get_range_error_isr(vl6180_t *handler){
    uint8_t buffer_rx[1] = {0x00};
    VL6180_read_data(handler, 0x004F, buffer_rx, 1); 
    uint8_t error_code = buffer_rx[0] & 0x07;
    return error_code;
}


bool VL6180_start_single_measurement(vl6180_t *handler){
    return VL6180_write_data(handler, 0x0018, 0x01);
}

bool VL6180_start_cont_measurement(vl6180_t *handler){
    return VL6180_write_data(handler, 0x0018, 0x03);
}

bool VL6180_stop_cont_measurement(vl6180_t *handler){
    return VL6180_write_data(handler, 0x0018, 0x00);
}

uint8_t VL6180_get_range_value(vl6180_t *handler){
    uint8_t buffer_rx[1] = {0x00};
    VL6180_read_data(handler, 0x0062, buffer_rx, 1);
    //VL6180_write_data(handler, 0x0015, 0x07);
    return buffer_rx[0]; 
}
