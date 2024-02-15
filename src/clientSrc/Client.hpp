#ifndef __CLIENT_H__
#define __CLIENT_H__


#include "../common/common.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stack>
#include <thread>
#include <condition_variable>
#include <list>

class Interface;

class Client : public ServeurClient
{
private:
    const char *domain;
    int port;

    int sock;
    struct sockaddr_in addr;
    struct timeval timeOut;
    fd_set readFd;

    bool connected = false, status = false;
    
    mutex Mutex;
    condition_variable cv;
    thread recvThread;
    list<string> stackMsg;
    
    int recvMessage(string &dest, bool block = false);
    
public:
    Client(const char *domain, int port);
    virtual ~Client();

    virtual bool connect();
    virtual void disconnect();

    virtual void run(Interface *interface);
    virtual void stop();
    
    virtual void sendMessage(const string &msg);
    virtual string getMessage(bool block = true);
};

#include "Interface.hpp"
#endif // __CLIENT_H__