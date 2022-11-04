#ifndef ARP_H
#define ARP_H


#define SRC_MAC_START 8
#define SRC_IP_START 24
#define DST_IP_START 14
#define DST_MAC_START 18
#define CACHE_TIMEOUT 20
#define IP_TYPE 4

struct cache_data {
   octet ip[IP_TYPE];
   octet mac[6];
   // string net;
   time_t time;
};

void *arp_protocol_loop(void *arg);

#endif //ARP_H