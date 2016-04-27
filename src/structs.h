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

struct CallbackInfo {
	v8::Local<v8::Function> callback;
	v8::Isolate* isolate;
};

#endif
