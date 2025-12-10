#ifndef DAQ_SAMPLE_H_
#define DAQ_SAMPLE_H_

#include "hal/tmr.h"



typedef struct {
    uint8_t packet_id;
    uint8_t iteration;
    uint64_t runtime;
    uint8_t channel_id;
    uint16_t value;
} daq_data_t;



bool init_daq_sampling(tmr_repeat_irq_t* handler);


bool update_daq_sampling_rate(tmr_repeat_irq_t* handler, int64_t new_rate_us);


bool start_daq_sampling(tmr_repeat_irq_t* handler);


bool stop_daq_sampling(tmr_repeat_irq_t* handler);


bool send_daq_data_usb(daq_data_t* data);


#endif
