#include <distmem.h>
#include <domain.h>
#include <server.h>
#include <connection.h>
#include <handler.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

static void *
thread_start(void *arg) {
    Connection *conn = (Connection*)arg;
    Handler(conn).handle();
    conn->close();
    delete conn;
}

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

    pthread_t p;
    for( ; ; ) {
        conn = server.getConnection();
        pthread_create(&p, NULL, thread_start, conn);
    }
    return 0;
}
