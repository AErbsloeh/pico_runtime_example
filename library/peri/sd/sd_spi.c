#include "peri/sd/sd_spi.h"
#include "ff.h"
#include "f_util.h"
#include "sd_card.h"
#include "hardware/spi.h"


// ======================================== MOUNT FILE ===============================================
static FATFS fs;

// ==================================== SD CARD INSTANCE =============================================
static spi_t sd_spi = {
    .miso_gpio = PICO_DEFAULT_SPI_RX_PIN,
    .mosi_gpio = PICO_DEFAULT_SPI_TX_PIN,
    .sck_gpio = PICO_DEFAULT_SPI_SCK_PIN,
    .baud_rate = 1 * 1000 * 1000, // 1 MHz
    .spi_mode = 0,
    .hw_inst = PICO_DEFAULT_SPI, 
    .no_miso_gpio_pull_up = true,
    .set_drive_strength = false,
    .use_static_dma_channels = false
};
static sd_spi_if_t spi_if = {
    .spi = &sd_spi,
    .set_drive_strength = false,
    .ss_gpio = 0
};
static sd_card_t sd_card = {
    .type = SD_IF_SPI,
    .spi_if_p = &spi_if, 
    .use_card_detect = false
};


// ==================================== CALLABLE FUNCS ==============================================
bool sd_spi_init(sd_spi_t *handler){
    uint baudrate_originial = handler->spi->fspi_khz * 1000;

    sd_spi.hw_inst = handler->spi->spi_mod;
    sd_spi.sck_gpio = handler->spi->pin_sclk;
    sd_spi.mosi_gpio = handler->spi->pin_mosi;
    sd_spi.miso_gpio = handler->spi->pin_miso;
    sd_spi.baud_rate = 1000000;
    sd_spi.spi_mode = handler->spi->mode;
    spi_if.ss_gpio = handler->gpio_cs;

    gpio_init(handler->gpio_cs);
    gpio_set_dir(handler->gpio_cs, GPIO_OUT);
    gpio_put(handler->gpio_cs, true);

    if(!handler->spi->init_done){
        configure_spi_module(handler->spi, true);
    }
    if(sd_init_driver()){
        handler->card_available = true;
        /*FRESULT fr = f_mount(&fs, sd_card.pcName, 1);
        if (fr != FR_OK) {
            printf("f_mount failed: %d\n", fr);
        }*/
    } else {
        handler->card_available = false;
    }

    sd_spi.baud_rate = baudrate_originial;
    handler->init_done = true;
    return handler->init_done;
}
