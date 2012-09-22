#ifndef __DM_HANDLER_H__
#define __DM_HANDLER_H__
#include <distmem.h>
#include <connection.h>
class Handler 
{
private:
    Connection *conn;
public:
    Handler(Connection *conn):conn(conn){}
    void handle();
};
#endif
