#include "hal/wifi/wifi_udp.h"
#ifdef PICO_CYW43_SUPPORTED
    #include "pico/cyw43_arch.h"
    #include "lwip/udp.h"
    #include "lwip/ip_addr.h"

    struct udp_pcb *pcb;
    ip_addr_t dest_addr;
#endif


bool cyw43_wireless_init(cyw43_wlan_t* config){
    #ifndef PICO_CYW43_SUPPORTED
        config->init_done = false;
    #else
        cyw43_arch_init();
        pcb = udp_new();
        config->init_done = true;
    #endif
    return config->init_done;
}


bool cyw43_wireless_connect(cyw43_wlan_t* config){
    #ifndef PICO_CYW43_SUPPORTED
        config->is_connected = false;
    #else
        if(!config->init_done){
            if(!cyw43_wireless_init(config))
                return false;
        }

        cyw43_arch_enable_sta_mode();
        if(cyw43_arch_wifi_connect_timeout_ms(
            config->ssid, 
            config->password, 
            CYW43_AUTH_WPA2_AES_PSK,
            10000
        )){
            cyw43_arch_deinit();
            config->is_connected = false;
        } else {
            ipaddr_aton(config->host_ip, &dest_addr);
            config->is_connected = true;
        };        
    #endif
    return config->is_connected;
}


bool cyw43_wireless_disconnect(cyw43_wlan_t* config){
    #ifndef PICO_CYW43_SUPPORTED
        return false;
    #else
        cyw43_arch_disable_sta_mode();
        cyw43_arch_deinit();
        return true;
    #endif
}


bool cyw43_wireless_send_data_udp(cyw43_wlan_t* config, char* data, size_t len){
    #ifndef PICO_CYW43_SUPPORTED
        return false;
    #else
        if(!config->init_done){
            return false;
        } else {
            struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
            memcpy(p->payload, data, len);

            udp_sendto(pcb, p, &dest_addr, config->port);
            pbuf_free(p);
            
            return true;
        }
    #endif
}
