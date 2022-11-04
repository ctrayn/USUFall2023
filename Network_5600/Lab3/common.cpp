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
#include "common.h"

void get_local_mac(ether_frame* buf) {
   std::ifstream mac_file;
   mac_file.open("/sys/class/net/eno1/address");
   std::string line;
   getline(mac_file, line);
   mac_file.close();

   int length = 0;
   std::string first = line.substr(length, 2);
   length += first.length() + 1;
   std::string second = line.substr(length, 2);
   length += second.length() + 1;
   std::string third = line.substr(length, 2);
   std::cout << line << std::endl;
   length += third.length() + 1;
   std::string fourth = line.substr(length, 2);
   length += fourth.length() + 1;
   std::string fifth = line.substr(length, 2);
   length += fifth.length() + 1;
   std::string sixth = line.substr(length, 2);

   buf->src_mac[0] = std::stol(first, 0, 16);
   buf->src_mac[1] = std::stol(second, 0, 16);
   buf->src_mac[2] = std::stol(third, 0, 16);
   buf->src_mac[3] = std::stol(fourth, 0, 16);
   buf->src_mac[4] = std::stol(fifth, 0, 16);
   buf->src_mac[5] = std::stol(sixth, 0, 16);
}

int get_local_ip() {
   system("ifconfig > if_output");
   std::ifstream ip_file;
   ip_file.open("if_output");
   std::string line;
   getline(ip_file, line);
   getline(ip_file, line);
   line = line.substr(line.find("inet") + 5);
   line = line.substr(0,line.find(" "));
   ip_file.close();
   
   std::string consumed = line;
   std::string first = consumed.substr(0,consumed.find("."));
   consumed = consumed.substr(consumed.find(".") + 1);
   std::string second = consumed.substr(0,consumed.find("."));
   consumed = consumed.substr(consumed.find(".") + 1);
   std::string third = consumed.substr(0, consumed.find("."));
   consumed = consumed.substr(consumed.find(".") + 1);
   //std::cout << consumed << std::endl;
   int my_ip = std::stoi(first) << 24 | std::stoi(second) << 16 | std::stoi(third) << 8 | std::stoi(consumed);
   //std::cout << "'" << line << "' 0x" << std::hex << my_ip << std::endl;
   return my_ip;
}

ip_oct get_gateway_ip() {
   std::ifstream ip_file;
   ip_file.open("gateway_output");
   std::string line;
   if (!ip_file.is_open()) {
      printf("Not open\n");
      exit(1);
   }
   getline(ip_file, line);
   line = line.substr(line.find("via") + 4);
   line = line.substr(0, line.find("dev") - 1);
   // std::cout << line << std::endl;
   ip_file.close();

   ip_oct ip = {};
   std::string temp;
   for (int i = 0; i < 4; i++) {
      temp = line.substr(0, line.find('.'));
      line = line.substr(line.find('.') + 1);
      ip.ip[i] = stol(temp);
      // printf("%d\n", ip.ip[i]);
   }
   return ip;
}

ip_oct get_subnet_mask() {
   std::ifstream ip_file;
   ip_file.open("gateway_output");
   std::string line;
   if (!ip_file.is_open()) {
      printf("Not open\n");
      exit(1);
   }
   getline(ip_file, line);
   getline(ip_file, line);
   // std::cout << line << std::endl;
   
   ip_oct ip;
   ip.num_bytes = stol(line.substr(line.find("/") + 1, 3));
   // printf("%d\n", ip.num_bytes);

   line = line.substr(0, line.find("/"));

   std::string temp;
   for (int i = 0; i < 4; i++) {
      temp = line.substr(0, line.find("."));
      // std::cout << temp << std::endl;
      line = line.substr(line.find(".") + 1);
      // std::cout << line << std::endl;
      ip.ip[i] = stol(temp);
      // printf("%d\n", ip.ip[i]);
   }
   return ip;
}
