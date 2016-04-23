#ifndef PCAP_SESSION
#define PCAP_SESSION

#include <pcap.h>
#include <node.h>
#include <node_object_wrap.h>

class PcapSession : public node::ObjectWrap {
  public:
    static void Init(v8::Local<v8::Object> exports);

  private:
    explicit PcapSession();
    ~PcapSession();

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Persistent<v8::Function> constructor;
};

#endif
