#include "src/usb_callbacks.h"
#include "hardware_io.h"


// ============================= COMMANDS =============================
typedef enum {
    ECHO,
    STATE,
    RUNTIME,
    ENABLE_LED,
    DISABLE_LED,
    TOGGLE_LED
} usb_cmd_t;


// ========================== PROCOTOL FUNCS ==========================
void echo(char* buffer){
    send_bytes(buffer, sizeof(buffer));
}


void get_state(char* buffer){
    buffer[0] = system_state;
    send_bytes(buffer, sizeof(buffer));
}


void get_runtime(char* buffer){
    char buffer_send[9] = {buffer[2]};
    uint64_t runtime = get_runtime_ms();
    for(uint8_t idx = 0; idx < 8; idx++){
        buffer_send[idx+1] = (runtime >> (8 * idx)) & 0xFF;
    }
    send_bytes(buffer_send, sizeof(buffer_send));
}


void enable_led(void){
    set_default_led(true);
}


void disable_led(void){
    set_default_led(false);
}


void toogle_led(void){
    set_default_led(!get_default_led());
}


// ======================== CALLABLE FUNCS ==========================
bool apply_usb_callback(usb_fifo_t* fifo_buffer){
    //Getting data
    handling_usb_fifo_buffer(fifo_buffer);
    
    // Datahandler    
    if(fifo_buffer->ready){
        char* buffer = *fifo_buffer->data;
        switch(buffer[2]){
            case ECHO:          echo(buffer);           break;
            case STATE:         get_state(buffer);      break; 
            case RUNTIME:       get_runtime(buffer);    break;
            case ENABLE_LED:    enable_led();           break;
            case DISABLE_LED:   disable_led();          break;
            case TOGGLE_LED:    toogle_led();           break;
            default:            sleep_us(10);           break;        
        }  
    }
    return true;
}
      
