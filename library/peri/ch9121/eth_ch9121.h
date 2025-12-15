#ifndef ETH_CH9121_H_
#define ETH_CH9121_H_


#include "hal/uart/uart.h"


typedef struct {
    uart_t* uart_handler;
    bool init_done;
} eth_ch9121_t;


bool eth_ch9121_init(eth_ch9121_t *settings);


void eth_ch9121_write_data(eth_ch9121_t *settings, uint8_t buffer_tx[], size_t len_tx);


void eth_ch9121_write_string(eth_ch9121_t *settings, const char *s);


void eth_ch9121_read_data(eth_ch9121_t *settings, uint8_t buffer_rx[], size_t len_rx);


void eth_ch9121_read_string(eth_ch9121_t *settings, char buffer_rx[], size_t len_rx);


#endif
