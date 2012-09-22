#include <server.h>
void Server::start() {
    int res;
    socklen_t server_len;
    struct sockaddr_in server_address;
    this->server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(DEFAULT_PORT);
    setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    res = listen(server_sockfd, 500);
    if(res == -1) exit(0);
}

int Server::getClient() {
    struct sockaddr_in client_address;
    int client_sockfd; 
    socklen_t client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
    std::cout << this->server_sockfd << std::endl;
    if(client_sockfd == -1) {perror("Accept"); exit(1);}
    return client_sockfd;
}
