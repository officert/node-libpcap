#include <node.h>

#include "pcap_Session.h"

namespace node_pcap {
  using v8::Function;
  using v8::FunctionCallbackInfo;
  using v8::FunctionTemplate;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::Persistent;
  using v8::String;
  using v8::Value;

  Persistent<Function> Pcap_Session::constructor;

  void Pcap_Session::Init(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();

    // // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);

    tpl->SetClassName(String::NewFromUtf8(isolate, "Pcap_Session"));
    // tpl->InstanceTemplate()->SetInternalFieldCount(1);

    constructor.Reset(isolate, tpl->GetFunction());

    exports->Set(String::NewFromUtf8(isolate, "Pcap_Session"), tpl->GetFunction());
  }

  void Pcap_Session::New(const FunctionCallbackInfo<Value>& args) {
    // Isolate* isolate = args.GetIsolate();

    // if (args.IsConstructCall()) {
    //   // Invoked as constructor: `new Pcap_Session(...)`
    //   double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    //   Pcap_Session* obj = new Pcap_Session();
    //   obj->Wrap(args.This());
    //   args.GetReturnValue().Set(args.This());
    // } else {
    //   // Invoked as plain function `Pcap_Session(...)`, turn into construct call.
    //   const int argc = 1;
    //   Local<Value> argv[argc] = { args[0] };
    //   Local<Function> cons = Local<Function>::New(isolate, constructor);
    //   args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    // }
  }
}
