#ifndef WIFI_UDP_H_
#define WIFI_UDP_H_


#include "pico/stdlib.h"


// ==================================== COMMENT ====================================
/* In order to use the WLAN UDP Communication the following CMake options must be addded to the target_link_libraries:
 * - pico_cyw43_arch_lwip_poll
 *
 * The wlan_udp module provides functions to initialize, connect, disconnect and send data via UDP over WLAN.
*/


/*! \brief  Struct for handling the Wireless Communication between RP2xxx and Host Computer
* \param ssid           String array with SSID from host computer
* \param passwort       String array with password for SSID
* \param host_ip        String with Host IP
* \param port           Integer with port number
* \param is_connected   Boolean for connection state
* \param init_done      Boolean for initialization state
*/
typedef struct {
    char ssid[32];
    char password[64];
    char host_ip[16];
    uint16_t port;    
    bool is_connected;
    bool init_done;
} cyw43_wlan_t;


/*! \brief  Initializes the WLAN module CYW43 within the Pico xW or custom designs
* \param config     Pointer to cyw43_wlan_t struct
* \return           Boolean indicating success of initialization
*/
bool cyw43_wireless_init(cyw43_wlan_t* config);


/*! \brief Connecting the WLAN module CYW43 to a host computer
* \param config     Pointer to cyw43_wlan_t struct
* \return           Boolean indicating success of connection
*/
bool cyw43_wireless_connect(cyw43_wlan_t* config);


/*! \brief Disconnecting the WLAN module CYW43 from a host computer
* \param config     Pointer to cyw43_wlan_t struct
* \return           Boolean indicating success of disconnection
*/
bool cyw43_wireless_disconnect(cyw43_wlan_t* config);


/*! \brief Sending data packages to the host computer
* \param config     Pointer to cyw43_wlan_t struct
* \param data       Pointer to data array to be sent
* \param len        Length of data array
* \return           Boolean indicating success of data transmission
*/
bool cyw43_wireless_send_data_udp(cyw43_wlan_t* config, char* data, size_t len);


#endif
