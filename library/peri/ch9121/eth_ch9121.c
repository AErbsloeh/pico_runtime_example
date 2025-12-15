#include "peri/ch9121/eth_ch9121.h"


bool eth_ch9121_init(eth_ch9121_t *settings){
    if(!settings->uart_handler->init_done){
        configure_uart_module(settings->uart_handler);
    }
    configure_uart_module(settings->uart_handler);

    settings->init_done = true;
    return settings->init_done;
};


void eth_ch9121_write_data(eth_ch9121_t *settings, uint8_t buffer_tx[], size_t len_tx){
    construct_uart_write_data(settings->uart_handler, buffer_tx, len_tx);
};


void eth_ch9121_write_string(eth_ch9121_t *settings, const char *s){
    construct_uart_write_string(settings->uart_handler, s);
};


void eth_ch9121_read_data(eth_ch9121_t *settings, uint8_t buffer_rx[], size_t len_rx){
    if(is_readable(settings->uart_handler)) {
        construct_uart_read_data(settings->uart_handler, buffer_rx, len_rx);
    }
};


void eth_ch9121_read_string(eth_ch9121_t *settings, char buffer_rx[], size_t len_rx){
    construct_uart_read_string(settings->uart_handler, buffer_rx, len_rx);
};
