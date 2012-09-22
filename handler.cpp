#include <handler.h>
void Handler::handle() {
    int fd = this->conn->getClientFd();
    std::cout << "Hello" <<fd << std::endl;
}
