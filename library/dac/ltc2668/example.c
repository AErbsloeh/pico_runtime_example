#include <stdio.h>
#include <stdlib.h>

#include "dac/ltc2668/ltc2668.h"


int main(){ 
    static ltc2668_t LTC2668_DEFAULT_CONFIG = {
		.spi_handler = &DEVICE_SPI_DEFAULT,
		.gpio_num_csn = PICO_DEFAULT_SPI_CSN_PIN,
		.gpio_num_clrn = 0,
		.use_clrn_hw = false,
		.pwr_up_chnnl = LTC2668_PWRUP_DAC_ALL,
		.use_int_vref = true,
		.vref_range = LTC2668_RANGE_BI_VREF,
		.init_done = false
	};

    // Init of device
    ltc2668_init(&setting_device);
    ltc2668_mux_control(&setting_device, 0);

    //Main Loop for communication
    uint16_t dac_out = 0;
    uint8_t mux_chnnl = 0;
    uint8_t cnt = 0;

    while (true){
        ltc2668_update_output_all_channel(&setting_device, dac_out);
        dac_out += 5;
        sleep_us(100);
        if (cnt >= 200){
            cnt = 0;
            if(mux_chnnl >= 16){
                mux_chnnl = 0;
            } else {
                mux_chnnl = ++;
            };
            ltc2668_mux_control(&setting_device, mux_chnnl);
        } else {
            cnt++;
        }
    };
}
