#ifndef FRAMEIO_H
#define FRAMEIO_H

#include <sys/socket.h>
#include <unistd.h>

bool damage_frame(void *frame, int len);

typedef unsigned char octet;

class frameio
{
public:
   int send_frame(void *frame, int len)
   {
      return damage_frame(frame,len)? write(sock,frame,len) : len;
   }
   int recv_frame(void *frame, int max_len)
   {
      for ( ; ; )
      {
         int len = recvfrom(sock,frame,max_len,0,NULL,NULL);
         if ( damage_frame(frame,len) ) return len;
      }
   }
   int open_net(const char *device); // e.g. "eth0"
   const octet *get_mac()
   {
      return my_mac;
   }
   frameio()
   {
      sock = -1;
   }
   ~frameio()
   {
      if ( sock >= 0 ) close(sock);
      sock = -1;
   }
private:
   octet my_mac[6];
   int sock;
};

#endif
