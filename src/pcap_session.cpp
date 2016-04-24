#include <node.h>
#include <pcap.h>

#include "pcap_session.h"

v8::Persistent<v8::Function> PcapSession::constructor;

PcapSession::PcapSession() {
}

PcapSession::~PcapSession() {
}

void PcapSession::Init(v8::Local<v8::Object> exports) {
	v8::Isolate* isolate = exports->GetIsolate();

	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, "PcapSession"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "open", Open);

	constructor.Reset(isolate, tpl->GetFunction());

	exports->Set(v8::String::NewFromUtf8(isolate, "PcapSession"), tpl->GetFunction());
}

void PcapSession::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) {
		// Invoked as constructor: `new PcapSession(...)`
		PcapSession* obj = new PcapSession();
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else {
		// Invoked as plain function `PcapSession(...)`, turn into construct call.
		const int argc = 1;
		v8::Local<v8::Value> argv[argc] = { args[0] };
		v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
		args.GetReturnValue().Set(cons->NewInstance(argc, argv));
	}
}

void PcapSession::Open(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();

	char error_buffer[PCAP_ERRBUF_SIZE];
	char* device_name;

	if (args.Length() == 0) {
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Device name is required")));
		return;
	}

	if (!args[0]->IsString()) {
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Device name must be a string")));
		return;
	}

	pcap_t* session_handle = pcap_create((char *) device_name, error_buffer);

	printf("%s\n", "HELLLLO");
}
