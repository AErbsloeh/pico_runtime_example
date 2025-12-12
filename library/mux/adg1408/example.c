#include <stdio.h>
#include <stdlib.h>

#include "mux/adg1408/adg1408.h"


int main(){ 
    adg1408_t setting_device = {
        .gpio_num_en = 0,
        .gpio_num_a0 = 1,
        .gpio_num_a1 = 2,
        .gpio_num_a2 = 3,
        .init_done = false
    }

    // Init of device
    if(adg1408_init(&setting_device)) prinft("Init of MUX ADG1408 done\n");

    //Main Loop for communication
    uint8_t cnt = 0;

    while (true){
        adg1408_select_chnnl(&setting_device, cnt);
        sleep_ms(1000);
        if(cnt >= 8){
            cnt = 0;
        } else {
            cnt++;
        };
    };
}
