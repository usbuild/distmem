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
                if(strncmp(params[0], "use", strlen("use")) == 0){
                    string doname(string(params[1], len));
                    this->handleUse(doname);
                } else {
                    if(this->domain == NULL) {
                        write(fd, "Please use domain", strlen("Please use domain") + 1);
                        continue;
                    }
                    if(strncmp(params[0], "set", strlen("set")) == 0) {
                        this->handleSet(params[1], params[2], len + 1);
                    } else if(strncmp(params[0], "get", strlen("get")) == 0){
                        this->handleGet(params[1]);
                    } else {
                        write(fd, "OK", strlen("OK") + 1);
                    }
                }
            }
        }
    }
}

void Handler::handleSet(const char *key, const char *data, size_t length) {
        int fd = this->conn->getClientFd();
        this->domain->set(key, (const byte*)data, length);
        write(fd, "OK", strlen("OK") + 1);
}
void Handler::handleGet(const char *key) {
        int fd = this->conn->getClientFd();
        size_t length;
        byte* data;
        this->domain->get(key, data, length);
        if(length == 0){
            write(fd, " ", 1);
        } else {
            write(fd, data, length);
        }
}
void Handler::handleUse(string &doname) {
    int fd = this->conn->getClientFd();
    if(this->domain != NULL) {
        delete this->domain;
    }
    this->domain = new Domain(doname);
    write(fd, "OK", strlen("OK") + 1);
}
