#pragma once

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

#include <string>
#include "message.h"

using namespace std;

class Server {
public:
    Server(int port);
    ~Server();

    void run();

private:
    void create();
    void close_socket();
    void serve();
    void handle(int);
    string get_request(int);
    bool send_response(int, string);
    Message parse_request(string);
    void get_value(int, Message&);
    bool handle_message(int, Message&);
    string getCache();
    void setCache(string);

    int port_;
    int server_;
    int buflen_;
    char* buf_;
    string cache;
};
