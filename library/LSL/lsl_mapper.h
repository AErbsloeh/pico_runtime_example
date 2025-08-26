#ifndef LSL_MAPPER_H_
#define LSL_MAPPER_H_


#include <stdint.h>
#include <stdbool.h>




bool
lsl_build_frame(
    uint8_t cmd,
    uint8_t len,
    const void *data,
    uint8_t *lsl_frame);


/*! \brief Function for performing the data transmission - Sending: [Start][Cmd][Len][data0..data3][Checksum]
 * \param lsl_frame     Baudrate of UART communication
 * \param len           Length of data
 * \return              Checksum of data packet
*/
bool lsl_do_transmission(uint8_t *lsl_frame, uint8_t len);


#endif
