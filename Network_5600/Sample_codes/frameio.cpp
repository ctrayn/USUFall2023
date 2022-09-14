#include "frameio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <string.h>
#include <netinet/in.h>

//
// Open a raw socket on the interface and get the MAC address
//
int frameio::open_net(const char *device)
{
   const int LEN = 80;
   struct sockaddr_ll sll;
   struct ifreq ifreq;

   //
   // set up the link-layer socket address
   //
   memset(&sll, 0, sizeof sll);
   sll.sll_family = PF_PACKET;
   sll.sll_protocol = htons (ETH_P_ALL);

   //
   // open the socket, tell the OS we want all protocols
   //
   sock = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL));
   if ( sock < 0 ) return sock;

   //
   // if all you want to do is read, we could stop here. But to
   // write a frame, we have to bind this socket to an interface
   // and to do that, we need its interface number (small int)
   //
   strcpy (ifreq.ifr_name, device);
   ioctl (sock, SIOCGIFINDEX, &ifreq);
   sll.sll_ifindex = ifreq.ifr_ifindex;

   //
   // now let us bind...
   //
   bind (sock, (struct sockaddr *) &sll, sizeof sll);

   //
   // get the mac address 
   //
   struct ifreq ifr;
   strncpy(&ifr.ifr_name[0], device, IFNAMSIZ);
   if (ioctl(sock, SIOCGIFHWADDR, &ifr) >= 0) 
   {
      memcpy(my_mac, &ifr.ifr_hwaddr.sa_data, 6);
      return sock;
   }
   
   //
   // could have failed three different ways, but failed nontheless...
   //
   close(sock);
   return sock = -1;
}
