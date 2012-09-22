#include <handler.h>
#include <domain.h>
#define LINE_LEN 2048
void Handler::handle() {
    int fd = this->conn->getClientFd();
    FILE *f = fdopen(fd, "r+");
    char line[LINE_LEN];
    char **params;
    int i = 0;
    int param_num = 0;
    Domain domain("Hello");
    while(fgets(line, LINE_LEN, f)) {
        if(line[0] == '*') {
            if(param_num != 0) {
                delete params;//memory leak
                i = 0;
            }
            param_num = atoi(line + 1);
            params = new char*[param_num];
        } 
        if(line[0] == '$') {
            size_t len = atol(line + 1);
            params[i] = new char[len];
            fread(params[i], len, 1, f);
            i++;
            fgets(line, LINE_LEN, f);
            if(i == param_num) {
                if(strncmp(params[0], "set", strlen("set")) == 0) {
                    domain.set(params[1], (const byte*)params[2], len + 1);
                    write(fd, "OK", strlen("OK") + 1);
                } else if(strncmp(params[0], "get", strlen("get")) == 0){
                    size_t length;
                    byte* data;
                    domain.get(params[1], data, length);
                    write(fd, data, length);
                } else {
                    write(fd, "OK", strlen("OK") + 1);
                }
            }
        }
    }
}
