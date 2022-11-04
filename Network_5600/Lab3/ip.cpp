#include "ip.h"
using namespace std;

extern frameio net;
extern message_queue ip_queue;

string get_ip_from_user() {
    cout << "Please enter a target IP address (X.X.X.X): ";
    string ip_string;
    cin >> ip_string;
    return ip_string;
};

void IPAddress::string_to_ip(string ip_string) {
    if (!validate_ip(ip_string)) {
        cout << "Invalid IP: " << ip_string << endl;
        for (int i = 0; i < 4; i++) {
            address[i] = 0;
        }
    }
    else {
        string input = ip_string;
            cout << "Valid IP: " << ip_string << endl;
        for (int i = 0; i < 4; i++) {
            address[i] = stol(input.substr(0, input.find('.')));
            input = input.substr(input.find('.') + 1);
        }
    }
}

bool IPAddress::validate_ip(string ip_string) {
    int i = -1;
    char current;
    string compile = "";
    int count = 0;
    int compile_num;

    if (ip_string.size() > 16) {
        cout << "IP too big " << ip_string.size() << endl;
        return false;   //IP too long
    }
    else if (ip_string.size() < 7) {
        cout << "IP too small " << ip_string.size() << endl;
        return false;
    }

    while (current != '\0') {
        i++;
        current = ip_string[i];

        if (isdigit(current)) {
            compile += current;
        }
        else if (current == '.' || current == '\0') {
            //Check if valid
            compile_num = stol(compile);
            if (compile_num < 0 || compile_num >= 256) {
                return false;
            }
            count++;
            compile = "";
        }
        else {
            return false;   //Not a number or '.' so it's not valid IP
        }
    }
    if (count != 4) {
        return false;
    }
    return true;
}

string IPAddress::get_string() {
    string out = "";
    for (int i = 0; i < 3; i++) {
        out += to_string(address[i]);
        out += '.';
    }
    out += to_string(address[3]);
    return out;
}

void handle_ICMP(octet buf[1500]) {
    //Is ICMP
    IPAddress source_ip = IPAddress(buf[IP_ADDRESS_SRC_START], buf[IP_ADDRESS_SRC_START + 1], buf[IP_ADDRESS_SRC_START + 2], buf[IP_ADDRESS_SRC_START + 3]);
    // IPAddress dest_ip = IPAddress(buf[IP_ADDRESS_DEST_START], buf[IP_ADDRESS_DEST_START + 1], buf[IP_ADDRESS_DEST_START + 2], buf[IP_ADDRESS_DEST_START + 3]);

    if (buf[ICMP_TYPE] == ICMP_REQUEST) {
        cout << "Saw ICMP Request from " << source_ip.get_string() << endl;
    }
    else if(buf[ICMP_TYPE] == ICMP_REPLY) {
        cout << "Saw ICMP Reply from " << source_ip.get_string() << endl;
    }
}

int IPAddress::to_int() {
    return (address[0] << 24) | (address[1] << 16) | (address[2] << 8) | (address[3]);
}

bool IPAddress::on_same_subnet(IPAddress other, int mask_bytes) {
    int one = this->to_int() >> (IP_LENGTH - mask_bytes);
    int two = other.to_int() >> (IP_LENGTH - mask_bytes);

    cout << "Comparing " << hex << one << " with " << two << dec << endl;

    return one == two;
}

/**
 * @param s octet array pointer
 * @param bytes number of bytes to do checksum
 * @param initial the previous checksum if fragmented, usually will be 0
 */
int chksum(octet *s, int bytes, int initial)
{
   long sum = initial;
   int i;
   for ( i=0; i<bytes-1; i+=2 )
   {
      sum += s[i]*256 + s[i+1];
   }
   //
   // handle the odd byte
   //
   if ( i < bytes ) sum += s[i]*256;
   //
   // wrap carries back into sum
   //
   while ( sum > 0xffff ) sum = (sum & 0xffff) + (sum >> 16);
   return sum;
}

//
// Toy function to print something interesting when an IP frame arrives
//
void *ip_protocol_loop(void *arg)
{
    octet buf[1500];
    event_kind event;
    int timer_no = 1;
    IPAddress dest_ip;
    IPAddress src_ip;
    IPAddress local_ip = IPAddress(get_local_ip());
    cout << "Local IP:   " << local_ip.get_string() << endl;
    IPAddress gate_ip = IPAddress(get_gateway_ip().ip);
    cout << "Gateway IP: " << gate_ip.get_string() << endl;
    ip_oct submask_struct = get_subnet_mask();
    IPAddress submask = IPAddress(submask_struct.ip);
    submask.num_bytes = submask_struct.num_bytes;
    cout << "Submask IP: " << submask.get_string() << endl;

    IPAddress target_ip = IPAddress(get_ip_from_user());
    cout << "Target IP:  " << target_ip.get_string() << endl;

    //Get Mac address

    // for fun, fire a timer each time we get a frame
    while ( 1 )
    {
        ip_queue.recv(&event, buf, sizeof(buf));
        if ( event != TIMER ) {
            //Check if is ICMP
            dest_ip = IPAddress(buf[IP_ADDRESS_DEST_START], buf[IP_ADDRESS_DEST_START + 1], buf[IP_ADDRESS_DEST_START + 2], buf[IP_ADDRESS_DEST_START + 3]);
            src_ip  = IPAddress(buf[IP_ADDRESS_SRC_START],  buf[IP_ADDRESS_SRC_START + 1],  buf[IP_ADDRESS_SRC_START + 2],  buf[IP_ADDRESS_SRC_START + 3]);
            if(local_ip == dest_ip || local_ip == src_ip) {
                //Is my IP
                // cout << "Is my IP: " << dest_ip.get_string() << endl;
                if (buf[IP_PROTOCOL] == ICMP_PROTOCOL_NUM) {
                    handle_ICMP(buf);
                }
            }
            // printf("got an IP frame from %d.%d.%d.%d, queued timer %d\n",
            //         buf[12],buf[13],buf[14],buf[15],timer_no);
            // cout << "Saw IP: " << dest_ip.get_string() << endl;
            ip_queue.timer(10,timer_no);
            timer_no++;
        }
        else
        {
            // printf("timer %d fired\n",*(int *)buf);
        }
    }
}