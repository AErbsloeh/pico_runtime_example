#ifndef DAQ_SAMPLE_H_
#define DAQ_SAMPLE_H_


#include "hal/tmr/tmr.h"


/*! \brief Data structure for DAQ sampling data packet
* \param packet_id  Identifier for the data packet type
* \param iteration  Iteration count of the sampling
* \param runtime    Actual runtime in microseconds since system start
* \param channel_id Identifier for the data channel
* \param value      Sampled data value
*/
typedef struct {
    uint8_t packet_id;
    uint8_t iteration;
    uint64_t runtime;
    uint8_t channel_id;
    uint16_t value;
} daq_data_t;


/*! \brief Function to init the DAQ sampling unit
* \param handler    Pointer to the timer IRQ handler structure
* \return           true if initialization was successful, false otherwise
*/
bool init_daq_sampling(tmr_repeat_irq_t* handler);


/*! \brief Function to update the sampling rate of the DAQ sampling unit
* \param handler        Pointer to the timer IRQ handler structure
* \param new_rate_us    New sampling rate in microseconds
* \return               State of the DAQ sampling unit (true=running, false=deactivated)
*/
bool update_daq_sampling_rate(tmr_repeat_irq_t* handler, int64_t new_rate_us);


/*! \brief Function to start the DAQ sampling unit
* \param handler        Pointer to the timer IRQ handler structure
* \return               State of the DAQ sampling unit (true=running, false=deactivated)
*/
bool start_daq_sampling(tmr_repeat_irq_t* handler);


/*! \brief Function to stop the DAQ sampling unit
* \param handler        Pointer to the timer IRQ handler structure
* \return               State of the DAQ sampling unit (true=deactivated, false=activated)
*/
bool stop_daq_sampling(tmr_repeat_irq_t* handler);


/*! \brief Function to send the DAQ data packet via USB
* \param data           Pointer to the DAQ data packet
*/
void send_daq_data_usb(daq_data_t* data);


#endif
