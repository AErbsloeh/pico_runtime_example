#include "src/usb_callbacks.h"
#include "wrapper/led_control.h"


// ============================= COMMANDS =============================
#define USB_CMD_ECHO    	0x00
#define USB_CMD_EN_LED      0x01
#define USB_CMD_DIS_LED     0x02
#define USB_CMD_TGG_LED     0x03


// ========================== PROCOTOL FUNCS ==========================
void echo(char* buffer){
    uint8_t lgth = sizeof(buffer);
    for(uint8_t idx = 0; idx < lgth; idx++){
        putchar(buffer[lgth-1-idx]);
    }
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
            case USB_CMD_ECHO:      echo(buffer);       break;
            case USB_CMD_EN_LED:    enable_led();       break;
            case USB_CMD_DIS_LED:   disable_led();      break;
            case USB_CMD_TGG_LED:   toogle_led();       break;
            default:                sleep_ms(1);        break;        
        }  
    }
    return true;
}
      
