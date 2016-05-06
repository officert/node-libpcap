#include <nan.h>
#include <iostream>

#include "pcapSession.h"

void Lookup_Device(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();

	char error_buffer[PCAP_ERRBUF_SIZE];

	char* device_name = pcap_lookupdev(error_buffer);

	args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, device_name));
}

NAN_MODULE_INIT(InitAll) {
	Nan::Set(target, Nan::New("lookupDevice").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Lookup_Device)).ToLocalChecked());

	PcapSession::Init(target);
}

NODE_MODULE(addon, InitAll)
