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

void Create_Session(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();

	std::string device_name;

	if(args.Length() > 0)
	{
		if(!args[0]->IsString())
		{
			// Throw an Error that is passed back to JavaScript
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "First argument must be a string")));
			return;
		}

		v8::String::Utf8Value param1(args[0]->ToString());

		device_name = std::string(*param1);
	}

	//pcap_t* session = node_pcap::Create_Session();

	v8::Local<v8::Object> obj = v8::Object::New(isolate);

	args.GetReturnValue().Set(obj);
}

void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
	NODE_SET_METHOD(exports, "lookupDevice", Lookup_Device);
	NODE_SET_METHOD(exports, "createSession", Create_Session);
}

NODE_MODULE(addon, InitAll)
