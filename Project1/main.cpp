#include <iostream>
#include "duk_config.h"
#include "duktape.h"

int main() {
	duk_context *ctx = duk_create_heap_default();
	duk_eval_string(ctx, "print('Hello world!');");
	duk_destroy_heap(ctx);
}