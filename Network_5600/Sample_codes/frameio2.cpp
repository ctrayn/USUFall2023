#include "frameio2.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <string.h>
#include <netinet/in.h>

//
// return false if the frame is to be lost entirely
//
bool damage_frame(void *vframe, int len)
{
   char *frame = (char *)vframe;
   static int frame_count = 0;
   static int udp_count = 0;

   frame_count = (frame_count + 1) % 37;
   if ( frame_count == 4 )
   {
      fprintf(stderr,"frame lost\n");
      return false;
   }

   // only damage UDP/IP frames
   if ( frame[12] != 8 || frame[13] != 0 || frame[23] != 17 ) return true;

   udp_count = udp_count + 1;
   switch ( udp_count % 47 )
   {
   case 2: // damage IP header
      frame[17] = '~';
      break;
   case 7: // damage UDP header
      frame[37] = '~';
      break;
   case 13: // damage data anywhere
      if ( udp_count % 557 < len ) frame[udp_count % 557] = '~';
      break;
   default:
      return true;
   }
   fprintf(stderr,"udp frame %d damaged.\n",udp_count);
   return true;
}

int frameio::open_net(const char *device)
{
   const int LEN = 80;
   struct sockaddr_ll sll;
   struct ifreq ifreq;

   memset(&sll, 0, sizeof sll);
   sll.sll_family = PF_PACKET;
   sll.sll_protocol = htons (ETH_P_ALL);

   sock = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL));
   if ( sock < 0 ) return sock;
   strcpy (ifreq.ifr_name, device);
   ioctl (sock, SIOCGIFINDEX, &ifreq);
   sll.sll_ifindex = ifreq.ifr_ifindex;
   bind (sock, (struct sockaddr *) &sll, sizeof sll);

   //
   // get the mac address - there must be a better way
   //
   char cmd[LEN];
   snprintf(cmd,LEN,"/sbin/ifconfig | grep %s > .my_mac_address",device);
   system(cmd);
   FILE *f = fopen(".my_mac_address","r");
   if ( f!=NULL )
   {
      char *s = fgets(cmd,LEN,f);
      fclose(f);
      if ( s!=NULL )
      {
         while ( s<cmd+LEN && strncmp(s,"HWaddr ",7) ) s++;
         if ( s != cmd+LEN )
         {
            unsigned int mac[6];
            sscanf(s+7,"%x:%x:%x:%x:%x:%x",mac+0,mac+1,mac+2,mac+3,mac+4,mac+5);
            for ( int i=0; i<6; ++i ) my_mac[i] = mac[i];
            return sock;
         }
      }
   }
   
}
