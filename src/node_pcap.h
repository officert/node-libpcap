#ifndef NODE_PCAP
#define NODE_PCAP

#include <node.h>

#include "pcap_session.h"

namespace node_pcap {
  Pcap_Session* Create_Session();
}

#endif
