#ifndef PCAP_SESSION
#define PCAP_SESSION

#include <pcap.h>
#include <nan.h>

class PcapSession : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);

  private:
    explicit PcapSession();
    ~PcapSession();

    static Nan::Persistent<v8::Function> constructor;

    static NAN_METHOD(New);
    static NAN_METHOD(Open);

    static void OnPacket(unsigned char *args, const struct pcap_pkthdr *header, const unsigned char *packet);

    pcap_t* pcapSession;
};

#endif
