#ifndef CALLBACK_INFO
#define CALLBACK_INFO

#include <node.h>

struct CallbackInfo {
	v8::Local<v8::Function> callback;
	v8::Isolate* isolate;
};

#endif
