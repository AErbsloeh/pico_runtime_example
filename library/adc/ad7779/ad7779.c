#include "adc/ad7779/ad7779.h"
#include "hardware/gpio.h"
#include <stdio.h>


// ======================================== INTERNAL READ/WRITE FUNCTIONS ===============================================
/*! \brief Function for writing a register to the AD7779
 *  \param handler  Pointer to typedef struct ad7779_t to handle the settings
 *  \return         true if the write was successful, false otherwise.
 */
static bool ad7779_write_reg(ad7779_t *handler,
                            ad7779_gpio_t *gpio_handler,
                             uint8_t reg,
                             uint8_t data)
{
    uint8_t frame[2];
    // R/W=0 (Write) in Bit7, Adresse in Bits[6:0]
    frame[0] = (0 << 7) | (reg & 0x7F);
    frame[1] = data;

    if (send_data_spi_module(handler->spi, gpio_handler->cs_pin, frame, 2) < 0) {
        return false;
    }
    sleep_ms(1);
    
    return true;
}

/*! \brief Function for reading a register from the AD7779
 *  \param handler  Pointer to typedef struct ad7779_t to handle the settings
 *  \return         true if the read was successful, false otherwise.
 */
static bool ad7779_read_reg(ad7779_t *handler,
                            ad7779_gpio_t *gpio_handler,
                            uint8_t reg,
                            uint8_t *data)
{
    uint8_t tx[2], rx[2];
    // R/W=1 (Read) in Bit7, Adress in Bits[6:0]
    tx[0] = (1 << 7) | (reg & 0x7F);
    tx[1] = 0x00;

    if (receive_data_spi_module(handler->spi, gpio_handler->cs_pin, tx, rx, 2) < 0) {
        return false;
    }
    sleep_ms(1);
    *data = rx[1];  // the second byte contains the register data

    return true;
}


// ======================== AD7779 FUNCTIONS =========================
bool ad7779_soft_reset(ad7779_t *handler, ad7779_gpio_t *gpio_handler){
    uint8_t tx_frame[2];
    int8_t result;

    // --- First Write for Soft-Reset (Bits [1:0] = 11b) ---
    tx_frame[0] = (0 << 7) | (AD7779_REG_GENERAL_USER_CONFIG_1 & 0x7F);
    tx_frame[1] = AD7779_SOFT_RST_FIRST_WRITE;
    result = send_data_spi_module(handler->spi, gpio_handler->cs_pin, tx_frame, 2);
    if (result < 0) {
        return false;
    }

    // Short delay to ensure the chip processes the first command
    sleep_ms(1);

    // --- Second Write for Soft-Reset (Bits [1:0] = 10b) ---
    tx_frame[1] = AD7779_SOFT_RST_SECOND_WRITE;
    result = send_data_spi_module(handler->spi, gpio_handler->cs_pin, tx_frame, 2);
    if (result < 0) {
        return false;
    }

    // Now the AD7779 internally waits a maximum of 225 µs until the first valid DRDY (Data Ready).
    // To be safe, we take a buffer of e.g. 1 ms.
    sleep_ms(1);

    return true;
}


bool ad7779_soft_sync(ad7779_t *handler, ad7779_gpio_t *gpio_handler){
    uint8_t tx_read[2];
    uint8_t rx_read[2];
    ad7779_read_reg(handler, gpio_handler, AD7779_REG_GENERAL_USER_CONFIG_2, &rx_read[1]); // Read register and save the value
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_GENERAL_USER_CONFIG_2, rx_read[1] ^ (1 << 0)); // Changing the last Bit of the Register to 0 this will trigger a SYNC event
    sleep_ms(10);
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_GENERAL_USER_CONFIG_2, rx_read[1]); // Change it back to the original value
    return true;
}


void ad7779_set_sampling_rate_2kSPS(ad7779_t *handler, ad7779_gpio_t *gpio_handler)
{
    uint8_t value_for_N_MSB = 0x04; // For master clock  2MH N_MSB=0x01; L_LSB=0xF4 for 2kSPS
    uint8_t value_for_N_LSB = 0x00;
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_DECIMATION_RATE_REGISTER_N_MSB, value_for_N_MSB);
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_DECIMATION_RATE_REGISTER_N_LSB, value_for_N_LSB);
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_DECIMATION_RATE_REGISTER_IF_MSB, 0x00);
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_DECIMATION_RATE_REGISTER_IF_LSB, 0x00);
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_LOAD_UPDATE_REGISTER, 0x01); // Load new settings
    sleep_ms(1);
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_LOAD_UPDATE_REGISTER, 0x00);
}


void ad7779_set_power_mode(ad7779_t *handler, ad7779_gpio_t *gpio_handler){
    uint8_t reg_value;
    if (handler->high_power_mode) {
        reg_value = 0b01100100; // Set POWERMODE=1
    } else {
        reg_value = 0b00100100; // Set POWERMODE=0
    }
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_GENERAL_USER_CONFIG_1, reg_value);
    sleep_ms(1);
    ad7779_soft_sync(handler, gpio_handler);
}


void ad7779_enable_disable_test_mode(ad7779_t *handler, ad7779_gpio_t *gpio_handler){   //Needs to be tested
    uint8_t reg_value;
    uint8_t channel_config_reg[] ={0x000, 0x001, 0x002, 0x003, 0x004, 0x005, 0x006, 0x007};
    ad7779_read_reg(handler, gpio_handler, AD7779_REG_ADC_MUX_CONFIG, &reg_value);

    reg_value &= ~0b00111100; // Clear TEST_MODE bit
    if (handler ->test_mode) {
        reg_value |= 0b00001000; // Set TEST_MODE bit
    } else {
        reg_value &= ~0b00001000; // Clear TEST_MODE bit
    }
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_ADC_MUX_CONFIG, reg_value);

    for (int i = 0; i < 8; i++) {
        ad7779_read_reg(handler, gpio_handler, channel_config_reg[i], &reg_value);
        if (handler ->test_mode) {
            reg_value |= 0b00010000; // Set TEST_MODE bit in channel config
        } else {
            reg_value &= ~0b00010000; // Clear TEST_MODE bit in channel config
        }
        ad7779_write_reg(handler, gpio_handler, channel_config_reg[i], reg_value);
    }
    ad7779_soft_sync(handler, gpio_handler);
}


bool ad7779_init(ad7779_t *handler, ad7779_gpio_t *gpio_handler){
    // --- GPIO initialization ---
    if(!gpio_handler->init_done){
        ad7779_init_cs_pin(gpio_handler);
        ad7779_trigger_reset(gpio_handler);
        ad7779_start_pin(gpio_handler);
        ad7779_enable_disable_shielding_channels(gpio_handler);
        ad7779_enable_disable_shielding_electrode_ref(gpio_handler);
        gpio_handler->init_done = true;
    }


    // --- SPI initialization ---
    if(!handler->spi->init_done){
        configure_spi_module(handler->spi, false);
    }

    // --- Softreset for AD7779 ---^
    if (!ad7779_soft_reset(handler, gpio_handler)) {
        handler->init_done = false;
        return false;
    }

    sleep_ms(10);

    uint8_t tx_read[2];
    uint8_t rx_read[2];
    int8_t  result;

    tx_read[0] = (1 << 7) | (AD7779_REG_ERROR_STATUS_REGISTER_3 & 0x7F);
    tx_read[1] = 0x00;  // Dummy-Byte

    result = receive_data_spi_module(handler->spi, gpio_handler->cs_pin, tx_read, rx_read, 2);
    if (result < 0) {
        handler->init_done = false;
        return false;
    }
    // Check for CHIP_ERROR and INIT_COMPLETE
    while (1) {
        ad7779_read_reg(handler, gpio_handler, AD7779_REG_GENERAL_ERRORS_REGISTER_2, &rx_read[1]);
        if (rx_read[1] == 0x00) {
            ad7779_read_reg(handler, gpio_handler, AD7779_REG_ERROR_STATUS_REGISTER_3, &rx_read[1]);
            if (rx_read[1] & AD7779_REG_ERROR_STATUS_REGISTER_3_INIT_COMPLETE_BIT) { // Bit 4 = INIT_COMPLETE
                if(DEBUG_MODE) 
                    printf("AD7779 successful init: 0x%02X; Adress: 0x%02X\n", rx_read[1], AD7779_REG_ERROR_STATUS_REGISTER_3);
                handler->init_done = true; 
                break;
            }
        }
        if(DEBUG_MODE) 
            printf("AD7779 ERROR detected! Code: 0x%02X; Adress: 0x%02X\n", rx_read[1], AD7779_REG_GENERAL_ERRORS_REGISTER_2);
        sleep_ms(100);
    }
    // Print all status and error registers
    if (DEBUG_MODE) {
        ad7779_print_all_status_error_register(handler, gpio_handler);
    }   
    return true;
}


bool ad7779_global_config(ad7779_t *handler, ad7779_gpio_t *gpio_handler){
    uint8_t tx_read[2];
    uint8_t rx_read[2];
    int8_t  result;

    // 1. Enable the ability to trigger tbe Sync via the Hardware Pin and set the SDO Driver Strength to extra Strong
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_GENERAL_USER_CONFIG_2, 0b00011001); // Be able to use the Hardware SYNC pin

    // 1. Energy Mode setup to High Resolution
    ad7779_set_power_mode(handler, gpio_handler);
    
    // 2. Bring the Mux register to a known state
    uint8_t mux_config_val = 0b00000000; // Setup for external reference and normal input muxing
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_ADC_MUX_CONFIG,  mux_config_val);
    // 2.1. Set Test Mode if required
    ad7779_enable_disable_test_mode(handler, gpio_handler);

    // 4. Set Sampling Rate to 2kSPS
    ad7779_set_sampling_rate_2kSPS(handler, gpio_handler); // Set sampling rate to 2kSPS
    
    // 5. Set Data Output Format to  CRC Header
    ad7779_write_reg(handler, gpio_handler, AD7779_DATA_OUTPUT_FORMAT_REGISTER, 0x20); // Set Data Output Format to CRC Header

    // Changing Data Read Mode for the SPI Interface
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_GENERAL_USER_CONFIG_3, AD7779_REG_GENERAL_USER_CONFIG_3_VALUE_READ_SPI_ENABLED);
    return true;
}


bool ad7779_configure_pga_for_all_channels(ad7779_t *handler, ad7779_gpio_t *gpio_handler) //needs to be tested
{
    uint8_t channel_config_reg[] ={0x000, 0x001, 0x002, 0x003, 0x004, 0x005, 0x006, 0x007};
    uint8_t reg_value = 0;
    uint8_t current_value =0;
    
    switch (handler ->pga_gain)
    {
    case 1:
        reg_value = 0b00000000;
        break;
    case 2:
        reg_value = 0b01000000;
        break;
    case 4:
        reg_value = 0b10000000;
        break;
    case 8:
        reg_value = 0b11000000;
        break;
    default:
        return false;
    }

    for (int i = 0; i < 8; i++) {
        if(! ad7779_read_reg(handler, gpio_handler, channel_config_reg[i], &current_value)){
            return false;
        }
        current_value &= 0x3F; // Clear PGA bits
        current_value |= reg_value; // Set new PGA bits
        if (!ad7779_write_reg(handler, gpio_handler, channel_config_reg[i], current_value)) {
            return false;
        }
    }
    ad7779_soft_sync(handler, gpio_handler);
    return true;
}


bool ad7779_enable_disable_channels(ad7779_t *handler, ad7779_gpio_t *gpio_handler){ //needs to be tested
    if (!ad7779_write_reg(handler, gpio_handler, AD7779_REG_CH_DISABLE, handler->channel_select)) { //Disable the selected channels
        return false;
    }   
    ad7779_soft_sync(handler, gpio_handler);
    return true;
}

bool ad7779_read_all_channel_data(ad7779_t *handler,
                                ad7779_gpio_t *gpio_handler,
                                uint8_t *pointer_for_rx)
{    
    uint8_t tx[NUM_CHANNELS *4] = {0};
    receive_data_spi_module(handler->spi, gpio_handler->cs_pin, tx, pointer_for_rx, NUM_CHANNELS * 4);
    return true;
}


// IMPORTANT: This function changes the SPI read mode to register read mode!
void ad7779_print_all_status_error_register(ad7779_t *handler, ad7779_gpio_t *gpio_handler){
    uint8_t tx_read[2];
    uint8_t rx_read[2];
    ad7779_write_reg(handler, gpio_handler, AD7779_REG_GENERAL_USER_CONFIG_3, AD7779_REG_GENERAL_USER_CONFIG_3_VALUE_READ_SPI_DISABLED); // Change to read mode for the registers
    printf("- - - - AD7779 Status and Error Registers - - - -\n");

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL0_STATUS_REGISTER, &rx_read[1]);
    printf("Status Register Channel 0 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL0_STATUS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL1_STATUS_REGISTER, &rx_read[1]);
    printf("Status Register Channel 1 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL1_STATUS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL2_STATUS_REGISTER, &rx_read[1]);
    printf("Status Register Channel 2 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL2_STATUS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL3_STATUS_REGISTER, &rx_read[1]);
    printf("Status Register Channel 3 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL3_STATUS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL4_STATUS_REGISTER, &rx_read[1]);
    printf("Status Register Channel 4 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL4_STATUS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL5_STATUS_REGISTER, &rx_read[1]);
    printf("Status Register Channel 5 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL5_STATUS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL6_STATUS_REGISTER, &rx_read[1]);
    printf("Status Register Channel 6 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL6_STATUS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL7_STATUS_REGISTER, &rx_read[1]);
    printf("Status Register Channel 7 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL7_STATUS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL0_CHANNEL1_DSP_ERRORS_REGISTER, &rx_read[1]);
    printf("Saturation Error Register Channel 0 and 1 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL0_CHANNEL1_DSP_ERRORS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL2_CHANNEL3_DSP_ERRORS_REGISTER, &rx_read[1]);
    printf("Saturation Error Register Channel 2 and 3 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL2_CHANNEL3_DSP_ERRORS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL4_CHANNEL5_DSP_ERRORS_REGISTER, &rx_read[1]);
    printf("Saturation Error Register Channel 4 and 5 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL4_CHANNEL5_DSP_ERRORS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_CHANNEL6_CHANNEL7_DSP_ERRORS_REGISTER, &rx_read[1]);
    printf("Saturation Error Register Channel 6 and 7 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_CHANNEL6_CHANNEL7_DSP_ERRORS_REGISTER, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_GENERAL_ERRORS_REGISTER_1, &rx_read[1]);
    printf("General Error Register 1 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_GENERAL_ERRORS_REGISTER_1, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_GENERAL_ERRORS_REGISTER_2, &rx_read[1]);
    printf("General Error Register 2 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_GENERAL_ERRORS_REGISTER_2, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_ERROR_STATUS_REGISTER_1, &rx_read[1]);
    printf("Error Status Register 1 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_ERROR_STATUS_REGISTER_1, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_ERROR_STATUS_REGISTER_2, &rx_read[1]);
    printf("Error Status Register 2 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_ERROR_STATUS_REGISTER_2, rx_read[1]);

    ad7779_read_reg(handler, gpio_handler, AD7779_REG_ERROR_STATUS_REGISTER_3, &rx_read[1]);
    printf("Error Status Register 3 (Address 0x%02X) (normal value 0x00): 0x%02X\n", AD7779_REG_ERROR_STATUS_REGISTER_3, rx_read[1]);

    printf("- - - - AD7779 Status and Error Registers End- - - -\n");
}


// ======================== AD7779 GPIO Functions =========================
bool ad7779_init_cs_pin(ad7779_gpio_t *handler){
    gpio_init(handler->cs_pin);
    gpio_set_dir(handler->cs_pin, GPIO_OUT);
    gpio_put(handler->cs_pin, true);
    return true;
}


bool ad7779_trigger_reset(ad7779_gpio_t *handler){
    if(!handler ->init_done){
        gpio_init(handler->reset_pin);
        gpio_set_dir(handler->reset_pin, GPIO_OUT);
    }
    gpio_put(handler->reset_pin, 1); 
    sleep_ms(10);
    gpio_put(handler->reset_pin, 0); 
    sleep_ms(10);
    gpio_put(handler->reset_pin, 1);
    sleep_ms(10);
    return true;
}


bool ad7779_start_pin(ad7779_gpio_t *handler){
    if (!handler ->init_done){
        gpio_init(handler->start_pin);
        gpio_set_dir(handler->start_pin, GPIO_OUT);
    }
    gpio_put(handler->start_pin, 1); //set start_pin to high, this is the required state, if this pin is not used
    sleep_ms(10);
    gpio_put(handler->start_pin, 0);
    sleep_ms(10);
    gpio_put(handler->start_pin, 1);
    sleep_ms(10);
    return true;
}


bool ad7779_enable_disable_shielding_channels(ad7779_gpio_t *handler){
    if (!handler ->init_done){
        gpio_init(handler->shielding_channels_pin);
        gpio_set_dir(handler->shielding_channels_pin, GPIO_OUT);
    }
    gpio_put(handler->shielding_channels_pin, handler->shielding_active_channels); // Enable shielding channels
    return true;
}


bool ad7779_enable_disable_shielding_electrode_ref(ad7779_gpio_t *handler){
    if (!handler ->init_done){
        gpio_init(handler->shielding_ref_pin);
        gpio_set_dir(handler->shielding_ref_pin, GPIO_OUT);
    }
    gpio_put(handler->shielding_ref_pin, handler->shielding_active_electrode_ref); // Enable shielding electrode reference
    return true;
}