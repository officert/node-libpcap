#include <node.h>
#include <pcap.h>
#include <string>
#include <net/ethernet.h>

#include "pcapSession.h"
#include "callbackInfo.h"
#include "ipAddress.h"
#include "ipHeader.h"
#include "macAddress.h"
#include "ethernetHeader.h"

char* formatMacAddress(struct MacAddress macAddress);

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
	const char* deviceName = *str;

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

	callbackInfo.deviceName = deviceName;

	// printf("Device: %s\n", callbackInfo.deviceName.c_str());
	// // printf("Filter expression: %s\n", filter_exp);
	// printf("Net: %u\n", net);
	// printf("Mask: %u\n", mask);

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

	printf("TYPE : %hu\n", ethernetHeader->type);

	// ---------------------------- //
	// callbackInfo
	// ---------------------------- //

	CallbackInfo* callbackInfo = (CallbackInfo *)(args);

	v8::Local<v8::Object> obj = v8::Object::New(callbackInfo->isolate);

	obj->Set(v8::String::NewFromUtf8(callbackInfo->isolate, "deviceName"), v8::String::NewFromUtf8(callbackInfo->isolate, callbackInfo->deviceName.c_str()));
	obj->Set(v8::String::NewFromUtf8(callbackInfo->isolate, "destAddress"), v8::String::NewFromUtf8(callbackInfo->isolate, formatMacAddress(ethernetHeader->destAddress)));
	obj->Set(v8::String::NewFromUtf8(callbackInfo->isolate, "srcAddress"), v8::String::NewFromUtf8(callbackInfo->isolate, formatMacAddress(ethernetHeader->srcAddress)));

	const int argc = 1;

	v8::Local<v8::Value> argv[argc] = {
		obj
	};

	callbackInfo->callback->Call(Null(callbackInfo->isolate), argc, argv);

	return;
}

char* formatMacAddress(struct MacAddress macAddress) {
	static char buffer[18];

	sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x",
	        macAddress.byte1, macAddress.byte2,
	        macAddress.byte3, macAddress.byte4,
	        macAddress.byte5, macAddress.byte6);

	return buffer;
}
