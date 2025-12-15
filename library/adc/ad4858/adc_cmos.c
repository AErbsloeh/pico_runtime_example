#include "sens/ad4858.h"
#include "hardware_io.h"


// ================= CODE FOR INTERRUPT CONTROL & COMMUNICATION; USES HARDWARE DEFINED IN HARDWAREIO =================

ad4858_t* adc_handler = CMOS_ADC;

//CMOS Status
uint32_t cmos_data_buffer[8];
cmos_result_t cmos_result_buffer;
uint8_t cmos_sck_countr;
bool cmos_is_reading;
void (*done_reading_callback)(cmos_result_t* out_data);

uint8_t SDO_with_status;
bool no_status;

void reset_cmos_result(cmos_result_t* res) {
    memset(res->channel_results, 0, sizeof(res->channel_results));
    res->OR_UR = 0;
    res->status = 0;
    res->crc = 0;
}

void cmos_analyze_packets(){
    for(uint8_t i = 0; i < 8; i++){
        //printf("Channel %d raw: 0x%08X\n", i, cmos_data_buffer[i]);
        if(i == SDO_with_status && !no_status){
            //handle Status
        }
        else{
            if(adc_handler->cmos_package_size == 20){
                //handle 20 Bit Package size
            }
            else if(adc_handler->cmos_package_size == 24){
                //handle 24 Bit Package size

                //channel id are bit 2, 1 and 0
                uint32_t channel_id = (cmos_data_buffer[i] & 0x0E) >> 1;
                //printf("CID Bit 1: %d\n", cmos_data_buffer[i] & 0x02);
                //printf("CID Bit 2: %d\n", cmos_data_buffer[i] & 0x04);
                //printf("CID Bit 3: %d\n", cmos_data_buffer[i] & 0x08);
                //printf("CID: %d\n", channel_id);

                //4th bit is OR/UR flag 
                cmos_result_buffer.OR_UR |= (cmos_data_buffer[i] & 0x08) << channel_id;

                //bit 23 to 4 are conv Data
                cmos_result_buffer.channel_results[channel_id] = (cmos_data_buffer[i] >> 4);
            }
            else{
                //handle 32 Bit Package size
            }
        }
    }
}


//read 1 bit from all 8 channels
void cmos_read_bits(void){
    uint8_t offset = adc_handler->cmos_package_size-cmos_sck_countr-1;
    for (uint8_t chnnl = 0; chnnl < 8; chnnl++){
        if(gpio_get(adc_handler->gpio_SDO[chnnl])){
            cmos_data_buffer[chnnl] |= 1 << (offset);
        }
    }
    cmos_sck_countr++;
}

void cmos_pwm_wrap_sck_callback(void){
    if(cmos_is_reading){
        //Dont read on last falling clock
        cmos_read_bits();
        if(cmos_sck_countr - 1 < adc_handler->cmos_package_size){
            //cmos_read_bits();
        }
        //Reading is done
        if(cmos_sck_countr >= adc_handler->cmos_package_size){
            cmos_is_reading = false;
            construct_pwm_enable_disable(adc_handler->cmos_pwm_handler, false); //Disable PWM
            gpio_put(adc_handler->gpio_csn, true); //set CS Pin high
            cmos_analyze_packets();

            if(no_status || SDO_with_status == 0){
                no_status = false;
                SDO_with_status = 7;
            }
            else{
                SDO_with_status--;
            }

            done_reading_callback(&cmos_result_buffer);
        }
    }
}

//callback is called once reading is done
bool cmos_start_reading(void (*callback)(cmos_result_t* out_data)){
    if(cmos_is_reading){
        return false;
    } else {
        //set variables
        cmos_sck_countr = 0;
        cmos_is_reading = true;
        for(uint8_t i = 0; i < 8; i++) cmos_data_buffer[i] = 0;
        done_reading_callback = callback;
        reset_cmos_result(&cmos_result_buffer);
        
        //set CS Pin low
        gpio_put(adc_handler->gpio_csn, false);

        //Read first Bits
        cmos_read_bits();
        //Enable PWM
        construct_pwm_enable_disable(adc_handler->cmos_pwm_handler, true);
        return true;
    }
};


bool init_adc_cmos(void){
    //Register callback function for CMOS SCK
    construct_irq_add_pwm_wrap(adc_handler->gpio_cmos_sck, &cmos_pwm_wrap_sck_callback);

    cmos_sck_countr = 0;
    cmos_is_reading = false;
    no_status = true;
    SDO_with_status = 0;
    return true;
}