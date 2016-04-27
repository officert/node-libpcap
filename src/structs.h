#ifndef STRUCTS_H
#define STRUCTS_H

#include <arpa/inet.h>
#include <node.h>

/* 4 bytes IP address */
struct IpAddress {
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
};

/* 6 bytes MAC address */
struct MacAddress {
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
    u_char byte5;
    u_char byte6;
};

/* Ethernet header */
struct EthernetHeader {
  struct MacAddress dest_address;
  struct MacAddress src_address;
	// u_char destination_host[ETHER_ADDR_LEN]; /* Destination host address */
	// u_char source_host[ETHER_ADDR_LEN]; /* Source host address */
	//u_short type; /* IP? ARP? RARP? etc */
};

/* IP header */
struct IpHeader {
	u_char ip_vhl;		/* version << 4 | header length >> 2 */
	u_char ip_tos;		/* type of service */
	u_short ip_len;		/* total length */
	u_short ip_id;		/* identification */
	u_short ip_off;		/* fragment offset field */
	u_char ip_ttl;		/* time to live */
	u_char ip_p;		/* protocol */
	u_short ip_sum;		/* checksum */
	struct IpAddress src_address; // source address
	struct IpAddress dest_address; // dest address
};

struct CallbackInfo {
	v8::Local<v8::Function> callback;
	v8::Isolate* isolate;
};

#endif
