#ifndef __DM_SERVER_H__
#define __DM_SERVER_H__
#include <distmem.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
class Server 
{
private:
    int server_sockfd;
public:
    Server(){}
    virtual ~Server(){}
    void start();
    int getClient();
};
#endif
