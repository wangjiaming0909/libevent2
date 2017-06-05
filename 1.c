#include <stdio.h>
#include <event.h>
#include <stdlib.h>

int main() {
	struct event_base *base = event_base_new();
	
	//get env
	char *env = NULL;
	env = getenv("PATH");

	printf("%s\n", env);
	printf("hello world\n");
	return 0;
}