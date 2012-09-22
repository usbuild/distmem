#include <distmem.h>
#include <domain.h>
#include <server.h>

int main() {
    Server server;
    server.start();
    int client_fd;
    while(client_fd = server.getClient()) {
        std::cout << "client_fd:" << client_fd << std::endl;
    }
    return 0;
}
