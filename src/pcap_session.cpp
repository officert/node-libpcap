#include <node.h>
#include <pcap.h>

#include "pcap_session.h"

const char* ToCString(const v8::String::Utf8Value& value);

v8::Persistent<v8::Function> PcapSession::constructor;

PcapSession::PcapSession() {
}

PcapSession::~PcapSession() {
}

void PcapSession::Init(v8::Local<v8::Object> exports) {
	v8::Isolate* isolate = exports->GetIsolate();

	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, "PcapSession"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "open", Open);

	constructor.Reset(isolate, tpl->GetFunction());

	exports->Set(v8::String::NewFromUtf8(isolate, "PcapSession"), tpl->GetFunction());
}

void PcapSession::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) {
		// Invoked as constructor: `new PcapSession(...)`
		PcapSession* obj = new PcapSession();
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else {
		// Invoked as plain function `PcapSession(...)`, turn into construct call.
		const int argc = 1;
		v8::Local<v8::Value> argv[argc] = { args[0] };
		v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
		args.GetReturnValue().Set(cons->NewInstance(argc, argv));
	}
}

void PcapSession::Open(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();

	if (args.Length() == 0) {
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Device name is required")));
		return;
	}

	if (!args[0]->IsString()) {
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Device name must be a string")));
		return;
	}

  //callback function for packets recieved
  v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[1]);

	char error_buffer[PCAP_ERRBUF_SIZE];
	int num_packets = 200; //TODO: make this not hardcoded
  v8::String::Utf8Value str(args[0]);
	const char* device_name = ToCString(str);

	bpf_u_int32 mask; // The netmask of our sniffing device
	bpf_u_int32 net; // The IP of our sniffing device

	PcapSession* session = ObjectWrap::Unwrap<PcapSession>(args.This());

	if (session == NULL)
	{
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Error opening pcap session")));
		return;
	}

	// get network number and mask associated with capture device
	if (pcap_lookupnet(device_name, &net, &mask, error_buffer) == -1)
	{
		printf("Couldn't get netmask for device %s: %s\n", device_name, error_buffer);

		net = 0;

		mask = 0;
	}

	printf("Device: %s\n", device_name);
	// printf("Filter expression: %s\n", filter_exp);
	printf("Net: %u\n", net);
	printf("Mask: %u\n", mask);

  const int packet_capture_length = 65536;

  //TODO: figure out whether to use 1) pcap_create or 2) pcap_open_live

  //1)
  //http://seclists.org/tcpdump/2012/q1/15
	// session->pcap_session = pcap_create(device_name, error_buffer);
  //
  // pcap_activate(session->pcap_session);

  //2)
  session->pcap_session = pcap_open_live(device_name, packet_capture_length, 1, 1000, error_buffer);

	printf("%s\n", "PCAP LOOOOOPPPP");

	//TODO: handle error_buffer

	pcap_loop(session->pcap_session, num_packets, PcapSession::On_Packet, NULL);

	pcap_close(session->pcap_session);

	return;
}

void PcapSession::On_Packet(unsigned char *args, const struct pcap_pkthdr *header, const unsigned char *packet) {
	printf("%s\n", "HEREEEEEE");

	// PcapSession* session = (PcapSession *)args;

  // v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args);
}

const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}
