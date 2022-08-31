#include "util.h"
#include <sys/times.h>

//
// member function message_queue::recv()
//
// Receive a message from this message queue, possibly blocking until
// one is available. If the message length is larger than the buffer
// provided, the message is truncated and the remainder of the message
// is lost.
//
// return value: recv() returns the actual number of bytes in the buffer.
//
// Note: Only one thread should call recv for a particular message queue
//
int message_queue::recv(event_kind *event, void *buf, int max_len)
{
   int len, header;
   //
   // first, read the header and extract the event kind and message length
   //
   read(fd[0],&header,sizeof(int));
   *event = event_kind(header>>24);
   len = header & 0xffffff;
   //
   // if len < max_len, we don't need max_len any more, so we make it
   // the actual returned message size, i.e. min(len,max_len)
   //
   if ( max_len > len ) max_len = len;
   read(fd[0],buf,max_len);
   //
   // usually, we're finished at this point, but if the message buffer
   // wasn't long enough, we need to discard the rest of the bytes in
   // this message (yes, I know that 1-byte reads suck, but if you don't
   // provide an adequate buffer, you deserve it.)
   //
   while ( len-- > max_len ) read(fd[0],&header,1);
   //
   // max_len is still the number of bytes in the buffer we're returning
   //
   return max_len;
}

//
// member function message_queue::send()
//
// Send a message to this message queue. Message length may not
// exceed 0xffffff bytes. If it does, a 0-length message is sent
// instead. The event kind must fit in 8 bits.
//
void message_queue::send(event_kind event, void *buf, int len)
{
   //
   // if message length is out of bounds (plus or minus) change it to 0
   //
   if ( (unsigned)len > 0xffffff ) len = 0;
   //
   // construct header
   //
   int header = event<<24 | len;
   //
   // We can't let another process come in between the write of the
   // header and the write of the actual message, so we protect that
   // operation with a semaphore.
   // 
   pthread_mutex_lock(&sem);
   write(fd[1],&header,sizeof(int));
   write(fd[1],buf,len);
   pthread_mutex_unlock(&sem);
}

//
// we could be cleaver about this timer, but we're not. There is one,
// common linked list containing timer records. Each record tells us
// how many (100msec) ticks the timer has left, the number that will
// be delivered in the 4-byte message, and to which queue the message
// will be sent. 
//
// Every 100msec, we simply run through each record in the list,
// decrementing the time left. If it becomes zero, we send the message
// and remove the record from the list. What could be simpler than that?
//
struct timer_header
{
   struct timer_header *next; // next record in the linked list
   message_queue *queue;      // message queue for message delivery
   int ticks_to_go;           // number of ticks until timer fires
   int timer_nbr;             // 4-byte message to send (kind = TIMER)
};

//
// Common linked list
//
static timer_header *timer_list = NULL;
//
// semaphore to protect the timer list
//
static pthread_mutex_t timer_mutex = PTHREAD_MUTEX_INITIALIZER;

#define TICK_NANOSECS 100000000 // 100 msec

//
// This thread is started whenever we start a timer. Once started, it never
// dies (even if all timers expire). It wakes up every 100msec and checks
// (and updates) the timer_list.
//
void *timer_loop(void *arg)
{
   for ( ; ; )
   {
      //
      // there's no handy way to call a function every 100msec, except
      // by sleeping that long and executing the function each time the
      // thread re-awakens. Ergo, we wait 100msec.
      //
      struct timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = TICK_NANOSECS;
      nanosleep(&ts,NULL);
      //
      // Now, to check the timer list. Naturally, we'll want to protect
      // this part of the code with a semaphore.
      //
      pthread_mutex_lock(&timer_mutex);
      timer_header *ptr, **p = &timer_list;
      while ( (ptr = *p) != NULL )
      {
         if ( --ptr->ticks_to_go <= 0 )
         {
            //
            // timer expired, send the message and deallocate the record.
            //
            ptr->queue->send(TIMER, &ptr->timer_nbr, sizeof(int));
            *p = ptr->next;
            delete ptr;
         }
         else
            p = &ptr->next;
      }
      pthread_mutex_unlock(&timer_mutex);
   }
}

//
// This function can be used to schedule a TIMER message for this
// queue to be delivered 100*ticks milliseconds in the future. The
// message consists of a 4-byte (integer) timer number (with event
// kind equal to TIMER, of course).
//
void message_queue::timer(int ticks, int timer_nbr)
{
   static bool timer_running = false;
   static pthread_t timer_thread;

   //
   // if the time is already past, just send the message now
   //
   if ( ticks <= 0 )
   {
      send(TIMER, &timer_nbr, sizeof(int));
      return;
   }

   //
   // Ok, we have a legitimate request. We need to protect the
   // following code so only one thread is messing with the list
   //
   pthread_mutex_lock(&timer_mutex);
   //
   // if no one has started the timer thread, start it now
   //
   if ( !timer_running )
   {
      pthread_create(&timer_thread, NULL, timer_loop, NULL);
      timer_running = true;
   }
   //
   // allocate a new record and fill it in
   //
   timer_header *ptr = new timer_header;
   ptr->queue = this;
   ptr->ticks_to_go = ticks;
   ptr->timer_nbr = timer_nbr;
   //
   // now stuff it in the list
   //
   ptr->next = timer_list;
   timer_list = ptr;
   pthread_mutex_unlock(&timer_mutex);
}
   
//
// This function returns the time since some arbitrary point using
// some arbitrary time scale. Hell, this may as well be a random
// number generator if not for the fact that you can call it twice and
// use age_in_ms() to compute the elapsed time in milliseconds.
//
long timestamp()
{
   struct tms junk;
   return times(&junk); // returns a tick count
}

//
// This handy function can be used to compute the elapsed time between two
// calls to time_stamp(). This can be useful for deciding when to discard an
// entry in the ARP cache (or which entry you have to jettison to make room)
//
unsigned age_in_ms(long now, long then)
{
   return unsigned(now - then) * 1000 / sysconf(_SC_CLK_TCK);
}

