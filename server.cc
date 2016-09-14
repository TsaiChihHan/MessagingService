#include "server.h"

Server::Server(int port) {
    // setup variables
    port_ = port;
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
}

Server::~Server() {
    delete buf_;
}

void
Server::run() {
    // create and run the server
    create();
    serve();
}

void
Server::create() {
    struct sockaddr_in server_addr;

    // setup socket address structure
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // create socket
    server_ = socket(PF_INET,SOCK_STREAM,0);
    if (!server_) {
        perror("socket");
        exit(-1);
    }

    // set socket to immediately reuse port when the application closes
    int reuse = 1;
    if (setsockopt(server_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        exit(-1);
    }

    // call bind to associate the socket with our local address and
    // port
    if (bind(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("bind");
        exit(-1);
    }

    // convert the socket to listen for incoming connections
    if (listen(server_,SOMAXCONN) < 0) {
        perror("listen");
        exit(-1);
    }
}

void
Server::close_socket() {
    close(server_);
}

void
Server::serve() {
    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

      // accept clients
    while ((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) {

        handle(client);
    }
    close_socket();
}

void
Server::handle(int client) {
    // loop to handle all requests
    while (1) {
        // get a request
        string request = get_request(client);
        // break if client is done or an error occurred
        if (request.empty())
          break;
            //break;
        // parse request
        Message message = parse_request(request);
        // get more characters if needed
        if (message.needed())
            get_value(client,message);
        // do something
        bool success = handle_message(client,message);
        // break if an error occurred
        if (not success)
            break;
    }
    close(client);
}

Message
Server::parse_request(string request) {
    std::stringstream ss;
    ss << request;
    string command;
    string name;
    int length;
    string value;
    ss >> command >> name >> length;
    Message message(command, name, length);
    // message.toString();
    return message;
}

void
Server::get_value(int client, Message& message) {
  string request = "";
  // read until we get a newline
  // cout << "message.getValue().size() = " << message.getValue().size() << endl;
  // cout << "message.getLength() = " << message.getLength() << endl;
  // cout << "getCache().size() = " << getCache().size() << endl;
  // cout << "cache = " << getCache() << endl;
  while (getCache().size() <= message.getLength()) {
      request = "";
      memset(buf_,0,buflen_);
      int nread = recv(client,buf_,1024,0);
      // cout << "Received message from client: " << buf_ << endl;
      // cout << "nread: " << nread << endl;
      if (nread < 0) {
          if (errno == EINTR)
              // the socket call was interrupted -- try again
              continue;
          else
              // an error occurred, so break out
              break;
      } else if (nread == 0) {
          // the socket is closed
          break;
      }
      // be sure to use append in case we have binary data
      request.append(buf_,nread);
      // cout << "request = " << request << endl;
      // cout << "request.size() = " << request.size() << endl;
      setCache(getCache() + request);
      // cout << "cache: " << getCache() << endl;
      // cout << "cache.size() = " << getCache().size() << endl;
  }
  message.setValue(cache.substr(0, message.getLength()));
  setCache(getCache().substr(message.getLength()));
  // cout << "message.getValue().size() = " << message.getValue().size() << endl;
  // cout << "message.getLength() = " << message.getLength() << endl;
  // cout << "getCache().size() = " << getCache().size() << endl;
  // message.toString();
}

bool
Server::handle_message(int client, Message& message) {
  std::stringstream ss;
  ss << message.getLength();
  string len = ss.str();
  std::string response = "Stored a file called " + message.getName() + " with " + len + " bytes.\n";
  const char* ptr = response.c_str();
  cout << response;
  return send(client, ptr, response.size(), 0);
}

string
Server::get_request(int client) {
    string request = getCache();
    // read until we get a newline
    while (request.find("\n") == string::npos) {
        memset(buf_,0,buflen_);
        int nread = recv(client,buf_,1024,0);
        // cout << "Received a request from client: " << buf_ << endl;
        // cout << "nread = " << nread << endl;
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        request.append(buf_,nread);
    }
    // a better server would cut off anything after the newline and
    // save it in a cache
    size_t pos = request.find("\n");
    // cout << "position: " << pos << endl;
    string cache = request.substr(pos + 1);
    // cout << "cache: " << cache << endl;
    setCache(cache);
    return request.substr(0,pos);
}

bool
Server::send_response(int client, string response) {
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}

string
Server::getCache() {
  return cache;
}

void
Server::setCache(string cache) {
  this->cache = cache;
}
/*
get_request()
  request cache

get_value
*/
