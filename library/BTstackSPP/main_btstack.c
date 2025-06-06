#include "sens/btstack_spp_counter.h"


int main(){   
    // Init Phase 
	stdio_init_all();
    if (cyw43_arch_init()) {
		return false;
	}

    btstack_spp_init();
    btstack_spp_start();

    // Main Loop
    while (true) {  
        sleep_ms(1);
    }
}
