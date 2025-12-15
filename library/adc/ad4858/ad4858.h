#ifndef AD4858_H_
#define AD4858_H_

#include "hal/spi/spi.h"
#include "hal/irq/irq.h"
#include "hal/irq/irq.h"


// More information: https://www.analog.com/media/en/technical-documentation/data-sheets/ad4858.pdf
// ====================== DEFINITIONS ====================== 

/*! \brief Structure with Settings for handling the Analog-Digital Converter AD4858 from Analog Devices
 * \param spi_mod         SPI handler for communication with the AD4858 (worked with SPI MODE 1)
 * \param gpio_csn        GPIO number of used CSN (default: PICO_DEFAULT_SPI_CSN_PIN)
 * \param gpio_pwr_dwn    GPIO number of used Power Down pin
 * \param gpio_convert    GPIO number of used Convert pin
 * \param gpio_busy       GPIO number of used Busy pin
 * \param init_done       Boolean if initialization is done
*/
typedef struct {
    spi_device_handler_t* spi_mod;  
    uint8_t gpio_csn;    
    uint8_t gpio_pwr_dwn;
    uint8_t gpio_convert;     
    uint8_t gpio_busy;
    //CMOS Pins
    uint8_t gpio_SDO[8];
    uint8_t gpio_cmos_sck;
    //package format
    uint8_t cmos_package_size;
    uint8_t cmos_clock_delay_us;
    bool init_done;

    //CMOS_SCK PWM_handler
    pwm_device_handler_t* cmos_pwm_handler;
    bool cmos_phase_correct;
    uint32_t cmos_frequency;
    int cmos_duty_cycle; //duty cycle in %
    
} ad4858_t;


// ====================== FUNCTIONS ====================== 
/*! \brief Function for initializing the AD4858 device
 * \param settings       Pointer to the AD4858 device structure
 * \return               Bool if initialization was successful
*/
bool ad4858_init(ad4858_t *settings);

bool ad4858_handler_pico_cmos_reciever_polling(ad4858_t *settings, uint32_t* data);

bool ad4858_handler_pico_cmos_crc_check(ad4858_t *settings, uint8_t const message[], int nBytes, uint16_t recieved_crc);

/*! \brief Perform a read-modify-write operation on a register
 *  \param settings   Pointer to AD4858 device settings
 *  \param adr        Target register address
 *  \param data       New data to be written (masked)
 *  \param mask       Bit mask to specify which bits to modify (1 = modify, 0 = leave unchanged)
 */
void ad4858_handler_pico_spi_rmw(ad4858_t *settings, uint16_t adr, uint8_t data, uint8_t mask);

/*! \brief Send a software reset command to the AD4858
 *  \param settings   Pointer to AD4858 device settings
 */
void ad4858_handler_spi_send_sw_reset(ad4858_t *settings);

/*! \brief Enable or disable 4-wire SPI mode
 *  \param settings   Pointer to AD4858 device settings
 *  \param enable     true to enable 4-wire mode, false to disable
 */
void ad4858_handler_spi_set_4_wire(ad4858_t *settings, bool enable);

/*! \brief Enable or disable streaming instruction mode
 *  \param settings   Pointer to AD4858 device settings
 *  \param enable     true to enable streaming instruction mode, false to disable
 */
void ad4858_handler_spi_set_streaming_instruction_mode(ad4858_t *settings, bool enable);

/*! \brief Read the 16-bit product ID of the AD4858
 *  \param settings   Pointer to AD4858 device settings
 *  \return           16-bit product ID (concatenation of PRODUCT_ID_H and PRODUCT_ID_L)
 */
uint16_t ad4858_handler_spi_get_prod_id(ad4858_t *settings);

/*! \brief Test SPI communication by writing and verifying scratch register
 *  \param settings   Pointer to AD4858 device settings
 *  \return           true if communication succeeded (read value matches written value), false otherwise
 */
bool ad4858_handler_spi_test_com(ad4858_t *settings);

/*! \brief Read the device status register
 *  \param settings   Pointer to AD4858 device settings
 *  \return           Byte containing the current device status
 */
uint8_t ad4858_handler_spi_get_device_status(ad4858_t *settings);

/*! \brief Set the ADC data packet size
 *  \param settings     Pointer to AD4858 device settings
 *  \param packet_size  2-bit value for packet size (0 = 20-bit, 1 = 24-bit, 2 = 32-bit, 3 = 32-bit)
 */
void ad4858_handler_spi_set_packet_size(ad4858_t *settings, uint16_t packet_size);

/*! \brief Enable or disable the test pattern toggle bit
 *  \param settings   Pointer to AD4858 device settings
 *  \param enable     true to enable test pattern toggle, false to disable
 */
void ad4858_handler_spi_set_test_pat_toggle(ad4858_t *settings, bool enable);

/*! \brief Configure the SoftSpan voltage range for a specific channel
 *  \param settings   Pointer to AD4858 device settings
 *  \param channel    ADC channel number (0 to 7)
 *  \param lvl        SoftSpan level (4-bit value according to datasheet range table)
 */
void ad4858_handler_spi_set_softspan(ad4858_t *settings, uint8_t channel, uint8_t lvl);

void ad4858_start_conv(ad4858_t* settings);

#endif