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

using namespace v8;

char* formatMacAddress(struct MacAddress macAddress);

Persistent<Function> PcapSession::constructor;

PcapSession::PcapSession() {
}

PcapSession::~PcapSession() {
}

void PcapSession::Init(Local<Object> exports) {
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "PcapSession"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "open", Open);

	constructor.Reset(isolate, tpl->GetFunction());

	exports->Set(String::NewFromUtf8(isolate, "PcapSession"), tpl->GetFunction());
}

void PcapSession::New(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) {
		// Invoked as constructor: `new PcapSession(...)`
		PcapSession* obj = new PcapSession();
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else {
		// Invoked as plain function `PcapSession(...)`, turn into construct call.
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		args.GetReturnValue().Set(cons->NewInstance(argc, argv));
	}
}

void PcapSession::Open(const FunctionCallbackInfo<Value>& args) {

	// javascript function signature:
	//
	// /**
	//  *	@param {String} deviceName
	//  * @param {Object} options
	//  * @param {Number} options.packetCount - number of packets to capture
	//  */
	// function(deviceName, options) {}

	Isolate* isolate = args.GetIsolate();

	if (args.Length() == 0) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Device name is required")));
		return;
	}

	if (!args[0]->IsString()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Device name must be a string")));
		return;
	}

	if (!args[1]->IsObject()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Options must be an object")));
		return;
	}

	//get options arg and pull out properties
	Local<Object> options = args[1]->ToObject();

	Local<Value> packetCount = options->Get(String::NewFromUtf8(isolate, "packetCount"));
	int numPackets = packetCount->NumberValue();

	//callback function for packets recieved
	CallbackInfo callbackInfo;
	callbackInfo.isolate = isolate;
	callbackInfo.callback = Local<Function>::Cast(args[2]);

	char errorBuffer[PCAP_ERRBUF_SIZE];
	String::Utf8Value str(args[0]);
	const char* deviceName = *str;

	bpf_u_int32 mask; // The netmask of our sniffing device
	bpf_u_int32 net; // The IP of our sniffing device

	PcapSession* session = ObjectWrap::Unwrap<PcapSession>(args.This());

	if (session == NULL)
	{
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Error opening pcap session")));
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

	CallbackInfo* callbackInfo = (CallbackInfo *)(args);

	Local<Object> obj = Object::New(callbackInfo->isolate);

	obj->Set(String::NewFromUtf8(callbackInfo->isolate, "deviceName"), String::NewFromUtf8(callbackInfo->isolate, callbackInfo->deviceName.c_str()));
	obj->Set(String::NewFromUtf8(callbackInfo->isolate, "destMacAddress"), String::NewFromUtf8(callbackInfo->isolate, formatMacAddress(ethernetHeader->destAddress)));
	obj->Set(String::NewFromUtf8(callbackInfo->isolate, "srcMacAddress"), String::NewFromUtf8(callbackInfo->isolate, formatMacAddress(ethernetHeader->srcAddress)));

	const int argc = 1;

	Local<Value> argv[argc] = {
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
