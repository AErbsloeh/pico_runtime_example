#ifndef ADC_CMOS_H_
#define ADC_CMOS_H_

#include "sens/ad4858.h"
#include "hardware_io.h"

typedef struct {
    uint32_t channel_results[8]; //when a channel is left out because of Status, its 0x00
    uint8_t OR_UR; //OR_UR Flags for all channels, lsb = channel 0, msb = channel 7
    uint8_t status;
    uint16_t crc;
} cmos_result_t;

bool init_adc_cmos(void);

bool cmos_start_reading(void (*callback)(cmos_result_t* out_data));
#endif