#include <node.h>
#include <pcap.h>
#include <vector>
#include <net/ethernet.h>

#include "pcapSession.h"
#include "structs.h"

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
	CallbackInfo callback;
	callback.isolate = isolate;
	callback.callback = v8::Local<v8::Function>::Cast(args[1]);

	char errorBuffer[PCAP_ERRBUF_SIZE];
	int numPackets = 200; //TODO: make this not hardcoded
	v8::String::Utf8Value str(args[0]);
	const char* deviceName = ToCString(str);

	bpf_u_int32 mask; // The netmask of our sniffing device
	bpf_u_int32 net; // The IP of our sniffing device

	PcapSession* session = ObjectWrap::Unwrap<PcapSession>(args.This());

	if (session == NULL)
	{
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Error opening pcap session")));
		return;
	}

	// get network number and mask associated with capture device
	if (pcap_lookupnet(deviceName, &net, &mask, errorBuffer) == -1)
	{
		printf("Couldn't get netmask for device %s: %s\n", deviceName, errorBuffer);

		net = 0;

		mask = 0;
	}

	printf("Device: %s\n", deviceName);
	// printf("Filter expression: %s\n", filter_exp);
	printf("Net: %u\n", net);
	printf("Mask: %u\n", mask);

	const int packetCaptureLength = 65536;

	//TODO: figure out whether to use 1) pcap_create or 2) pcap_open_live

	//1)
	//http://seclists.org/tcpdump/2012/q1/15
	// session->pcapSession = pcap_create(deviceName, errorBuffer);
	//
	// pcap_activate(session->pcapSession);

	//2)
	session->pcapSession = pcap_open_live(deviceName, packetCaptureLength, 1, 1000, errorBuffer);

	//TODO: handle errorBuffer

	pcap_loop(session->pcapSession, numPackets, PcapSession::OnPacket, (unsigned char *)&callback);

	pcap_close(session->pcapSession);

	return;
}

void PcapSession::OnPacket(unsigned char* args, const struct pcap_pkthdr *header, const unsigned char *packet) {
	static int count = 1; //packet counter
	static std::vector<MacAddress> macAddresses;
	const int ethernetSize = 14; //ethernet headers are always 14 bytes long

	const struct EthernetHeader *ethernetHeader; /* The Ethernet header */
	const struct IpHeader *ip_header; /* The IP header */

	count++;

	ethernetHeader = (struct EthernetHeader*)(packet);
	ip_header = (struct IpHeader*)(packet + ethernetSize);

	// ---------------------------- //

	char* srcMacAddress = ether_ntoa((ether_addr *)&ethernetHeader->srcAddress);
	char* destMacAddress = ether_ntoa((ether_addr *)&ethernetHeader->destAddress);

	// ---------------------------- //
	// callback
	// ---------------------------- //

	CallbackInfo* callback = (CallbackInfo *)(args);
	const int argc = 1;

	v8::Local<v8::Value> argv[2] = {
		v8::String::NewFromUtf8(callback->isolate, srcMacAddress),
		v8::String::NewFromUtf8(callback->isolate, destMacAddress)
	};

	callback->callback->Call(Null(callback->isolate), argc, argv);

	return;
}

const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}
