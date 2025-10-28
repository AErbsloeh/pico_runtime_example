#include "src/usb_callbacks.h"
#include "hardware_io.h"


// ============================= COMMANDS =============================
typedef enum {
    ECHO,
    STATE,
    ENABLE_LED,
    DISABLE_LED,
    TOGGLE_LED
} usb_cmd_t;


// ======================== INTERNAL FUNCTIONS ========================
void send_bytes(char* buffer, size_t num_bytes){
    for(size_t idx = 0; idx < num_bytes; idx++){
        putchar(buffer[num_bytes- 1 - idx]);
    }
}


// ========================== PROCOTOL FUNCS ==========================
void echo(char* buffer){
    send_bytes(buffer, sizeof(buffer));
}


void send_state(char* buffer){
    buffer[0] = system_state;
    send_bytes(buffer, sizeof(buffer));
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
            case STATE:         send_state(buffer);     break; 
            case ENABLE_LED:    enable_led();           break;
            case DISABLE_LED:   disable_led();          break;
            case TOGGLE_LED:    toogle_led();           break;
            default:            sleep_ms(1);            break;        
        }  
    }
    return true;
}
      
