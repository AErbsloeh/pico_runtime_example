#include "adc/ad4858/ad4858.h"
#include <stdio.h>
#include "hardware/gpio.h"


// ====================== REGISTER ADDRESSES ====================== 
#define SPI_CONFIG_A    0x00
#define SPI_CONFIG_B    0x01
#define DEVICE_CONFIG   0x02
#define PRODUCT_ID_L    0x04
#define PRODUCT_ID_H    0x05
#define SCRATCH_VALUE   0x0A
#define DEVICE_STATUS   0x21
#define PACKET          0x26
#define DEVICE_CTRL     0x25
static const uint16_t SOFTSPAN_REG_ADDR[8] = {
    0x2A,  // CH0_SOFTSPAN
    0x3C,  // CH1_SOFTSPAN
    0x4E,  // CH2_SOFTSPAN
    0x60,  // CH3_SOFTSPAN
    0x72,  // CH4_SOFTSPAN
    0x84,  // CH5_SOFTSPAN
    0x96,  // CH6_SOFTSPAN
    0xA8   // CH7_SOFTSPAN
};

// ======================================== INTERNAL READ/WRITE COMMANDS ===============================================

uint8_t handler_pico_spi_transmission(ad4858_t *settings, bool rnw, uint16_t adr, uint8_t data){
    uint8_t buffer_tx[3] = {0x00};
    uint8_t buffer_rx[3] = {0x00};
    buffer_tx[0] = ((rnw) ? 0x80 : 0x00) | ((adr & 0x7F00) >> 0x08);
    buffer_tx[1] = ((adr & 0x00FF) >> 0x00);
    buffer_tx[2] = ((data & 0x00FF) >> 0x00);

    receive_data_spi_module(settings->spi_mod, settings->gpio_csn, buffer_tx, buffer_rx, 3);
    return buffer_rx[2];
};

//read-modify-write
//mask says what to modify
void ad4858_handler_pico_spi_rmw(ad4858_t *settings, uint16_t adr, uint8_t data, uint8_t mask){
    uint8_t reg_data = handler_pico_spi_transmission(settings, true, adr, 0x00);    //Read
    uint8_t mod = (mask & data) | (~mask & reg_data);                               //Modify
    handler_pico_spi_transmission(settings, false, adr, mod);                       //Write
}

void ad4858_handler_spi_send_sw_reset(ad4858_t *settings){
    ad4858_handler_pico_spi_rmw(settings, SPI_CONFIG_A, 0x81, 0x81); // 10000001 = 0x81
}

void ad4858_handler_spi_set_4_wire(ad4858_t *settings, bool enable){
    if(enable) ad4858_handler_pico_spi_rmw(settings, SPI_CONFIG_A, 0x10, 0x10); // 00010000 = 0x10
    else ad4858_handler_pico_spi_rmw(settings, SPI_CONFIG_A, 0x00, 0x10);
}

void ad4858_handler_spi_set_streaming_instruction_mode(ad4858_t *settings, bool enable){
    if(enable) ad4858_handler_pico_spi_rmw(settings, SPI_CONFIG_B, 0x00, 0x80); // 00010000 = 0x10
    else ad4858_handler_pico_spi_rmw(settings, SPI_CONFIG_B, 0x80, 0x80);
}

uint16_t ad4858_handler_spi_get_prod_id(ad4858_t *settings){
    uint8_t prod_h = handler_pico_spi_transmission(settings, true, PRODUCT_ID_H, 0x00);
    uint8_t prod_l = handler_pico_spi_transmission(settings, true, PRODUCT_ID_L, 0x00);
    return (prod_h << 8) | prod_l;
}

//write some data and see if the same data comes back
bool ad4858_handler_spi_test_com(ad4858_t *settings){
    uint8_t tst_val = 0x55;
    handler_pico_spi_transmission(settings, false, SCRATCH_VALUE, tst_val);
    uint8_t reg_data = handler_pico_spi_transmission(settings, true, SCRATCH_VALUE, 0x00);
    return reg_data == tst_val;
}

uint8_t ad4858_handler_spi_get_device_status(ad4858_t *settings){
    return handler_pico_spi_transmission(settings, true, DEVICE_STATUS, 0x00);
}

void ad4858_handler_spi_set_packet_size(ad4858_t *settings, uint16_t packet_size){
    ad4858_handler_pico_spi_rmw(settings, PACKET, packet_size, 0x03);
}

void ad4858_handler_spi_set_test_pat_toggle(ad4858_t *settings, bool enable){
    ad4858_handler_pico_spi_rmw(settings, PACKET, 0x00 | (enable << 2), 0x04);
}

void ad4858_handler_spi_set_ref_sel(ad4858_t *settings, bool state){
    ad4858_handler_pico_spi_rmw(settings, DEVICE_CTRL, 0x00 | (state << 1), 0x02);
}
/* 
see: https://www.analog.com/media/en/technical-documentation/data-sheets/ad4858.pdf
0000: 0 V to 2.5V range.
0001: ±2.5 V range
0010: 0 V to 5V range.
0011: ±5 V range.
0100: 0 V to 6.25 V range.
0101: ±6.25 V range.
0110: 0 V to 10 V range.
0111: ±10 V range.
1000: 0 V to 12.5 V range.
1001: ±12.5 V range.
1010: 0 V to 20 V range.
1011: ±20 V range.
1100: 0 V to 25 V range.
1101: ±25V range.
1110: 0 V to 40 V range.
1111: ±40 V range. */

void ad4858_handler_spi_set_softspan(ad4858_t *settings, uint8_t channel, uint8_t lvl){
    ad4858_handler_pico_spi_rmw(settings, SOFTSPAN_REG_ADDR[channel], lvl, 0x0F);
}

// ====================== FUNCTIONS ====================== 

// max bis 500kHz
bool handler_pico_cmos_reciever_polling(ad4858_t *settings, uint32_t* data){
    if (!gpio_get(settings->gpio_busy)){
        return false;
    } else {
        for (uint8_t cnt_bit = 0; cnt_bit < settings->cmos_package_size; cnt_bit++){
            //then send 1 clock pulse
            gpio_put(settings->gpio_cmos_sck, true);
            sleep_us(settings->cmos_clock_delay_us);

            //read every channel
            for (uint8_t chnnl = 0; chnnl < 8; chnnl++){
                if(gpio_get(settings->gpio_SDO[chnnl])){
                    data[chnnl] |= 1 << (settings->cmos_package_size-cnt_bit-1);
                };
            }

            gpio_put(settings->gpio_cmos_sck, false);
            sleep_us(settings->cmos_clock_delay_us);
        }
        return true;
    }
};

//#define POLY  0x755B //equal to bitmask of crc polynomial given in the datasheet
// More information: https://www.analog.com/media/en/technical-documentation/data-sheets/ad4858.pdf

// Code by: https://barrgroup.com/blog/crc-series-part-3-crc-implementation-code-cc
// LUT by: https://crccalc.com/?crc=&method=CRC-16/OPENSAFETY-B&datatype=hex&outtype=hex
bool ad4858_handler_pico_cmos_crc_check(ad4858_t *settings, uint8_t const message[], int nBytes, uint16_t recieved_crc){
    uint16_t crcTable[] = {
        0x0000, 0x755B, 0xEAB6, 0x9FED,   0xA037, 0xD56C, 0x4A81, 0x3FDA,
        0x3535, 0x406E, 0xDF83, 0xAAD8,   0x9502, 0xE059, 0x7FB4, 0x0AEF,
        0x6A6A, 0x1F31, 0x80DC, 0xF587,   0xCA5D, 0xBF06, 0x20EB, 0x55B0,
        0x5F5F, 0x2A04, 0xB5E9, 0xC0B2,   0xFF68, 0x8A33, 0x15DE, 0x6085,
        0xD4D4, 0xA18F, 0x3E62, 0x4B39,   0x74E3, 0x01B8, 0x9E55, 0xEB0E,
        0xE1E1, 0x94BA, 0x0B57, 0x7E0C,   0x41D6, 0x348D, 0xAB60, 0xDE3B,
        0xBEBE, 0xCBE5, 0x5408, 0x2153,   0x1E89, 0x6BD2, 0xF43F, 0x8164,
        0x8B8B, 0xFED0, 0x613D, 0x1466,   0x2BBC, 0x5EE7, 0xC10A, 0xB451,
        0xDCF3, 0xA9A8, 0x3645, 0x431E,   0x7CC4, 0x099F, 0x9672, 0xE329,
        0xE9C6, 0x9C9D, 0x0370, 0x762B,   0x49F1, 0x3CAA, 0xA347, 0xD61C,
        0xB699, 0xC3C2, 0x5C2F, 0x2974,   0x16AE, 0x63F5, 0xFC18, 0x8943,
        0x83AC, 0xF6F7, 0x691A, 0x1C41,   0x239B, 0x56C0, 0xC92D, 0xBC76,
        0x0827, 0x7D7C, 0xE291, 0x97CA,   0xA810, 0xDD4B, 0x42A6, 0x37FD,
        0x3D12, 0x4849, 0xD7A4, 0xA2FF,   0x9D25, 0xE87E, 0x7793, 0x02C8,
        0x624D, 0x1716, 0x88FB, 0xFDA0,   0xC27A, 0xB721, 0x28CC, 0x5D97,
        0x5778, 0x2223, 0xBDCE, 0xC895,   0xF74F, 0x8214, 0x1DF9, 0x68A2,
        0xCCBD, 0xB9E6, 0x260B, 0x5350,   0x6C8A, 0x19D1, 0x863C, 0xF367,
        0xF988, 0x8CD3, 0x133E, 0x6665,   0x59BF, 0x2CE4, 0xB309, 0xC652,
        0xA6D7, 0xD38C, 0x4C61, 0x393A,   0x06E0, 0x73BB, 0xEC56, 0x990D,
        0x93E2, 0xE6B9, 0x7954, 0x0C0F,   0x33D5, 0x468E, 0xD963, 0xAC38,
        0x1869, 0x6D32, 0xF2DF, 0x8784,   0xB85E, 0xCD05, 0x52E8, 0x27B3,
        0x2D5C, 0x5807, 0xC7EA, 0xB2B1,   0x8D6B, 0xF830, 0x67DD, 0x1286,
        0x7203, 0x0758, 0x98B5, 0xEDEE,   0xD234, 0xA76F, 0x3882, 0x4DD9,
        0x4736, 0x326D, 0xAD80, 0xD8DB,   0xE701, 0x925A, 0x0DB7, 0x78EC,
        0x104E, 0x6515, 0xFAF8, 0x8FA3,   0xB079, 0xC522, 0x5ACF, 0x2F94,
        0x257B, 0x5020, 0xCFCD, 0xBA96,   0x854C, 0xF017, 0x6FFA, 0x1AA1,
        0x7A24, 0x0F7F, 0x9092, 0xE5C9,   0xDA13, 0xAF48, 0x30A5, 0x45FE,
        0x4F11, 0x3A4A, 0xA5A7, 0xD0FC,   0xEF26, 0x9A7D, 0x0590, 0x70CB,
        0xC49A, 0xB1C1, 0x2E2C, 0x5B77,   0x64AD, 0x11F6, 0x8E1B, 0xFB40,
        0xF1AF, 0x84F4, 0x1B19, 0x6E42,   0x5198, 0x24C3, 0xBB2E, 0xCE75,
        0xAEF0, 0xDBAB, 0x4446, 0x311D,   0x0EC7, 0x7B9C, 0xE471, 0x912A,
        0x9BC5, 0xEE9E, 0x7173, 0x0428,   0x3BF2, 0x4EA9, 0xD144, 0xA41F
    };
    typedef uint16_t crc;

    #define WIDTH  (8 * sizeof(crc))
    uint8_t data;
    crc remainder = 0;


    /*
     * Divide the message by the polynomial, a byte at a time.
     */
    for (int byte = 0; byte < nBytes; ++byte)
    {
        data = message[byte] ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);
    }

    /*
     * The final remainder is the CRC.
     */
    return (remainder == recieved_crc);
};

void ad4858_start_conv(ad4858_t* settings){
    //uint8_t reg_data = handler_pico_spi_transmission(settings, true, 0x20, 0x00); 
    //printf("Data: %x\n", reg_data);
    gpio_put(settings->gpio_convert, true);
    //sleep_us(1);
    gpio_put(settings->gpio_convert, false);
}


bool ad4858_init(ad4858_t* settings) {
    /*if(settings->spi_mod->init_done){
        configure_spi_module(settings->spi_mod, false);
    }*/
    configure_spi_module(settings->spi_mod, false);

    // --- Init of GPIOs ---
    gpio_init(settings->gpio_csn);
    gpio_set_dir(settings->gpio_csn, GPIO_OUT);
    gpio_put(settings->gpio_csn, true); 

    gpio_init(settings->gpio_pwr_dwn);
    gpio_set_dir(settings->gpio_pwr_dwn, GPIO_OUT);
    gpio_put(settings->gpio_pwr_dwn, false);  
    
    gpio_init(settings->gpio_convert);
    gpio_set_dir(settings->gpio_convert, GPIO_OUT);
    gpio_put(settings->gpio_convert, false); 

    gpio_init(settings->gpio_busy);
    gpio_set_dir(settings->gpio_busy, GPIO_IN);

    for(int i = 0; i < 8; i++){
        gpio_init(settings->gpio_SDO[i]);
        gpio_set_dir(settings->gpio_SDO[i], GPIO_IN);
    }

    //Global reset using PD pin
    for(uint8_t rpt=0; rpt < 4; rpt++){
        sleep_us(100);
        gpio_put(settings->gpio_pwr_dwn, !gpio_get(settings->gpio_pwr_dwn)); 
    }
    sleep_us(100);
    gpio_put(settings->gpio_pwr_dwn, true); 

    //configure PWM module
    construct_pwm_set_phase_correct(settings->cmos_pwm_handler, settings->cmos_phase_correct);
    construct_pwm_set_freq_duty(settings->cmos_pwm_handler, settings->cmos_frequency, settings->cmos_duty_cycle);
    construct_pwm_set_wrap_irq(settings->cmos_pwm_handler, true);
    //construct_pwm_enable_disable(settings->cmos_pwm_handler, true);
    //construct_pwm_enable_disable(settings->cmos_pwm_handler, false);

    //init registers
    ad4858_handler_spi_set_4_wire(settings, true);
    ad4858_handler_spi_set_ref_sel(settings, true);
    switch (settings->cmos_package_size)
    {
    case 20:
        ad4858_handler_spi_set_packet_size(settings, 0);
        break;
        
    case 24:
        ad4858_handler_spi_set_packet_size(settings, 1);
        break;
    
    case 32:
        ad4858_handler_spi_set_packet_size(settings, 2);
        break;
        
    default:
        ad4858_handler_spi_set_packet_size(settings, 0);
        break;
    }
    

    //set softspan to +-20V on all channels
    for(int i = 0; i < 8; i++){
        ad4858_handler_spi_set_softspan(settings, i, 0x0F);
    }

    // --- Init of ADC ---
    settings->init_done = true;
    return settings->init_done;
};