#include "hardware_io.h"
#include "lib/ws2812.h"
#include "src/waveform_lut1.h"
#include "src/timer_daq.h"


/* --------------- CODE FOR TIMER INTERRUPT SERVICE ROUTINE --------------- */
static uint32_t cnt_timer_pos = 0;
bool process_timer0_irq(repeating_timer_t *rt) {
    int16_t data_used = read_waveform_value_runtime_1(true);

    // --- Do filter calculation
    printf("W%d\n", data_used);

    // --- Do LED control
    if(cnt_timer_pos % 3 == 0) put_pixel_rgb(0, led_intensity, 0);
    else if(cnt_timer_pos % 3 == 1) put_pixel_rgb(led_intensity, 0, 0);
    else if(cnt_timer_pos % 3 == 2) put_pixel_rgb(0, 0, led_intensity);
    else put_pixel_rgb(0, 0, 0);

    cnt_timer_pos += 1;
    // Return
    return alarm_done; 
};


bool process_timer1_irq(repeating_timer_t *rt) {
    /* CODE HAS TO BE INCLUDED */
    return true;
};


/* --------------- CODE FOR TIMER ALARM --------------- */
static volatile bool alarm_done = false;
static void init_timer_irq_alarm(void) {
    hw_clear_bits(&timer_hw->intr, 1u);
    alarm_done = true;
};


/* --------------- CODE FOR INIT TIMER --------------- */
static repeating_timer_t timer0;
void init_timer0_isr(bool enable_timer, int32_t period_callback){
    // Register control
    if(enable_timer && !irq_is_enabled(TIMER_IRQ_0)){
        hw_set_bits(&timer_hw->inte, 1u);
        irq_set_exclusive_handler(TIMER_IRQ_0, init_timer_irq_alarm);
        irq_set_enabled(TIMER_IRQ_0, true);
        timer_hw->alarm[0] = timer_hw->timerawl + 1000000;

        // Register control (wait until done)
        while(!alarm_done){
            sleep_ms(1);
        }

        // Register        
        if (!add_repeating_timer_us(period_callback, process_timer0_irq, NULL, &timer0)) {
            printf("Failed to add timer\n");
        };
    } else if(!enable_timer && irq_is_enabled(TIMER_IRQ_0)) {
        irq_set_enabled(TIMER_IRQ_0, false);
        alarm_done = false;
        while(alarm_done){
            sleep_ms(1);
        }
        irq_clear(TIMER_IRQ_0);
    } else {
        sleep_ms(1);
    }     
};

static repeating_timer_t timer1;
void init_timer1_isr(bool enable_timer, int32_t period_callback){
    // Register control
    if(enable_timer && !irq_is_enabled(TIMER_IRQ_1)){
        hw_set_bits(&timer_hw->inte, 1u);
        irq_set_exclusive_handler(TIMER_IRQ_1, init_timer_irq_alarm);
        irq_set_enabled(TIMER_IRQ_1, true);
        timer_hw->alarm[0] = timer_hw->timerawl + 1000000;

        // Register control (wait until done)
        while(!alarm_done){
            sleep_ms(1);
        }

        // Register        
        if (!add_repeating_timer_us(period_callback, process_timer1_irq, NULL, &timer1)) {
            printf("Failed to add timer\n");
        };
    } else if(!enable_timer && irq_is_enabled(TIMER_IRQ_1)) {
        irq_set_enabled(TIMER_IRQ_1, false);
        alarm_done = false;
        while(alarm_done){
            sleep_ms(1);
        }
        irq_clear(TIMER_IRQ_1);
    } else {
        sleep_ms(1);
    }     
};
