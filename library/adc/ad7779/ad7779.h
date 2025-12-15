#ifndef AD7779_H_
#define AD7779_H_


#include "hal/spi/spi.h"


// TODO: Add hal_clk for clock generation


// More Informations from sensor: https://www.analog.com/media/en/technical-documentation/data-sheets/ad7779.pdf

// ========================= AD7779 DEFINITIONS =========================
// AD7779 Operating Modes
#define DEBUG_MODE 0 //1 for True, 0 for False; Enable/Disable debug output


// AD7779 total Number of Channels
#define NUM_CHANNELS 8


// Register Addresses
#define AD7779_REG_GENERAL_USER_CONFIG_1                            0x011
#define AD7779_REG_GENERAL_USER_CONFIG_2                            0x012
#define AD7779_REG_GENERAL_USER_CONFIG_3                            0x013
#define AD7779_REG_ADC_MUX_CONFIG                                   0x015
#define AD7779_DATA_OUTPUT_FORMAT_REGISTER                          0x014

#define AD7779_REG_GENERAL_ERRORS_REGISTER_1                        0x059
#define AD7779_REG_GENERAL_ERRORS_REGISTER_2                        0x05B
#define AD7779_REG_CHANNEL0_STATUS_REGISTER                         0x04C
#define AD7779_REG_CHANNEL1_STATUS_REGISTER                         0x04D
#define AD7779_REG_CHANNEL2_STATUS_REGISTER                         0x04E
#define AD7779_REG_CHANNEL3_STATUS_REGISTER                         0x04F
#define AD7779_REG_CHANNEL4_STATUS_REGISTER                         0x050
#define AD7779_REG_CHANNEL5_STATUS_REGISTER                         0x051
#define AD7779_REG_CHANNEL6_STATUS_REGISTER                         0x052
#define AD7779_REG_CHANNEL7_STATUS_REGISTER                         0x053
#define AD7779_REG_CHANNEL0_CHANNEL1_DSP_ERRORS_REGISTER            0x054
#define AD7779_REG_CHANNEL2_CHANNEL3_DSP_ERRORS_REGISTER            0x055
#define AD7779_REG_CHANNEL4_CHANNEL5_DSP_ERRORS_REGISTER            0x056
#define AD7779_REG_CHANNEL6_CHANNEL7_DSP_ERRORS_REGISTER            0x057
#define AD7779_REG_ERROR_STATUS_REGISTER_1                          0x05D
#define AD7779_REG_ERROR_STATUS_REGISTER_2                          0x05E
#define AD7779_REG_ERROR_STATUS_REGISTER_3                          0x05F

#define AD7779_REG_DECIMATION_RATE_REGISTER_N_MSB                   0x060
#define AD7779_REG_DECIMATION_RATE_REGISTER_N_LSB                   0x061
#define AD7779_REG_DECIMATION_RATE_REGISTER_IF_MSB                  0x062
#define AD7779_REG_DECIMATION_RATE_REGISTER_IF_LSB                  0x063
#define AD7779_REG_LOAD_UPDATE_REGISTER                             0x064

// Definitions for setting specific bits in registers
#define AD7779_REG_GENERAL_USER_CONFIG_3_VALUE_READ_SPI_ENABLED     0b10010000 // Value read mode for SPI
#define AD7779_REG_GENERAL_USER_CONFIG_3_VALUE_READ_SPI_DISABLED    0b10000000 // Register read mode for SPI

// Values that confirm successful initialization
#define AD7779_REG_ERROR_STATUS_REGISTER_3_INIT_COMPLETE_BIT        (1 << 4)  // Bit 4 = INIT_COMPLETE
#define AD7779_REG_ERROR_STATUS_REGISTER_3_CHIP_ERROR_BIT           (1 << 5)  // Bit 5 = CHIP_ERROR

//Register values for enabling / disabling Channels
#define AD7779_REG_CH_DISABLE                                       0x08

// Registervalues for a Soft-Reset
// Bits [1:0] = SOFT_RESET:
//   - First write operation:  11b → Value 0x03
//   - Second write operation: 10b → Value 0x02
#define AD7779_SOFT_RST_FIRST_WRITE                                 0x03  // Bits[1:0] = 11b (1. Write operation)
#define AD7779_SOFT_RST_SECOND_WRITE                                0x02  // Bits[1:0] = 10b (2. Write operation)



/*! \brief Structure to handle the Analog-Digital Converter AD7779 from Analog Devices. 
+   \param spi           Pointer to spi_device_handler_t for SPI communication
    \param pga_gain     Programmable Gain Amplifier setting (steps of 1, 2, 4, 8)
    \param channel_select  Selected channel 0 to 7 (0b10000010 for channel 7 and 2 are enabled)
    \param high_power_mode   Flag to enable/disable High Power Mode (disabled[false] the low power mode is used)
    \param error_header Flag to enable/disable Error Header in data output(disabled[false] the CRS Header is used)
    \param test_mode    Flag to enable/disable Test Mode
    \param init_done    Flag to indicate if initialization was successful
*/
typedef struct {
    spi_t *spi;
    uint8_t pga_gain;
    uint8_t channel_select;
    bool test_mode;
    bool high_power_mode;
    bool error_header;
    bool init_done;
} ad7779_t;


/*! \brief Structure to setuo and control the GPIO pins for the AD7779.
    \param cs_pin       GPIO pin for Chip-Select (CS)
    \param reset_pin    GPIO pin for Hardware Reset (RESET)
    \param start_pin    GPIO pin for synchronization (START)
    \param shielding_channels_pin GPIO pin for shielding of channels
    \param shielding_ref_pin GPIO pin for shielding of electrode reference
    \param shielding_active_channels Flag to enable/disable shielding for active channels (False, shielding goes to GND)
    \param shielding_electrode_ref Flag to enable/disable shielding for electrode reference (False, shielding goes to GND)
    \param init_done    Flag to indicate if initialization was successful
*/
typedef struct {
    uint8_t cs_pin;
    uint8_t reset_pin;
    uint8_t start_pin;
    uint8_t shielding_channels_pin;
    uint8_t shielding_ref_pin;
    bool shielding_active_channels;
    bool shielding_active_electrode_ref;
    bool init_done;
} ad7779_gpio_t;


// ========================= FUNCTION PROTOTYPES =========================

/*! \brief Function to perform a soft reset on the AD7779.
 *  \param handler  Pointer to typedef struct ad7779_t to handle the settings
 *  \return         true if soft reset was successful, false otherwise.
 */
bool ad7779_soft_reset(ad7779_t *handler, ad7779_gpio_t *gpio_handler);


/*! \brief Function to perform a soft sync on the AD7779.
 *  \param handler  Pointer to typedef struct ad7779_t to handle the settings
 *  \return         true if soft sync was successful, false otherwise.
 */
bool ad7779_soft_sync(ad7779_t *handler, ad7779_gpio_t *gpio_handler);


/*! \brief Function to set the Output Sampling Rate to 2kSPS.
 *  \param handler  Pointer to typedef struct ad7779_t to handle the settings
 */
void ad7779_set_sampling_rate_2kSPS(ad7779_t *handler, ad7779_gpio_t *gpio_handler);



/*! \brief Function to set the Power Mode of the AD7779 and perform a soft sync.
 *  \param handler  Pointer to typedef struct ad7779_t to handle the settings
 */
void ad7779_set_power_mode(ad7779_t *handler, ad7779_gpio_t *gpio_handler);


/*! \brief Function to enable or disable the Test Mode of the AD7779, if enabled all channels get a 280mV Test signal, it also performs a soft sync.
 *  \param handler  Pointer to typedef struct ad7779_t to handle the settings
 */
void ad7779_enable_disable_test_mode(ad7779_t *handler, ad7779_gpio_t *gpio_handler);


/*! \brief Function to initialize the Analog-Digital Converter AD7779 from Analog Devices.
 *  \param handler  Pointer to typedef struct ad7779_t to handle the settings
 *  \return         true if initialization is successful, false otherwise.
 */
bool ad7779_init(ad7779_t *handler, ad7779_gpio_t *gpio_handler);


/*! \brief Function for global configuration of the AD7779 after initialization
 *  \param handler  Pointer to typedef struct ad7779_t to handle the settings
 *  \return         true if the configuration was successful, false otherwise.
 */
bool ad7779_global_config(ad7779_t *handler, ad7779_gpio_t *gpio_handler);


/**
 * \brief Allows to configure the PGA gain for all channels of the AD7779 and performs a soft sync.
 * \param handler   Pointer to typedef struct ad7779_t to handle the settings
 * \return          true if configuration was successful, false otherwise.
 */
bool ad7779_configure_pga_for_all_channels(ad7779_t *handler, ad7779_gpio_t *gpio_handler);


/**
 * \brief Disables or anable the selected channels on the AD7779 and performs a soft sync (it is important that the channel_select field in the handler is set correctly)
 * \param handler   Pointer to typedef struct ad7779_t to handle the settings
 * \return           true if disabling was successful, false otherwise.
 */
bool ad7779_enable_disable_channels(ad7779_t *handler, ad7779_gpio_t *gpio_handler);


/**
 * \brief Returns all channel data from the AD7779 via SPI
 * \param handler   Pointer to typedef struct ad7779_t to handle the settings
 * \return           true if the read was successful, false otherwise.
 */
bool ad7779_read_all_channel_data(ad7779_t *handler, ad7779_gpio_t *gpio_handler, uint8_t *pointer_for_rx);


/**
 * \brief Prints all status and error registers of the AD7779 to the console.
 * \param handler   Pointer to typedef struct ad7779_t to handle the settings
 */               
void ad7779_print_all_status_error_register(ad7779_t *handler, ad7779_gpio_t *gpio_handler);


/**
 * \brief Initializes the Chip Select (CS) pin for the AD7779.
 * \param handler   Pointer to typedef struct ad7779_t to handle the settings
 * \return           true if the initialization was successful, false otherwise.
 */
bool ad7779_init_cs_pin(ad7779_gpio_t *handler);


/**
 * \brief Triggers a hardware reset of the AD7779 via the RESET pin.
 * \param handler   Pointer to typedef struct ad7779_t to handle the settings
 * \return           true if the reset was successful, false otherwise.
 */
bool ad7779_trigger_reset(ad7779_gpio_t *handler);


/**
 * \brief initialize and triggers the Start Pin
 * \param handler   Pointer to typedef struct ad7779_t to handle the settings
 * \return           true if the initialization and trigger was successful, false otherwise.
 */
bool ad7779_start_pin(ad7779_gpio_t *handler);


/**
 * \brief initialize and enable/disable active shielding for channels
 * \param handler   Pointer to typedef struct ad7779_t to handle the settings
 * \return           true if the initialization and trigger was successful, false otherwise.
 */
bool ad7779_enable_disable_shielding_channels(ad7779_gpio_t *handler);


/**
 * \brief initialize and enable/disable active shielding for electrode reference
 * \param handler   Pointer to typedef struct ad7779_t to handle the settings
 * \return           true if the initialization and trigger was successful, false otherwise.
 */
bool ad7779_enable_disable_shielding_electrode_ref(ad7779_gpio_t *handler);


#endif