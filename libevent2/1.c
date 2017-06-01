#include <stdio.h>
#ifndef  WIN32
#define WIN32
#include <event2\event.h>

int main() {
	struct event_base *base = event_base_new();
	
	printf("hello world\n");
	return 0;
}
#endif //  WIN32