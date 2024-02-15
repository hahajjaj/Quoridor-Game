#include "Client.hpp"

/**
 * @brief recois un message
 * 
 * @param buffer 
 * @param block est ce que la méthode est bloquante
 * @return int 0 = rien recu, -1 = socket close
 */
int Client::recvMessage(string &dest, bool block)
{
    if (!connected)
        return -1;
    timeOut.tv_sec = 0; timeOut.tv_usec = 0;
    FD_ZERO(&readFd);
    FD_SET(sock, &readFd);

    if (block || (checked(select(sock + 1, &readFd, nullptr, nullptr, &timeOut)) && FD_ISSET(sock, &readFd)))
    {
        char *buffer;
        size_t recv = receive(sock, (void**)&buffer, nullptr);
        if (recv == 0) // connection lost
        {
            disconnect();
            return -1;
        }
        dest = buffer;
        delete buffer;
        return recv;
    }
    return 0;
}

Client::Client(const char *domain, int port)
    : domain{strdup(domain)}, port{port}, timeOut{0, 0}
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, this->domain, &addr.sin_addr);
}

Client::~Client()
{
    stop();
    disconnect();
}

bool Client::connect()
{
    if (!connected)
        connected = ::connect(sock, (const sockaddr *)&addr, sizeof(addr)) == 0;
    return connected;
}

void Client::disconnect()
{
    if (!connected)
        return;
    ::close(sock);
    {
        lock_guard<mutex> lg{Mutex};
        connected = false;
    }
    cv.notify_one();
}

void Client::run(Interface *interface)
{
    if (!connect())
        return;
    status = true;
    recvThread = thread{[&] () -> void {
        
        while (status && connected)
        {
            string buffer = "";
            int recv = recvMessage(buffer);
            if (recv == -1)
            {
                break;
            }
            else if (recv > 0)
            {
                stackMsg.push_back(buffer);
            }
            this_thread::sleep_for(chrono::milliseconds(20));
        }
    }};
    interface->run();

    unique_lock<mutex> ul{Mutex};
    cv.wait(ul, [&] () -> bool {return !status || !connected; });
    ul.unlock();
    interface->stop();
    recvThread.join();
    if (status && !connected)
        cerr << "Le serveur s'est déconnecté\n";
    disconnect();
}

void Client::stop()
{
    disconnect();
    {
        lock_guard<mutex> lg{Mutex};
        status = false;
    }
    cv.notify_one();
}

void Client::sendMessage(const string &msg)
{
    if (connected)
        ssend(sock, msg.c_str(), msg.size() + 1);
}

string Client::getMessage(bool block)
{
    string buff{"0"};
    while (block && stackMsg.empty())
    {
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    if (!stackMsg.empty())
    {
        buff = stackMsg.front();
        stackMsg.pop_front();
    }
    return buff;
    // int recv = recvMessage(buff, block);
    // switch (recv)
    // {
    // case -1: // server lost
    //     stop();
    //     printf("Le serveur s'est déconnecté\n");
        
    // break;
    // }
    // return buff;
}