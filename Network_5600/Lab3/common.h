#ifndef COMMON_H
#define COMMON_H


typedef struct ether_frame       // handy template for 802.3/DIX frames
{
   octet dst_mac[6];     // destination MAC address
   octet src_mac[6];     // source MAC address
   octet prot[2];        // protocol (or length)
   octet data[1500];     // payload
} ether_frame;

typedef struct ip_oct {
   octet ip[4];
   int num_bytes;       //not used for gateway, used for subnets
} ip_oct;

typedef struct ip_mac_t {
   bool done;     //The struct has both an IP and a MAC
   octet ip[4];
   octet mac[6];
} ip_mac_t;

void get_local_mac(ether_frame* buf);
int get_local_ip();
ip_oct get_gateway_ip();
ip_oct get_subnet_mask();

#define IP_LENGTH 32

#endif //COMMON_H