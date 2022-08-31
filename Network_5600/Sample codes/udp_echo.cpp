#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>

//
// number of 8-byte real numbers to transfer
//
#define POINTS 2000

//
// request and response buffers
//
double request[POINTS];
double response[POINTS];

//
// Test the echo protocol of another computer
//
int main(int argc, char *argv[])
{
   //
   // check the validity of the arguments
   //
   if ( argc != 2 )
   {
      printf("--usage: udp_echo <ip address>\n");
      return 1;
   }

   sockaddr_in sock_addr;
   sock_addr.sin_family = AF_INET;
   sock_addr.sin_port = htons(7);
   if ( !inet_pton(AF_INET,argv[1],&sock_addr.sin_addr.s_addr) )
   {
      printf("--error: invalid IP address\n");
      return 1;
   }

   int sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
   if ( sock < 0 )
   {
      printf("--error: unable to open local socket\n");
      return 1;
   }

   //
   // initialize the buffer
   //
   for ( int i=0; i<POINTS; ++i ) request[i] = 1.0 / (i+1);

   fd_set rset;
   FD_ZERO(&rset);
   for ( ; ; )
   {
      sendto(sock,request,sizeof(request),0,
             (struct sockaddr *)&sock_addr,sizeof(sock_addr));
      bool echoed = false;
      FD_SET(sock,&rset);
      do
      {
         struct timeval timeout;
         timeout.tv_sec = 1;
         timeout.tv_usec = 0;
         //
         // wait for a response or 1 second timeout whichever is first
         //
         select(sock+1,&rset,NULL,NULL,&timeout);
         if ( !FD_ISSET(sock,&rset) ) break;
         int n = recv(sock,response,sizeof(response),MSG_WAITALL);
         if ( n != sizeof(request) )
            printf("--echoed response is the wrong size (%d)!\n",n);
         else if ( memcmp(request,response,sizeof(request))!=0 )
            printf("--echoed response does not match request!\n");
         else
            printf("  echo received correctly.\n");
         echoed = true;
      } while ( 1 );
      if ( !echoed ) printf("--echo request timed out!\n");
   }
}

