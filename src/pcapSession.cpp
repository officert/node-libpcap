#include <nan.h>
#include <pcap.h>
// #include <net/ethernet.h>

#include "pcapSession.h"
#include "callbackInfo.h"

Nan::Persistent<v8::Function> PcapSession::constructor;

PcapSession::PcapSession() {
}

PcapSession::~PcapSession() {
}

NAN_MODULE_INIT(PcapSession::Init) {
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

	tpl->SetClassName(Nan::New("PcapSession").ToLocalChecked());

	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	//
	Nan::SetPrototypeMethod(tpl, "open", Open);

	constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());

	Nan::Set(target, Nan::New("PcapSession").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(PcapSession::New) {
	if (info.IsConstructCall()) {
		PcapSession *obj = new PcapSession();
		obj->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	} else {
		const int argc = 1;
		v8::Local<v8::Value> argv[argc] = {info[0]};
		v8::Local<v8::Function> cons = Nan::New(constructor);
		info.GetReturnValue().Set(cons->NewInstance(argc, argv));
	}
}

NAN_METHOD(PcapSession::Open) {
	// javascript function signature:
	//
	// /**
	//  *	@param {String} deviceName
	//  * @param {Object} options
	//  * @param {Number} options.packetCount - number of packets to capture
	//  * @param {Function} callback - callback function that gets called on every packet received
	//  */
	// function(deviceName, options) {}

	PcapSession* obj = Nan::ObjectWrap::Unwrap<PcapSession>(info.This());

	if (info.Length() == 0) {
		Nan::ThrowTypeError("Device name is required");
		return;
	}

	if (!info[0]->IsString()) {
		Nan::ThrowTypeError("Device name must be a string");
		return;
	}

	if (!info[1]->IsObject()) {
		Nan::ThrowTypeError("Options must be an object");
		return;
	}

	v8::Local<v8::Object> options = info[1]->ToObject();

	//packetCount

	// Nan::MaybeLocal<v8::Number> defaultPacketCount = Nan::MakeMaybe(Nan::New<v8::Number>(10));
	// Nan::MaybeLocal<v8::Value> packetCount = Nan::Get(options, Nan::New("packetCount").ToLocalChecked());
	//
	// v8::Local<v8::Number> numPackets = packetCount.FromMaybe(defaultPacketCount.ToLocalChecked());

	int numPackets = 10;

	char errorBuffer[PCAP_ERRBUF_SIZE];
	v8::String::Utf8Value str(info[0]);
	const char* deviceName = *str;

	//callback function for packets recieved
	CallbackInfo callbackInfo;
	callbackInfo.callback = info[2].As<v8::Function>();
	callbackInfo.deviceName = deviceName;

	bpf_u_int32 mask;      // The netmask of our sniffing device
	bpf_u_int32 net;      // The IP of our sniffing device

	PcapSession* session = ObjectWrap::Unwrap<PcapSession>(info.Holder());

	if (session == NULL)
	{
		Nan::ThrowTypeError("Error opening pcap session");
		return;
	}

	// get network number and mask associated with capture device
	if (pcap_lookupnet(deviceName, &net, &mask, errorBuffer) == -1)
	{
		printf("Couldn't get netmask for device %s: %s\n", deviceName, errorBuffer);

		net = 0;

		mask = 0;
	}

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

	packetCount++;

	CallbackInfo* callbackInfo = (CallbackInfo *)(args);

	unsigned long packetLength = header->caplen;

	char* packetData = new char[packetLength];

	memcpy(packetData, packet, packetLength);

	Nan::MaybeLocal<v8::Object> buffer = Nan::NewBuffer(packetData, packetLength);

	const int argc = 1;

	v8::Local<v8::Value> argv[argc] = {
		buffer.ToLocalChecked()
	};

	Nan::Callback callback(callbackInfo->callback);

	callback.Call(argc, argv);

	return;
}
