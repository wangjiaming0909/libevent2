#include <stdio.h>
#include <event.h>
#include <stdlib.h>

int main() {
	int 					i = 0;
	ev_uint32_t 			vers = 0;
	const char**			methods = NULL;
	struct event_base*		base = NULL;
	const char* 			libeventverion = NULL;
	struct event_config* 	evconf = NULL;

	evconf = event_config_new();
	event_config_set_flag(evconf, EVENT_BASE_FLAG_NOLOCK);
	base = event_base_new_with_config(evconf);
	event_config_free(evconf);
	event_base_priority_init(base, 10);
	methods = event_get_supported_methods();//epoll, poll select
	//get libevent version infomation
	libeventverion = event_get_version();
	vers = event_get_version_number();

	while(methods[i]){
		printf("%s\n", methods[i]);
		i++;
	}
	//EVENT_MAX_PRIORITIES
	printf("%d\n", event_base_get_npriorities(base));
	printf("%d\n", event_base_get_npriorities(base));

	printf("libevent version infomation: %s\nversion number: %d\n", 
			libeventverion, vers);
	printf("hello world\n");
	return 0;
}