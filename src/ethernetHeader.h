#ifndef ETHERNET_HEADER
#define ETHERNET_HEADER

#include "macAddress.h"

struct EthernetHeader {
  struct MacAddress destAddress;
  struct MacAddress srcAddress;
  unsigned short type;
};

#endif
