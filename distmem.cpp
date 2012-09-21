#include "distmem.h"
#include "domain.h"
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>

void handle_client_fd(int sockfd) {

    close(sockfd);
}
void start_server() {
    int res;
    socklen_t server_len;
    struct sockaddr_in server_address;
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(4327);
    res = setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    server_len = sizeof(server_address);
    res = bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    res = listen(server_sockfd, 500);
    struct sockaddr_in client_address;
    int client_sockfd; 
    socklen_t client_len = sizeof(client_address);
    for(; ;) {
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        handle_client_fd(client_sockfd);
    }
    
}
int main() {
    start_server();
    return 0;
}
