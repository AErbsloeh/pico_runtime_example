#include "mux/adg1408/adg1408.h"
#include "hardware/gpio.h"


bool adg1408_init(adg1408_t *handler){
    gpio_init(handler->gpio_num_en);
    gpio_set_dir(handler->gpio_num_en, GPIO_OUT);
    gpio_put(handler->gpio_num_en, false);

    gpio_init(handler->gpio_num_a0);
    gpio_set_dir(handler->gpio_num_a0, GPIO_OUT);
    gpio_put(handler->gpio_num_a0, false);

    gpio_init(handler->gpio_num_a1);
    gpio_set_dir(handler->gpio_num_a1, GPIO_OUT);
    gpio_put(handler->gpio_num_a1, false);

    gpio_init(handler->gpio_num_a2);
    gpio_set_dir(handler->gpio_num_a2, GPIO_OUT);
    gpio_put(handler->gpio_num_a2, false);

    handler->init_done = true;
    return handler->init_done;
}


void adg1408_select_chnnl(adg1408_t *handler, uint8_t sel_chnnl){ 
    if(!handler->init_done){
        adg1408_init(handler);
    } else {
        switch(sel_chnnl & 0x0F){
            case 1:
                gpio_put(handler->gpio_num_en, true);
                gpio_put(handler->gpio_num_a0, false);
                gpio_put(handler->gpio_num_a1, false);
                gpio_put(handler->gpio_num_a2, false);
            break;
            case 2:
                gpio_put(handler->gpio_num_en, true);
                gpio_put(handler->gpio_num_a0, true);
                gpio_put(handler->gpio_num_a1, false);
                gpio_put(handler->gpio_num_a2, false);
            break;
            case 3:
                gpio_put(handler->gpio_num_en, true);
                gpio_put(handler->gpio_num_a0, false);
                gpio_put(handler->gpio_num_a1, true);
                gpio_put(handler->gpio_num_a2, false);
            break;
            case 4:
                gpio_put(handler->gpio_num_en, true);
                gpio_put(handler->gpio_num_a0, true);
                gpio_put(handler->gpio_num_a1, true);
                gpio_put(handler->gpio_num_a2, false);
            break;
            case 5:
                gpio_put(handler->gpio_num_en, true);
                gpio_put(handler->gpio_num_a0, false);
                gpio_put(handler->gpio_num_a1, false);
                gpio_put(handler->gpio_num_a2, true);
            break;
            case 6:
                gpio_put(handler->gpio_num_en, true);
                gpio_put(handler->gpio_num_a0, true);
                gpio_put(handler->gpio_num_a1, false);
                gpio_put(handler->gpio_num_a2, true);
            break;
            case 7:
                gpio_put(handler->gpio_num_en, true);
                gpio_put(handler->gpio_num_a0, false);
                gpio_put(handler->gpio_num_a1, true);
                gpio_put(handler->gpio_num_a2, true);
            break;
            case 8:
                gpio_put(handler->gpio_num_en, true);
                gpio_put(handler->gpio_num_a0, true);
                gpio_put(handler->gpio_num_a1, true);
                gpio_put(handler->gpio_num_a2, true);
            break;
            default:
                gpio_put(handler->gpio_num_en, false);
                gpio_put(handler->gpio_num_a0, false);
                gpio_put(handler->gpio_num_a1, false);
                gpio_put(handler->gpio_num_a2, false);
            break;
        }
    };
}
