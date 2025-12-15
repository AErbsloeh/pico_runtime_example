#include "src/daq_sample.h"
#include "hal/usb/usb.h"


//==================== CALLABLE FUNCS ====================//
bool init_daq_sampling(tmr_repeat_irq_t* handler){
    return init_timer_irq(handler);
};


bool update_daq_sampling_rate(tmr_repeat_irq_t* handler, int64_t new_rate_us){
    bool daq_enabled = handler->enable_state;
    if(daq_enabled){
        disable_repeat_timer_irq(handler);
    };
    
    handler->period_us = new_rate_us;

    if(daq_enabled){
        enable_repeat_timer_irq(handler);
    }
    return daq_enabled;
};


bool start_daq_sampling(tmr_repeat_irq_t* handler){
    return enable_repeat_timer_irq(handler);
};


bool stop_daq_sampling(tmr_repeat_irq_t* handler){
    return disable_repeat_timer_irq(handler);
};


void send_daq_data_usb(daq_data_t* data){
    char buffer[14] = {0};

    buffer[0] = data->packet_id;
    buffer[1] = data->iteration;
    uint64_t runtime = data->runtime;
    for(uint8_t idx = 0; idx < 8; idx++){
        buffer[idx+2] = (uint8_t)runtime;
        runtime >>= 8;
    };
    buffer[10] = data->channel_id;
    buffer[11] = (uint8_t)(data->value >> 0);
    buffer[12] = (uint8_t)(data->value >> 8);
    buffer[13] = 0xFF;

    usb_send_bytes(buffer, sizeof(buffer));
};
