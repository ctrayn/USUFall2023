//
// int chksum(octet *s, int bytes, int initial)
//
// compute the one's complement checksum used in most IP protocols.
//
// parameters:
//   s:       address of block (string) to be checksummed
//   bytes:   number of bytes to checksum (an odd number of bytes is allowed)
//   initial: checksum from a previous block (this allows a checksum to be
//            computed for a set of non-contiguous blocks)
// return value:
//   one's complement checksum
//
// Note: the checksum this function provides must usually
// be complemented before it is placed in the packet:
//
//    ip->header_checksum[0] = 0; // initial checksum value must be 0
//    ip->header_checksum[1] = 0;
//    int sum = chksum((octet *)ip,20,0);
//    ip->header_checksum[0] = ~sum >> 8;
//    ip->header_checksum[1] = ~sum & 0xff;
//   
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
