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
    pcap_t* pcap_session;
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Persistent<v8::Function> constructor;
    static void Open(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void On_Packet(unsigned char *args, const struct pcap_pkthdr *header, const unsigned char *packet);
};

#endif
