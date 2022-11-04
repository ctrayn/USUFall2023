#ifndef IP_H
#define IP_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include "frameio2.h"
#include "util.h"
#include "common.h"

using namespace std;

class IPAddress {
    public:
        octet address[4];
        int num_bytes;      //used for sub_net masks

        IPAddress() {
            for (int i = 0; i < 4; i++) {
                address[i] = 0;
            }
            num_bytes = 0;
        }

        IPAddress(string ip_string) {
            string_to_ip(ip_string);
            num_bytes = 0;
        }

        IPAddress(octet in[4]) {
            for (int i = 0; i < 4; i++) {
                address[i] = in[i];
            }
            num_bytes = 0;
        }

        IPAddress(octet a, octet b, octet c, octet d) {
            address[0] = a;
            address[1] = b;
            address[2] = c;
            address[3] = d;
            num_bytes = 0;
        }

        IPAddress(int input) {
            address[0] = (input >> 24) & 0xFF;
            address[1] = (input >> 16) & 0xFF;
            address[2] = (input >>  8) & 0xFF;
            address[3] = (input >>  0) & 0xFF;
            num_bytes = 0;
        }

        string get_ip_from_user();
        void string_to_ip(string ip_string);
        bool validate_ip(string ip_string);
        int to_int();
        string get_string();
        bool on_same_subnet(const IPAddress other, int mask_bytes);

        inline bool operator== (const IPAddress one) {
            for (int i = 0; i < 4; i++) {
                if (this->address[i] != one.address[i]) {
                    return false;
                }
            }
            return true;
        } 
};

// #define IP_ADDRESS_DEST_START 30
#define IP_ADDRESS_DEST_START 16
#define IP_ADDRESS_SRC_START 12
#define IP_PROTOCOL 9
#define ICMP_PROTOCOL_NUM 0x01
#define ICMP_REQUEST 0x08
#define ICMP_REPLY 0x00
#define ICMP_TYPE 20

void *ip_protocol_loop(void *arg);

#endif //IP_H