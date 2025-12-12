#include "hal/uart/uart.h"


bool configure_uart_module(uart_t *handler){
    uart_init(handler->uart_id, handler->baudrate);
    gpio_set_function(handler->pin_tx, GPIO_FUNC_UART);
    gpio_set_function(handler->pin_rx, GPIO_FUNC_UART);

    uart_set_format(handler->uart_id, handler->data_bits, handler->stop_bits, handler->parity);
    uart_set_hw_flow(handler->uart_id, handler->cts, handler->rts);
    uart_set_fifo_enabled(handler->uart_id, handler->fifo);

    handler->init_done = true;
    return handler->init_done;
}


void construct_uart_write_data(uart_t *handler, uint8_t buffer_tx[], size_t len_tx){
    uart_write_blocking(handler->uart_id, buffer_tx, len_tx);
};


void construct_uart_write_string(uart_t *handler, const char *s){
    uart_puts(handler->uart_id, s);
};


void construct_uart_read_data(uart_t *handler, uint8_t buffer_rx[], size_t len_rx){
    uint8_t i = 0;
    while (is_readable(handler) && i < len_rx)
    {
        uint8_t ch = uart_getc(handler->uart_id);
        buffer_rx[i] = ch;
        i++;
    }
};


void construct_uart_read_string(uart_t *handler, char buffer_rx[], size_t len_rx){
    uint8_t i = 0;
    while (is_readable(handler) && i < len_rx)
    {
        char ch = uart_getc(handler->uart_id);
        if (ch == '\n' || ch == '\r') buffer_rx[i] = '\0';
        else buffer_rx[i] = ch;
        i++;
    }
};


bool is_readable(uart_t *handler){
    return uart_is_readable(handler->uart_id);
};
