#include <distmem.h>
#include <domain.h>
#include <server.h>
#include <connection.h>
#include <handler.h>

int main() {
    Server server;
    server.start();
    Connection *conn;
    for( ; ; ) {
        conn = server.getConnection();
        if(conn == NULL) continue;
        Handler handler(conn);
        handler.handle();
        conn->close();
        delete conn;
    }
    return 0;
}
