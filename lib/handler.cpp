#include <node.h>
#include "parser.h"

using namespace v8;

Handle<Value> parseHandler(const Arguments& args) {
    HandleScope scope;

	bool validArguments = 
		args[0]->IsString() &&
		args[1]->IsString() && 
		args[2]->IsNumber() && 
		args[3]->IsFunction();

	if (!validArguments) {
        return ThrowException(Exception::TypeError(
			String::New("Invalid arguments! Pass the arguments in the following order: {string} text, {string} title, {int} outputlength, {function} callback.")));
    }

	Local<Array> nodes = Parser(std::string(*String::Utf8Value(args[0]->ToString())),
								std::string(*String::Utf8Value(args[1]->ToString())), 
								int(args[2]->Int32Value()),
								false);

	const unsigned argc = 2;
	Local<Value> argv[argc] = {
		Local<Value>::New(Null()),
		nodes
	};

	Local<Function> callback = Local<Function>::Cast(args[3]);

	callback->Call(Context::GetCurrent()->Global(), argc, argv);

    return Undefined();
}

// Define the handlers
void RegisterModule(Handle<Object> target) {
    target->Set(String::NewSymbol("parse"),
		FunctionTemplate::New(parseHandler)->GetFunction());
}

NODE_MODULE(smartparser, RegisterModule);