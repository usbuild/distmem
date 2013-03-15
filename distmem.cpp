#include <distmem.h>
#include <domain.h>
#include <server.h>
#include <connection.h>
#include <handler.h>
#include <unistd.h>
#include <signal.h>
//#define DEBUG

int main() {

#ifndef DEBUG
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    if(fork() != 0) exit(0);
    if(setsid() < 0) exit(0);
    if(fork() != 0) exit(0);
    int fd, fdtablesize;
    for (fd = 0, fdtablesize = getdtablesize(); fd < fdtablesize; fd++)
        close(fd);
    umask(0);
#endif


    Server server;
    server.start();
    Connection *conn;
    signal(SIGCHLD, SIG_IGN);
    for( ; ; ) {
        conn = server.getConnection();
        pid_t pid = fork();
        if(pid != 0) continue;
        if(conn == NULL) continue;
        Handler handler(conn);
        handler.handle();
        conn->close();
        delete conn;
    }
    return 0;
}
