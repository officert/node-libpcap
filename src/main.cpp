#include <node.h>
#include <iostream>
#include <pcap.h>

#include "pcap_session.h"

void Lookup_Device(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();

	char error_buffer[PCAP_ERRBUF_SIZE];

	char* device_name = pcap_lookupdev(error_buffer);

	args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, device_name));
}

void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
	NODE_SET_METHOD(exports, "lookupDevice", Lookup_Device);

	PcapSession::Init(exports);
}

NODE_MODULE(addon, InitAll)
