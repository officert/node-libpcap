#ifndef CALLBACK_INFO
#define CALLBACK_INFO

#include <node.h>
#include <string>

struct CallbackInfo {
	v8::Local<v8::Function> callback;
	v8::Isolate* isolate;
	std::string deviceName;
};

#endif
