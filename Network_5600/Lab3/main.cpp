#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "frameio2.h"
#include "util.h"
#include "ip.h"
#include "arp.h"
#include "common.h"

frameio net;             // gives us access to the raw network
message_queue ip_queue;  // message queue for the IP protocol stack
message_queue arp_queue; // message queue for the ARP protocol stack
message_queue ip_arp_comms; //To talk back and forth

//
// This thread sits around and receives frames from the network.
// When it gets one, it dispatches it to the proper protocol stack.
//
void *protocol_loop(void *arg)
{
   ether_frame buf;
    while(1) {
        int n = net.recv_frame(&buf,sizeof(buf));
        if ( n < 42 ) continue; // bad frame!
        switch ( buf.prot[0]<<8 | buf.prot[1] ) {
            case 0x800:
                ip_queue.send(PACKET,buf.data,n);
                break;
            case 0x806:
                arp_queue.send(PACKET,buf.data,n);
                break;
        }
    }
}

//
// if you're going to have pthreads, you'll need some thread descriptors
//
pthread_t loop_thread, arp_thread, ip_thread;

//
// start all the threads then step back and watch (actually, the timer
// thread will be started later, but that is invisible to us.)
//
int main()
{
   net.open_net("eno1");
   pthread_create(&loop_thread,NULL,protocol_loop,NULL);
//    pthread_create(&arp_thread,NULL,arp_protocol_loop,NULL);
   pthread_create(&ip_thread,NULL,ip_protocol_loop,NULL);
   for ( ; ; )
      sleep(1);
}

