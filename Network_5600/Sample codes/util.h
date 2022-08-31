//**********************************************************************
//
// util.h
//
// util.h and util.cpp provide utilities that may be of use when writing
// network protocol stacks.
//
//**********************************************************************

#ifndef UTIL_H
#define UTIL_H

#include <pthread.h>
#include <unistd.h>

//
// Message queues can be used to communicate from one thread to another
// A message consists of a 4-byte packed integer header and a variable
// length message. The header is an integer in native endian, whose
// most significant byte contains a message type. Currently, only PACKET
// and TIMER are defined. Feel free to add more. The other three bytes
// of header are message length.
//
typedef enum { PACKET, TIMER } event_kind;

class message_queue
{
public:
   //
   // Constructor
   //
   message_queue()
   {
      pipe(fd);
      pthread_mutex_init(&sem,NULL);
   }
   //
   // Destructor
   //
   ~message_queue()
   {
      pthread_mutex_destroy(&sem);
      close(fd[0]);
      close(fd[1]);
   }
   //
   // Send a message to this message queue. Message length may not
   // exceed 0xffffff bytes. The event kind must fit in 8 bits.
   //
   void send(event_kind event, void *buf, int len);
   //
   // Receive a message from this message queue, possibly blocking until
   // one is available. If the message length is larger than the buffer
   // provided, the message is truncated and the remainder of the message
   // is lost. recv() returns the actual number of bytes in the buffer.
   //
   // Note: Only one thread should call recv for a particular message queue
   //
   int recv(event_kind *event, void *buf, int max_len);
   //
   // This function can be used to schedule a TIMER message for this
   // queue to be delivered 100*ticks milliseconds in the future. The
   // message consists of a 4-byte (integer) timer number (with event
   // kind equal to TIMER, of course).
   //
   void timer(int ticks, int timer_number);
   
private:
   pthread_mutex_t sem;
   int fd[2];
};

//
// This function returns the time since some arbitrary point using
// some arbitrary time scale. Hell, this may as well be a random
// number generator if not for the fact that you can call it twice and
// use age_in_ms() to compute the elapsed time in milliseconds.
//
long timestamp();

//
// This handy function can be used to compute the elapsed time between two
// calls to time_stamp(). This can be useful for deciding when to discard an
// entry in the ARP cache (or which entry you have to jettison to make room)
// 
unsigned age_in_ms(long now, long then);

#endif
