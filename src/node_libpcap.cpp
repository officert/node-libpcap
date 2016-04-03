#include <node.h>
#include <pcap.h>

namespace node_libpcap {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

//function prototypes
pcap_t* create_session(const char *device);

void Create_Session(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

  // pcap_t* session = create_session("en0");

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void init(Local<Object> exports) {
	NODE_SET_METHOD(exports, "createSession", Create_Session);
}

NODE_MODULE(addon, init)

}

/* --------------------------- */
pcap_t* create_session(const char *device)
{
	pcap_t* session;
	char error_buffer[PCAP_ERRBUF_SIZE];
	const int packet_capture_length = 65536;

	//struct bpf_program fp; // The compiled filter expression
	//char filter_exp[] = "ip"; // The filter expression

	// bpf_u_int32 mask; // The netmask of our sniffing device
	// bpf_u_int32 net; // The IP of our sniffing device

	// char *device_name = get_device_name(device);

	//
	// if (device_name == NULL)
	// {
	//      throw "Error, could not get device name";
	// }
	//
	// // get network number and mask associated with capture device
	// if (pcap_lookupnet(device, &net, &mask, error_buffer) == -1)
	// {
	//      printf("Couldn't get netmask for device %s: %s\n", device, error_buffer);
	//
	//      net = 0;
	//
	//      mask = 0;
	// }
	//
	// printf("Device: %s\n", device_name);
	// printf("Filter expression: %s\n", filter_exp);
	// printf("Net: %u\n", net);
	// printf("Mask: %u\n", mask);

	session = pcap_open_live(device, packet_capture_length, 1, 1000, error_buffer);
	//
	// if (session == NULL)
	// {
	//      printf("Error Buffer: %s\n", error_buffer);
	//
	//      throw "Error opening session to device";
	// }
	//
	// if (pcap_datalink(session) != DLT_EN10MB) {
	//      fprintf(stderr, "%s is not an Ethernet\n", device);
	//      throw "Device is not an Ethernet device";
	// }

	//
	// //create and compile a filter for only packets on port 23
	// if (pcap_compile(session, &fp, filter_exp, 0, net) == -1)
	// {
	//      fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(session));
	//
	//      throw "Couldn't parse filter";
	// }
	//
	// //set the filter to actually apply it to our current session
	// if (pcap_setfilter(session, &fp) == -1)
	// {
	//      fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(session));
	//
	//      throw "Couldn't install filter";
	// }

	return session;
}
