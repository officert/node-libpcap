#include <node.h>
#include <pcap.h>

#include "node_pcap.h"

namespace node_pcap {
	using v8::FunctionCallbackInfo;
	using v8::Local;
	using v8::Object;
	using v8::Value;

	void Create_Session(const FunctionCallbackInfo<Value>& args) {
		v8::Isolate* isolate = args.GetIsolate();

	  //pcap_t* session = node_pcap::Create_Session();

		printf("HELLLLO FROM C++\n");

		Local<Object> obj = Object::New(isolate);

	  args.GetReturnValue().Set(obj);
	}

	void InitAll(Local<Object> exports, Local<Object> module) {
	  NODE_SET_METHOD(exports, "createSession", Create_Session);
	}

	NODE_MODULE(addon, InitAll)
}
