#include "sens/m24m01e.h"


// ======================================== FUNCTIONS ===============================================
bool M24M01E_init(m24m01e_t *handler){
    gpio_init(handler->gpio_rnw);
    gpio_set_dir(handler->gpio_rnw, true);
    gpio_put(handler->gpio_rnw, true);
    return false;
}


bool M24M01E_read_data(m24m01e_t *handler){

    return false;
}


bool M24M01E_write_data(m24m01e_t *handler){

    return false;
}


bool M24M01E_delete_flash(m24m01e_t *handler){

    return false;
}
