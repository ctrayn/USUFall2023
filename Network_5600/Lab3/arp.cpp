#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "frameio2.h"
#include "util.h"
#include "arp.h"
#include "common.h"

extern frameio net;
extern message_queue arp_queue;
ether_frame buf;

int read_dst_ip(octet data[1500]) {
   //std::cout << std::stoi(frame.data[DST_IP_START],nullptr,8) << std::endl;
   int my_ip = data[DST_IP_START] << 24 | data[DST_IP_START + 1] << 16 | data[DST_IP_START + 2] << 8 | data[DST_IP_START + 3];
   // std::cout << std::hex << my_ip << std::endl;
   return my_ip;
}

void read_dst_mac(ether_frame* buf) {
   for (int i = 0; i < 6; i++) {
      buf->dst_mac[i] = buf->data[DST_MAC_START + i];
   }
}

int read_src_ip(octet data[1500]) {
   //std::cout << std::stoi(frame.data[DST_IP_START],nullptr,8) << std::endl;
   int my_ip = data[SRC_IP_START] << 24 | data[SRC_IP_START + 1] << 16 | data[SRC_IP_START + 2] << 8 | data[SRC_IP_START + 3];
   // std::cout << std::hex << my_ip << std::endl;
   return my_ip;
}

cache_data check_is_cached(octet* ip, std::vector<cache_data> cache) {
   for (int i = 0; i < cache.size(); i++) {
      if (cache[i].ip[0] == ip[0] && 
          cache[i].ip[1] == ip[1] && 
          cache[i].ip[2] == ip[2] &&
          cache[i].ip[3] == ip[3]) {
         return cache[i];
      }
   }
   cache_data not_found = cache_data{0,0,0};
   return not_found;
}

void print_cache_data(cache_data data) {
   printf("IP: %d.%d.%d.%d Mac %02X:%02X:%02X:%02X:%02X:%02X\n",
      data.ip[0], data.ip[1], data.ip[2], data.ip[3],
      data.mac[0], data.mac[1], data.mac[2], data.mac[3], data.mac[4], data.mac[5]);
}

void print_cache_stack(std::vector<cache_data> cache) {
   for (int i = 0; i < cache.size(); i++) {
      printf("Cache stack: %d: ", i);
      print_cache_data(cache[i]);
   }
}

void *arp_protocol_loop(void *arg) {
   std::vector<cache_data> cache;
   event_kind event;
   int my_ip = get_local_ip();
   int dst_ip = 0;
   int src_ip = 0;

   int n = 0;
   octet request_ip[4] = {129,123,4,83};
   int request_ip_int = request_ip[0] << 24 || request_ip[1] << 16 || request_ip[2] << 8 || request_ip[3];
   printf("Request ip %d.%d.%d.%d %d", request_ip[0],request_ip[1],request_ip[2],request_ip[3],request_ip_int);
   bool response_received = false;

   ether_frame buf;
   buf.prot[0] = 0x08;
   buf.prot[1] = 0x06; //ARP
   get_local_mac(&buf); //Get my own MAC Address
   printf("My IP: %d.%d.%d.%d\n", (my_ip >> 24) & 0xFF, (my_ip >> 16) & 0xFF, (my_ip >> 8) & 0xFF, my_ip & 0xFF);
   printf("My MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", buf.src_mac[0], buf.src_mac[1], buf.src_mac[2], buf.src_mac[3], buf.src_mac[4], buf.src_mac[5]);
   while ( 1 ) {
      arp_queue.recv(&event, buf.data, sizeof(buf.data));
      dst_ip = read_dst_ip(buf.data);
      src_ip = read_src_ip(buf.data);
      if (dst_ip == my_ip && buf.data[6] == 0x0 && buf.data[7] == 0x2) {   //If it's my ip, and it's a request
         read_dst_mac(&buf);
         // std::cout << "SRC IP: 0x" << read_src_ip(buf.data) << " MAC " << buf.dst_mac << std::endl;
         printf("IP: %d.%d.%d.%d Mac %02X:%02X:%02X:%02X:%02X:%02X\n",
          (src_ip >> 24) & 0xFF, (src_ip >> 16) & 0xFF, (src_ip >> 8) & 0xFF, (src_ip) & 0xFF,
          buf.dst_mac[0], buf.dst_mac[1], buf.dst_mac[2], buf.dst_mac[3], buf.dst_mac[4], buf.dst_mac[5]);
         
         printf("Replying MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
          buf.src_mac[0], buf.src_mac[1], buf.src_mac[2], buf.src_mac[3], buf.src_mac[4], buf.src_mac[5]);

         buf.data[0] = 0x00;   //Hardware Type 1
         buf.data[1] = 0x01;   //Hardware Type 2
         buf.data[2] = 0x08;   //Protocol Type 1
         buf.data[3] = 0x00;   //Protocol Type 2
         buf.data[4] = 0x06;   //Hardware Size
         buf.data[5] = 0x04;   //Protocol Size
         buf.data[6] = 0x00;   //OPCODE
         buf.data[7] = 0x02;   //OPCODE Reply

         buf.data[8] = buf.src_mac[0];
         buf.data[9] = buf.src_mac[1];
         buf.data[10] = buf.src_mac[2];
         buf.data[11] = buf.src_mac[3];
         buf.data[12] = buf.src_mac[4];
         buf.data[13] = buf.src_mac[5];

         buf.data[14] = (my_ip >> 24) & 0xFF;
         buf.data[15] = (my_ip >> 16) & 0xFF;
         buf.data[16] = (my_ip >> 8 ) & 0xFF;
         buf.data[17] = (my_ip >> 0 ) & 0xFF;

         buf.data[18] = buf.dst_mac[0];
         buf.data[19] = buf.dst_mac[1];
         buf.data[20] = buf.dst_mac[2];
         buf.data[21] = buf.dst_mac[3];
         buf.data[22] = buf.dst_mac[4];
         buf.data[23] = buf.dst_mac[5];

         buf.data[24] = (src_ip >> 24) & 0xFF;
         buf.data[25] = (src_ip >> 16) & 0xFF;
         buf.data[26] = (src_ip >> 8 ) & 0xFF;
         buf.data[27] = (src_ip >> 0 ) & 0xFF;

         net.send_frame(&buf, sizeof(buf.dst_mac) + sizeof(buf.src_mac) + sizeof(buf.prot) + (sizeof(octet) * 32));
      }
      // std::cout << "Got out of IF" << std::endl;
      // print_cache_stack(cache);

      octet data_ip[4];
      data_ip[0] = (src_ip >> 24) & 0xFF;
      data_ip[1] = (src_ip >> 16) & 0xFF;
      data_ip[2] = (src_ip >> 8 ) & 0xFF;
      data_ip[3] = (src_ip >> 0 ) & 0xFF;

      if (check_is_cached(data_ip, cache).time == 0) {
         cache_data data;
         int temp_ip = read_src_ip(buf.data);
         data.ip[0]  = buf.data[DST_IP_START + 0];
         data.ip[1]  = buf.data[DST_IP_START + 1];
         data.ip[2]  = buf.data[DST_IP_START + 2];
         data.ip[3]  = buf.data[DST_IP_START + 3];
         data.mac[0] = buf.data[SRC_MAC_START + 0];
         data.mac[1] = buf.data[SRC_MAC_START + 1];
         data.mac[2] = buf.data[SRC_MAC_START + 2];
         data.mac[3] = buf.data[SRC_MAC_START + 3];
         data.mac[4] = buf.data[SRC_MAC_START + 4];
         data.mac[5] = buf.data[SRC_MAC_START + 5];
         data.time = time(nullptr);
         printf("Adding to cache: ");
         print_cache_data(data);
         cache.push_back(data);
      }
      else {
         for (int i = 0; i < cache.size(); i++) {
            if (cache[i].ip == data_ip) {
               cache[i].time = time(nullptr);
               break;
            }
         }
      }

      for (int i = 0; i < cache.size(); i++) {
         // printf("Current Time: %d Cache time: %d ", time(nullptr), cache[i].time);
         // print_cache_data(cache[i]);
         if (cache[i].time + CACHE_TIMEOUT <= time(nullptr)) {
            printf("Removing from cache ");
            print_cache_data(cache[i]);
            cache.erase(cache.begin() + i);
            i--;
         }
      }

      if (dst_ip == my_ip && buf.data[6] == 0x0 && buf.data[7] == 0x5) {   //If it's my ip, and it's a response
         printf("Response recieved\n***********************\n\n************************\n");
         response_received = true;
      }

      if (n >= 10 || response_received) {
         //Every arbitrary time interval, send a ARP request
         n = 0;
         cache_data cached_ip = check_is_cached(request_ip, cache);

         buf.data[0] = 0x00;   //Hardware Type 1
         buf.data[1] = 0x01;   //Hardware Type 2
         buf.data[2] = 0x08;   //Protocol Type 1
         buf.data[3] = 0x00;   //Protocol Type 2
         buf.data[4] = 0x06;   //Hardware Size
         buf.data[5] = 0x04;   //Protocol Size
         buf.data[6] = 0x00;   //OPCODE
         buf.data[7] = 0x01;   //OPCODE Request

         buf.data[8] = buf.src_mac[0];
         buf.data[9] = buf.src_mac[1];
         buf.data[10] = buf.src_mac[2];
         buf.data[11] = buf.src_mac[3];
         buf.data[12] = buf.src_mac[4];
         buf.data[13] = buf.src_mac[5];

         buf.data[14] = (my_ip >> 24) & 0xFF;
         buf.data[15] = (my_ip >> 16) & 0xFF;
         buf.data[16] = (my_ip >> 8 ) & 0xFF;
         buf.data[17] = (my_ip >> 0 ) & 0xFF;

         buf.data[24] = request_ip[0];
         buf.data[25] = request_ip[1];
         buf.data[26] = request_ip[2];
         buf.data[27] = request_ip[3];

         if (cached_ip.time == 0) { //Not in the cache
            buf.data[18] = 0x0;
            buf.data[19] = 0x0;
            buf.data[20] = 0x0;
            buf.data[21] = 0x0;
            buf.data[22] = 0x0;
            buf.data[23] = 0x0;

            buf.dst_mac[0] = 0xFF;
            buf.dst_mac[1] = 0xFF;
            buf.dst_mac[2] = 0xFF;
            buf.dst_mac[3] = 0xFF;
            buf.dst_mac[4] = 0xFF;
            buf.dst_mac[5] = 0xFF;
            printf("Broadcasting to %d.%d.%d.%d\n", request_ip[0],request_ip[1],request_ip[2],request_ip[3]);
         }
         else {   //Found in cache
            buf.data[18] = cached_ip.mac[0];
            buf.data[19] = cached_ip.mac[1];
            buf.data[20] = cached_ip.mac[2];
            buf.data[21] = cached_ip.mac[3];
            buf.data[22] = cached_ip.mac[4];
            buf.data[23] = cached_ip.mac[5];

            buf.dst_mac[0] = cached_ip.mac[0];
            buf.dst_mac[1] = cached_ip.mac[1];
            buf.dst_mac[2] = cached_ip.mac[2];
            buf.dst_mac[3] = cached_ip.mac[3];
            buf.dst_mac[4] = cached_ip.mac[4];
            buf.dst_mac[5] = cached_ip.mac[5];

            printf("Pinging %d.%d.%d.%d - %X:%X:%X:%X:%X:%X\n",
               request_ip[0],request_ip[1],request_ip[2],request_ip[3],
               buf.dst_mac[0],buf.dst_mac[1],buf.dst_mac[2],buf.dst_mac[3],buf.dst_mac[4],buf.dst_mac[5]);
         }
         net.send_frame(&buf, sizeof(buf.dst_mac) + sizeof(buf.src_mac) + sizeof(buf.prot) + (sizeof(octet) * 32));
         response_received = false;
      }
      n++;
   }
}
