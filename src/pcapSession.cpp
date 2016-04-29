#include <node.h>
#include <pcap.h>
#include <net/ethernet.h>

#include "pcapSession.h"
#include "callbackInfo.h"
#include "ipAddress.h"
#include "ipHeader.h"
#include "macAddress.h"
#include "ethernetHeader.h"

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
	CallbackInfo callbackInfo;
	callbackInfo.isolate = isolate;
	callbackInfo.callback = v8::Local<v8::Function>::Cast(args[1]);

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

	pcap_loop(session->pcapSession, numPackets, PcapSession::OnPacket, (unsigned char *)&callbackInfo);

	pcap_close(session->pcapSession);

	return;
}

void PcapSession::OnPacket(unsigned char* args, const struct pcap_pkthdr *header, const unsigned char *packet) {
	static int packetCount = 1;
	const int ethernetSize = 14; //ethernet headers are always 14 bytes long

	const struct EthernetHeader *ethernetHeader;
	const struct IpHeader *ipHeader;

	packetCount++;

	ethernetHeader = (struct EthernetHeader*)(packet);
	ipHeader = (struct IpHeader*)(packet + ethernetSize);

	// ---------------------------- //

	// char* destMacAddress = ether_ntoa((ether_addr *)&ethernetHeader->destAddress);
	// char* srcMacAddress = ether_ntoa((ether_addr *)&ethernetHeader->srcAddress);

	//TODO figure out why printing the char* version of these messes things up
	// printf("DEST MAC ADDRESS 1: %s\n", destMacAddress);
	// printf("DEST MAC ADDRESS 2: %s\n", ether_ntoa((ether_addr *)&ethernetHeader->destAddress));
	// printf("SRC MAC ADDRESS 1: %s\n", srcMacAddress);
	// printf("SRC MAC ADDRESS 2: %s\n", ether_ntoa((ether_addr *)&ethernetHeader->srcAddress));

	printf("TYPE : %hu\n", ethernetHeader->type);

	// ---------------------------- //
	// callbackInfo
	// ---------------------------- //

	CallbackInfo* callbackInfo = (CallbackInfo *)(args);
	const int argc = 2;

	v8::Local<v8::Value> argv[argc] = {
		v8::String::NewFromUtf8(callbackInfo->isolate, ether_ntoa((ether_addr *)&ethernetHeader->destAddress)),
		v8::String::NewFromUtf8(callbackInfo->isolate, ether_ntoa((ether_addr *)&ethernetHeader->srcAddress))
	};

	callbackInfo->callback->Call(Null(callbackInfo->isolate), argc, argv);

	return;
}

const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}
