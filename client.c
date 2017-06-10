#include <stdio.h>
#include <event.h>

const char*     g_szServerIP = "139.224.239.147";
const short     g_nConnectPort = 9090;

int main(){
    int                     fd;
    struct sockaddr_in      addr;
    
    fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        printf("socket error..");
        return -1;
    }

    


}
