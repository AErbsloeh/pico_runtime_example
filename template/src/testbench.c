#include "src/testbench.h"
#include "hardware_io.h"


// ============================ HELP FUNCTION FOR TESTING ============================
void mode_tb_switch(void){
		sleep_us(1);
}


// ============================ MAIN FUNCTION ============================
bool run_testbench(uint8_t mode){
    switch(mode){
        case TB_MODE_NONE:      sleep_us(1);                            break;
        case TB_MODE_MUX:       mode_tb_switch();             			break;
        default:                printf("... Testmode not available\n"); break;
    }
    return true;
}
