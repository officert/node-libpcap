#ifndef NODE_PCAP
#define NODE_PCAP

#include <pcap.h>

namespace node_pcap {
  pcap_t* Create_Session();
}

#endif
