#include <node.h>
#include <pcap.h>

#include "pcap_session.h"

namespace node_libpcap {
	using v8::FunctionCallbackInfo;
	using v8::Local;
	using v8::Object;
	using v8::Value;

	void Create_Session(const FunctionCallbackInfo<Value>& args) {
		v8::Isolate* isolate = args.GetIsolate();

	  //node_pcap::Pcap_Session::Create_Session(args);

		printf("HELLLLO FROM C++\n");

		Local<Object> obj = Object::New(isolate);

	  args.GetReturnValue().Set(obj);
	}

	void InitAll(Local<Object> exports, Local<Object> module) {
	  NODE_SET_METHOD(exports, "createSession", Create_Session);
	}

	NODE_MODULE(addon, InitAll)
}
