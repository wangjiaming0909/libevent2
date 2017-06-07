#include <stdio.h>
#include <event.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

void event_cb(evutil_socket_t fd, short what, void *args){
	printf("event_cb called..  %s\n", (char *)args);	
	
	char 				buf[64];
	const char *		readdr;
	evutil_socket_t 	clientfd;
	struct sockaddr_in 	clientaddr;
	socklen_t 			len = 0;

	len = sizeof(struct sockaddr_in);
	if((clientfd = accept(fd, (struct sockaddr *)&clientaddr, &len)) < 0){
		printf("accpet error..");
		return;
	}
	readdr = inet_ntop(AF_INET, &clientaddr.sin_addr, buf, INET_ADDRSTRLEN);
	printf("accepted a socket from: %s\n", readdr);
}

int main() {
	extern int				errno;
	int 					fd;
	struct sockaddr_in 		addr;
	int 					i = 0, err = 0;
	ev_uint32_t 			vers = 0;
	const char**			methods = NULL;
	struct event_base*		base = NULL;
	const char* 			libeventverion = NULL;
	struct event_config* 	evconf = NULL;
	struct event *			ev = NULL;
	struct timeval 			five_seconds = {5, 0};

	//socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == 0){
		printf("socket create error");
		return -1;
	}

	// struct sockaddr_in {
 	// 	__kernel_sa_family_t	sin_family;	/* Address family		*/
  	// 	__be16					sin_port;	/* Port number			*/
	// 	struct in_addr			sin_addr;	/* Internet address		*/

 	// 	 /* Pad to size of `struct sockaddr'. */
  	// 	unsigned char			__pad[__SOCK_SIZE__ - sizeof(short int) - sizeof(unsigned short int) - sizeof(struct in_addr)];
	// };

	// struct in_addr {
	// 	__be32	s_addr;//Internet address
	// };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = 0;//"set to 0" means to accept all ip addr? 
	addr.sin_port = htons(9999);//listen port
	if((err = bind(fd, (struct sockaddr *)&addr, sizeof(addr))) < 0){
		printf("bind error...%s\n", strerror(errno));
		return -2;
	}
	if((err = listen(fd, 10)) == -1){
		printf("listen error....");
		return -3;
	}
	

	//event
	evconf = event_config_new();
	event_config_set_flag(evconf, EVENT_BASE_FLAG_NOLOCK);
	base = event_base_new_with_config(evconf);
	event_config_free(evconf);
	event_base_priority_init(base, 10);
	methods = event_get_supported_methods();//epoll, poll select
	//get libevent version infomation
	libeventverion = event_get_version();
	vers = event_get_version_number();
	ev = event_new(base, fd, EV_TIMEOUT|EV_READ, event_cb, (char *)"asd");
	if(NULL == ev){
		printf("event create failed..");
		return -3;
	}
	event_add(ev, &five_seconds);
	event_base_dispatch(base);

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