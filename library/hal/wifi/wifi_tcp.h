
#ifndef WIFI_HANDLER_H_
#define WIFI_HANDLER_H_

#include "pico/stdlib.h"
#include "lwip/tcp.h"


typedef struct {
    bool init_done;
    bool use_dhcp;
    uint32_t country;
    uint16_t port;
    const char* hostname;

    //only necessary when disabling DHCP
    const char* static_ip;
    const char* netmask;
    const char* gateway;
} wifi_handler_t;


bool wifi_handler_init(wifi_handler_t* handler);


void wifi_handler_broadcast_string(wifi_handler_t* handler, const char *msg);


void wifi_handler_broadcast_data(wifi_handler_t* handler, uint8_t *data, uint8_t data_len);

#endif
