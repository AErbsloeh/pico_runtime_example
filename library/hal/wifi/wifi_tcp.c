#include "hal/wifi/wifi_tcp.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "secrets.h"
#include "src/daq.h"

#define MAX_CLIENTS 4
static struct tcp_pcb *clients[MAX_CLIENTS] = {0};

err_t tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }

    //printf("Recieved (%d Bytes) over WIFI: %.*s\n", p->len, p->len, (char *)p->payload);
    daq_handle_message(p->payload, p->len);
    tcp_recved(tpcb, p->len);
    pbuf_free(p);

    return ERR_OK;
}

err_t tcp_accept_callback(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    //register client in array and accept connection if we havent reached the limit
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] == NULL) {
            clients[i] = client_pcb;
            tcp_recv(client_pcb, tcp_recv_callback);
            printf("Client %d connected\n", i);
            return ERR_OK;
        }
    }

    // Kein Platz mehr
    tcp_close(client_pcb);
    return ERR_ABRT;
}

void set_static_ip(wifi_handler_t* handler) {
    struct netif *netif = netif_list;
    dhcp_stop(netif);
    ip4_addr_t ip, netmask, gw;
    ip4addr_aton(handler->static_ip, &ip);
    ip4addr_aton(handler->netmask, &netmask);
    ip4addr_aton(handler->gateway, &gw);
    netif_set_addr(netif, &ip, &netmask, &gw);
    netif_set_up(netif);
}


void wifi_handler_broadcast_string(wifi_handler_t* handler, const char *msg) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i]) {
            err_t err = tcp_write(clients[i], msg, strlen(msg), TCP_WRITE_FLAG_COPY);
            if (err == ERR_OK) {
                tcp_output(clients[i]);
            } else {
                printf("Error sending to Client %d\n", i);
            }
        }
    }
}

void wifi_handler_broadcast_data(wifi_handler_t* handler, uint8_t *data, uint8_t data_len) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i]) {
            err_t err = tcp_write(clients[i], data, data_len, TCP_WRITE_FLAG_COPY);
            if (err == ERR_OK) {
                tcp_output(clients[i]);
            } else {
                printf("Error sending to Client %d\n", i);
            }
        }
    }
}


bool wifi_handler_init(wifi_handler_t* handler){
    //Start WIFI
    if(cyw43_arch_init_with_country(handler->country)){
        printf("WIFI init failed\n");
        return false;
    }
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(wifi_ssid, wifi_pass, CYW43_AUTH_WPA2_AES_PSK, 10000)){
        printf("Couldnt connect to WIFI network %s\n", wifi_ssid);
        return false;
    }
    if(!handler->use_dhcp) set_static_ip(handler);
    struct netif *netif = netif_list;
    netif->hostname = handler->hostname;

    //Start TCP Server
    struct tcp_pcb *pcb = tcp_new();
    err_t e = tcp_bind(pcb, IP_ADDR_ANY, handler->port);
    if (e != ERR_OK) {
        printf("Error binding Port %d: %d\n", handler->port, e);
        return false;
    }
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, tcp_accept_callback);

    handler->init_done = true;
    return true;
}