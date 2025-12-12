// *****************************************************************************
/* EXAMPLE_START(spp_counter): SPP Server - Heartbeat Counter over RFCOMM
 *
 * @text The Serial port profile (SPP) is widely used as it provides a serial
 * port over Bluetooth. The SPP counter example demonstrates how to setup an SPP
 * service, and provide a periodic timer over RFCOMM.   
 *
 * @text Note: To test, please run the spp_counter example, and then pair from 
 * a remote device, and open the Virtual Serial Port.
 */
// *****************************************************************************

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "btstack_run_loop.h"


#define ENABLE_CLASSIC
#define RFCOMM_SERVER_CHANNEL 1
#define HEARTBEAT_PERIOD_MS 1000


static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);


static uint16_t rfcomm_channel_id;
static uint8_t  spp_service_buffer[150];
static btstack_packet_callback_registration_t hci_event_callback_registration;


// ========================================================================================
/* @section Periodic Timer Setup
 * 
 * @text The heartbeat handler increases the real counter every second, 
 * and sends a text string with the counter value, as shown in Listing PeriodicCounter. 
 */

/* LISTING_START(PeriodicCounter): Periodic Counter */ 
static btstack_timer_source_t heartbeat;
static char lineBuffer[30];
static void  heartbeat_handler(struct btstack_timer_source *ts){
    static int counter = 0;

    if (rfcomm_channel_id){
        snprintf(lineBuffer, sizeof(lineBuffer), "BTstack counter %04u\n", ++counter);
        printf("%s", lineBuffer);

        rfcomm_request_can_send_now_event(rfcomm_channel_id);
    }

    btstack_run_loop_set_timer(ts, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(ts);
} 


/* @section Bluetooth Logic 
 * @text The Bluetooth logic is implemented within the 
 * packet handler, see Listing SppServerPacketHandler. In this example, 
 * the following events are passed sequentially: 
 * - BTSTACK_EVENT_STATE,
 * - HCI_EVENT_PIN_CODE_REQUEST (Standard pairing) or 
 * - HCI_EVENT_USER_CONFIRMATION_REQUEST (Secure Simple Pairing),
 * - RFCOMM_EVENT_INCOMING_CONNECTION,
 * - RFCOMM_EVENT_CHANNEL_OPENED, 
* - RFCOMM_EVETN_CAN_SEND_NOW, and
 * - RFCOMM_EVENT_CHANNEL_CLOSED
 */

/* @text Upon receiving HCI_EVENT_PIN_CODE_REQUEST event, we need to handle
 * authentication. Here, we use a fixed PIN code "0000".
 *
 * When HCI_EVENT_USER_CONFIRMATION_REQUEST is received, the user will be 
 * asked to accept the pairing request. If the IO capability is set to 
 * SSP_IO_CAPABILITY_DISPLAY_YES_NO, the request will be automatically accepted.
 *
 * The RFCOMM_EVENT_INCOMING_CONNECTION event indicates an incoming connection.
 * Here, the connection is accepted. More logic is need, if you want to handle connections
 * from multiple clients. The incoming RFCOMM connection event contains the RFCOMM
 * channel number used during the SPP setup phase and the newly assigned RFCOMM
 * channel ID that is used by all BTstack commands and events.
 *
 * If RFCOMM_EVENT_CHANNEL_OPENED event returns status greater then 0,
 * then the channel establishment has failed (rare case, e.g., client crashes).
 * On successful connection, the RFCOMM channel ID and MTU for this
 * channel are made available to the heartbeat counter. After opening the RFCOMM channel, 
 * the communication between client and the application
 * takes place. In this example, the timer handler increases the real counter every
 * second. 
 *
 * RFCOMM_EVENT_CAN_SEND_NOW indicates that it's possible to send an RFCOMM packet
 * on the rfcomm_cid that is include

 */ 

/* LISTING_START(SppServerPacketHandler): SPP Server - Heartbeat Counter over RFCOMM */
static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(channel);

    bd_addr_t event_addr;
    uint8_t   rfcomm_channel_nr;
    uint16_t  mtu;
    int i;

    switch (packet_type) {
        case HCI_EVENT_PACKET:
            switch (hci_event_packet_get_type(packet)) {
                case HCI_EVENT_PIN_CODE_REQUEST:
                    // inform about pin code request
                    printf("Pin code request - using '0000'\n");
                    hci_event_pin_code_request_get_bd_addr(packet, event_addr);
                    gap_pin_code_response(event_addr, "0000");
                    break;

                case HCI_EVENT_USER_CONFIRMATION_REQUEST:
                    // ssp: inform about user confirmation request
                    printf("SSP User Confirmation Request with numeric value '%06"PRIu32"'\n", little_endian_read_32(packet, 8));
                    printf("SSP User Confirmation Auto accept\n");
                    break;

                case RFCOMM_EVENT_INCOMING_CONNECTION:
                    rfcomm_event_incoming_connection_get_bd_addr(packet, event_addr);
                    rfcomm_channel_nr = rfcomm_event_incoming_connection_get_server_channel(packet);
                    rfcomm_channel_id = rfcomm_event_incoming_connection_get_rfcomm_cid(packet);
                    printf("RFCOMM channel %u requested for %s\n", rfcomm_channel_nr, bd_addr_to_str(event_addr));
                    rfcomm_accept_connection(rfcomm_channel_id);
                    break;
               
                case RFCOMM_EVENT_CHANNEL_OPENED:
                    if (rfcomm_event_channel_opened_get_status(packet)) {
                        printf("RFCOMM channel open failed, status %u\n", rfcomm_event_channel_opened_get_status(packet));
                    } else {
                        rfcomm_channel_id = rfcomm_event_channel_opened_get_rfcomm_cid(packet);
                        mtu = rfcomm_event_channel_opened_get_max_frame_size(packet);
                        printf("RFCOMM channel open succeeded. New RFCOMM Channel ID %u, max frame size %u\n", rfcomm_channel_id, mtu);
                    }
                    break;
                case RFCOMM_EVENT_CAN_SEND_NOW:
                    rfcomm_send(rfcomm_channel_id, (uint8_t*) lineBuffer, (uint16_t) strlen(lineBuffer));  
                    break;
       
                case RFCOMM_EVENT_CHANNEL_CLOSED:
                    printf("RFCOMM channel closed\n");
                    rfcomm_channel_id = 0;
                    break;
                
                default:
                    break;
            }
            break;

        case RFCOMM_DATA_PACKET:
            printf("RCV: '");
            for (i=0;i<size;i++){
                putchar(packet[i]);
            }
            printf("'\n");
            break;

        default:
            break;
    }
/* LISTING_RESUME */ 
}
/* LISTING_END */


bool btstack_spp_init(void){
    // set one-shot timer
    heartbeat.process = &heartbeat_handler;
    btstack_run_loop_set_timer(&heartbeat, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(&heartbeat);

    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    l2cap_init();
    rfcomm_init();
    rfcomm_register_service(packet_handler, RFCOMM_SERVER_CHANNEL, 0xffff);  // reserved channel, mtu limited by l2cap

    // init SDP, create record for SPP and register with SDP
    sdp_init();
    memset(spp_service_buffer, 0, sizeof(spp_service_buffer));
    spp_create_sdp_record(spp_service_buffer, 0x10001, RFCOMM_SERVER_CHANNEL, "PICO SPP");
    sdp_register_service(spp_service_buffer);
    printf("SDP service record size: %u\n", de_get_len(spp_service_buffer));

    gap_discoverable_control(1);
    gap_ssp_set_io_capability(SSP_IO_CAPABILITY_DISPLAY_YES_NO);
    gap_set_local_name("PICO SPP 00:00:00:00:00:00");

    // turn on!
    hci_power_control(HCI_POWER_ON);
}


bool btstack_spp_start(void){
    btstack_run_loop_execute();
    return true;
}
