#include <node.h>

#include "pcap_session.h"

namespace node_libpcap {
	using v8::FunctionCallbackInfo;
	// using v8::Isolate;
	using v8::Local;
	using v8::Object;
	// using v8::String;
	using v8::Value;

	void Create_Session(const FunctionCallbackInfo<Value>& args) {
	  //node_pcap::Pcap_Session::Create_Session(args);

		// Local<Number> num = Number::New(isolate, value);
	  // args.GetReturnValue().Set(num);

		printf("HELLLLO FROM C++\n");
	}

	void InitAll(Local<Object> exports, Local<Object> module) {
	  // node_pcap::Pcap_Session::Init(exports->GetIsolate());

	  NODE_SET_METHOD(exports, "createSession", Create_Session);
	}

	NODE_MODULE(addon, InitAll)
}
