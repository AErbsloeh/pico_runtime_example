#include "src/rpc_callbacks.h"
#include "hardware_io.h"


// ============================= COMMANDS =============================
typedef enum {
    ECHO,
    RESET,
    STATE_SYS,
    STATE_PIN,
    RUNTIME,
    ENABLE_LED,
    DISABLE_LED,
    TOGGLE_LED,
    START_DAQ,
    STOP_DAQ,
    UPDATE_DAQ,
} usb_cmd_t;


// ========================== PROCOTOL FUNCS ==========================
void echo(char* buffer, size_t length){
    usb_send_bytes(buffer, length);
}


void system_reset(void){
    reset_pico_mcu(true);
}


void get_state_system(char* buffer, size_t length){
    buffer[2] = system_state;
    usb_send_bytes(buffer, length);
}


void get_state_pin(char* buffer, size_t length){
    buffer[2] = get_default_led();
    usb_send_bytes(buffer, length);
}


void get_runtime(char* buffer){
    char buffer_send[9] = {0};
    buffer_send[0] = buffer[0];
    uint64_t runtime = get_runtime_ms();
    for(uint8_t idx = 0; idx < 8; idx++){
        buffer_send[idx+1] = (uint8_t)runtime;
        runtime >>= 8;
    }
    usb_send_bytes(buffer_send, sizeof(buffer_send));
}


void enable_led(void){
    set_default_led(true);
}


void disable_led(void){
    set_default_led(false);
}


void toogle_led(void){
    toggle_default_led();
}


void start_daq(void){
    set_system_state(STATE_DAQ);
    start_daq_sampling(&tmr_daq0_hndl);
}


void stop_daq(void){
    set_system_state(STATE_IDLE);
    stop_daq_sampling(&tmr_daq0_hndl);
}


void update_daq(char* buffer){
    uint64_t new_rate_us = (buffer[1] << 16) | (buffer[2] << 8);
    update_daq_sampling_rate(&tmr_daq0_hndl, (int64_t)(-1)*new_rate_us);
}


// ======================== CALLABLE FUNCS ==========================
bool apply_rpc_callback(char* buffer, size_t length, bool ready){    
    if(ready){
        switch(buffer[0]){
            case ECHO:          echo(buffer, length);               break;
            case RESET:         system_reset();                     break;
            case STATE_SYS:     get_state_system(buffer, length);   break;
            case STATE_PIN:     get_state_pin(buffer, length);      break; 
            case RUNTIME:       get_runtime(buffer);                break;
            case ENABLE_LED:    enable_led();                       break;
            case DISABLE_LED:   disable_led();                      break;
            case TOGGLE_LED:    toogle_led();                       break;
            case START_DAQ:     start_daq();                        break;
            case STOP_DAQ:      stop_daq();                         break;
            case UPDATE_DAQ:    update_daq(buffer);                 break;
            default:            sleep_us(10);                       break;        
        }  
    }
    return true;
}
      