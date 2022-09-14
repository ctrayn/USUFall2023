//**********************************************************************
//
// frameio.h
//
// frameio.h and frameio.cpp provide convenient access to the ethernet
// using raw sockets. This provides the means to read and write frames
// directly to and from the ethernet interface.
//
// Before the frameio object can be used, you must specify which interface
// you are using. This is done via the member function open_net(). For
// example, if you wish to communicate using inteface "eth1", you might
// write the code:
//
// frameio net;
//
// main(int argc, char *argv[])
// {
//    net.open_net("eth1");
//
// After the net has been opened, the interface's 6-byte MAC address can be
// obtained by calling get_mac(). Note that this function gives you a pointer
// to the object's internal storage - it is not recommended that you change
// the memory referenced by get_mac.
//
// To read from the ethernet interface, call net.recv_frame with a buffer
// address (and maximum size). The function waits for the next frame (unless
// one is already queued up) and copies it into the buffer (except for the
// CRC, which is handled by the interface). The function returns the actual
// number of bytes in the frame, but beware, it may not match the number of
// bytes in the logical frame (although it better not be smaller). Usually,
// you will want to dedicate a thread to reading the frame from the network
// and dispatching them to the protocol stack(s).
//
// To write a frame to the interface, call net.send_frame with the address
// and size of the frame to send. Again, leave off the CRC - the interface
// handles that. send_frame returns the number of bytes actually written,
// but you can usually ignore that.
//
//**********************************************************************
#ifndef FRAMEIO_H
#define FRAMEIO_H

#include <sys/socket.h>
#include <unistd.h>

//
// it is a pain to declare unsigned chars everywhere, so we define
// "octet" to be unsigned char
//
typedef unsigned char octet;

//
// class frameio - see descripion in the file header.
//
class frameio
{
public:
   //
   // send a frame to the open interface, return number of bytes sent
   //
   int send_frame(void *frame, int len)
   {
      return write(sock,frame,len);
   }
   //
   // block, waiting for a frame. When it arrives, copy it into the buffer.
   // return the number of bytes in the wire packet
   //
   int recv_frame(void *frame, int max_len)
   {
      return recvfrom(sock,frame,max_len,0,NULL,NULL);
   }
   //
   // open a socket on the specified interface and load my_mac
   //
   int open_net(const char *device); // e.g. "eth0"
   //
   // return this interface's MAC address
   //
   const octet *get_mac()
   {
      return my_mac;
   }
   //
   // Constructor
   //
   frameio()
   {
      // make sure sock is not valid
      sock = -1;
   }
   //
   // Destructor
   //
   ~frameio()
   {
      // if socket has been opened, close it
      if ( sock >= 0 ) close(sock);
      sock = -1;
   }
private:
   octet my_mac[6]; // this interface's 6-byte MAC address
   int sock;        // socket descriptor
};

#endif
