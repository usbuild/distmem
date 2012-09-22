#include <handler.h>
void Handler::handle() {
    int fd = this->conn->getClientFd();
    FILE *f = fdopen(fd, "r+");
    char line[2048];
    while(fgets(line, 2048, f)) {
        std::cout << line << std::endl;
        write(fd, line, strlen(line) + 1);
    }
}
