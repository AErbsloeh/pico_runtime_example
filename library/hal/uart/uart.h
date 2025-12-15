#ifndef HAL_UART_H_
#define HAL_UART_H_


#include "pico/stdlib.h"
#include "hardware/uart.h"


/*! \brief Struct handler for configuring the UART interface of RP2040
 *  \param pin_tx         GPIO number used for UART TX
 *  \param pin_rx         GPIO number used for UART RX
 *  \param uart_id        UART instance (e.g. uart0 or uart1)
 *  \param baudrate       Baud rate for UART communication
 *  \param data_bits      Number of data bits (5–8)
 *  \param stop_bits      Number of stop bits (1 or 2)
 *  \param parity         UART parity configuration (none, even, odd)
 *  \param cts            Enable/disable CTS (clear to send) flow control
 *  \param rts            Enable/disable RTS (request to send) flow control
 *  \param fifo           Enable/disable FIFO usage
 *  \param init_done      Flag indicating whether the UART has been initialized
 */
typedef struct{
    //HW
    uint8_t pin_tx;
    uint8_t pin_rx;
    uart_inst_t *uart_id;
    uint baudrate;
    //Format
    uint data_bits; 
    uint stop_bits;
    uart_parity_t parity;
    //Flow & FiFo
    bool cts;
    bool rts;
    bool fifo;
    bool init_done;
} uart_t;


/*! \brief Initializes the UART module with given configuration
 *  \param handler Pointer to uart_t containing UART settings
 *  \return true if initialization was successful, false otherwise
 */
bool init_uart_module(uart_t *handler);


/*! \brief Configures the UART hardware based on handler settings
 *  \param handler Pointer to uart_t containing UART settings
 *  \return true if configuration was successful, false otherwise
 */
bool configure_uart_module(uart_t *handler);


/*! \brief Sends raw binary data over UART
 *  \param handler   Pointer to configured uart_t
 *  \param buffer_tx Pointer to buffer containing data to transmit
 *  \param len_tx    Number of bytes to transmit
 */
void construct_uart_write_data(uart_t *handler, uint8_t buffer_tx[], size_t len_tx);


/*! \brief Sends a null-terminated string over UART
 *  \param handler Pointer to configured uart_t
 *  \param s       Null-terminated string to transmit
 */
void construct_uart_write_string(uart_t *handler, const char *s);


/*! \brief Reads binary data from UART into buffer
 *  \param handler   Pointer to configured uart_t
 *  \param buffer_rx Pointer to buffer to store received data
 *  \param len_rx    Number of bytes to read from UART
 */
void construct_uart_read_data(uart_t *handler, uint8_t buffer_rx[], size_t len_rx);


/*! \brief Checks if UART RX buffer has data to read
 *  \param handler      Pointer to configured uart_t
 *  \return             true if data is available to read, false otherwise
 */
bool is_readable(uart_t *handler);


/*! \brief Function for reading string from UART module
 *  \param handler      Pointer to configured uart_t
 *  \param buffer_rx    Pointer to buffer to store received string
 *  \param len_rx       Maximum length of the string to read
 *  \return             true if data is available to read, false otherwise
 */
void construct_uart_read_string(uart_t *handler, char buffer_rx[], size_t len_rx);


#endif