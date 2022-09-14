#include "frameio.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

frameio net;             // gives us access to the raw network
message_queue ip_queue;  // message queue for the IP protocol stack
message_queue arp_queue; // message queue for the ARP protocol stack

struct ether_frame       // handy template for 802.3/DIX frames
{
   octet dst_mac[6];     // destination MAC address
   octet src_mac[6];     // source MAC address
   octet prot[2];        // protocol (or length)
   octet data[1500];     // payload
};

//Recieve frames from the network
void *protocol_loop(void *arg) {
    ether_frame buffer;
    u_int16_t protocol;
    const u_int16_t IP_PROTOCOL  = 0x800;   //Name the magic numbers
    const u_int16_t ARP_PROTOCOL = 0x806;

    while (true) {
        int num = net.recv_frame(&buffer, sizeof(buffer));
        if (num >= 42) {    //Frame must be >= 42 to be valid
            protocol = (buffer.prot[0] << 8) | (buffer.prot[1]);
            switch (protocol) {
                case IP_PROTOCOL:
                    ip_queue.send(PACKET, buffer.data, num);
                    break;

                case ARP_PROTOCOL:
                    arp_queue.send(PACKET, buffer.data, num);
                    break;
                
                default:
                    /* Bad frame */
                    break;
            }
        }
    }
}

void *ip_loop(void *arg) {
    octet buffer[1500];
    event_kind event;
    int timer_no = 1;
    
    while (true) {
        ip_queue.recv(&event, buffer, sizeof(buffer));

        if (event == PACKET) {
            for (int i = 0; i < 22; i++) {
                printf("%02X ", buffer[i]);
            }
            printf("\n");
            for (int i = 22; i < 42; i++) {
                printf("%02X ", buffer[i]);
            }
            ip_queue.timer(10, timer_no);
            timer_no++;
        }
    }
}

void *arp_loop(void *arg) {
    octet buffer[1500];
    event_kind event;

    while (true) {
        arp_queue.recv(&event, buffer, sizeof(buffer));
        printf("Got ARP packet %s\n", buffer[7] == 1? "request":"reply");
    }
}

pthread_t loop_thread, arp_thread, ip_thread;

int main() {

    system("ifconfig > ifresult.log");
    // net.open_net("/* TODO */");
    // pthread_create(&loop_thread, NULL, protocol_loop, NULL);
    // pthread_create(&arp_thread,  NULL, arp_loop,      NULL);
    // pthread_create(&ip_thread,   NULL, ip_loop,       NULL);

    // while(true) {
    //     sleep(1);
    // }
}