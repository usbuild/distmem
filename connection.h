#ifndef __DM_CONNECTION_H__
#define __DM_CONNECTION_H__
#include <distmem.h>
class Connection 
{
public:
    int server_fd;
    int client_fd;
    string domain;
    Connection():domain(""){}
    void close(){
        ::close(client_fd);
    }
    int getClientFd() {
        return client_fd;
    }
};
#endif
