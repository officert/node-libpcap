#ifndef PCAP_SESSION
#define PCAP_SESSION

#include <pcap.h>
#include <node.h>
#include <node_object_wrap.h>

namespace node_pcap {
  class Pcap_Session : node::ObjectWrap {
    public:
      static void Init(v8::Local<v8::Object> exports);

    private:
      static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
      static v8::Persistent<v8::Function> constructor;
  };
}

#endif
