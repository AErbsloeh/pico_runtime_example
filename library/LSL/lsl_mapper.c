#include "src/lsl_mapper.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"


// ========================= INTERNAL FUNCTIONS ==========================
uint8_t calc_checksum(uint8_t *lsl_frame, uint16_t len) {
    uint16_t sum = 0;    
    for (uint16_t idx=0; idx < len; idx++)
        sum += lsl_frame[idx];
    
    lsl_frame[len] = (uint8_t)(sum & 0xFF);
    return lsl_frame[len];
}


// ========================= CALLABLE FUNCTIONS ==========================
bool lsl_init(void){
    stdio_init_all();
	stdio_set_translate_crlf(&stdio_usb, false);
    return true;
}


bool lsl_build_frame(uint8_t cmd, uint16_t len_data, uint8_t *data, uint8_t *lsl_frame){
    lsl_frame[0] = LSL_FRAME_START;
    lsl_frame[1] = cmd;
    lsl_frame[2] = len_data;
    for(uint16_t idx=0; idx < len_data; idx++)    lsl_frame[idx+3] = data[idx];
    calc_checksum(lsl_frame, len_data+3);
    
    return true;
}


bool lsl_do_transmission(uint8_t *frame, uint16_t len) {
    size_t written = fwrite(frame, 1, len, stdout);
    fflush(stdout);
    return (written == len);
}




