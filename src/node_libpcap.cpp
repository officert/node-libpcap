#include <node.h>

#include "node_pcap.h"

namespace node_libpcap {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Create_Session(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

  node_pcap::Pcap_Session* session = node_pcap::Create_Session();

	// Local<Object> obj = Object::New(isolate, session);
  //
	// args.GetReturnValue().Set(obj);
}

void init(Local<Object> exports) {
	NODE_SET_METHOD(exports, "createSession", Create_Session);
}

NODE_MODULE(addon, init)

}
