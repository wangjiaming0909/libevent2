#include <stdio.h>
#include <event.h>
#include <stdlib.h>

int main() {
	struct event_base *base = event_base_new();
	char *env = NULL;
	const char* libeventverion = NULL;
	ev_uint32_t vers;
	
	//get libevent version infomation
	libeventverion = event_get_version();
	vers = event_get_version_number();
	printf("libevent version infomation: %s\nversion number: %d\n", 
			libeventverion, vers);

	//get env
	env = getenv("PATH");

	printf("%s\n", env);
	printf("hello world\n");
	return 0;
}