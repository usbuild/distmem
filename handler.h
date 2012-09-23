#ifndef __DM_HANDLER_H__
#define __DM_HANDLER_H__
#include <distmem.h>
#include <connection.h>
#include <domain.h>
class Handler 
{
private:
    Connection *conn;
    Domain *domain;
    void handleUse(string &doname);
    void handleSet(const char *key, const char *data, size_t length);
    void handleGet(const char *key);
public:
    Handler(Connection *conn):conn(conn), domain(NULL){}
    void handle();
};
#endif
