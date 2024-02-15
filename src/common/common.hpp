#ifndef __COMMON_HPP
#define __COMMON_HPP
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

#include <cstring>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <memory>
#include <mutex>
#include <locale.h>
#include <wchar.h>

using namespace std;

template <typename T = int>
struct Point
{
    T x = 0;
    T y = 0;
};

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <chrono>

int _checked(int ret, const char *calling_function);

// The macro allows us to retrieve the name of the calling function
#define checked(call) _checked(call, #call)
struct ServeurClient
{
    /**
         * @brief Send data under the form <size_t len><...data>
         * Function name is 'ssend' instead of 'send' because the latter already exists.
         */
    void ssend(int sock, const void *data, size_t len)
    {
        time_t t = time(0);

        checked(write(sock, &len, sizeof(len)));
        checked(write(sock, &t, sizeof(t)));
        checked(write(sock, data, len));
#ifdef __SERVEUR_HPP
        printf("Message send to %d (%zd bytes): '%s'\n", sock, len, (const char *)data);
#endif
    }

    size_t receive(int sock, void **dest, time_t *timestamp)
    {
        size_t nbytes_to_receive;
        if (checked(read(sock, &nbytes_to_receive, sizeof(nbytes_to_receive))) == 0)
        {
            // Connection closed
            return 0;
        }
        unsigned char *buffer = (unsigned char *)malloc(nbytes_to_receive);
        if (buffer == NULL)
        {
            fprintf(stderr, "malloc could not allocate %zd bytes", nbytes_to_receive);
            perror("");
            exit(1);
        }
        memset(buffer, 0, nbytes_to_receive);
        time_t t;
        checked(read(sock, &t, sizeof(t))); //read time
        if (timestamp != NULL)
            *timestamp = t;
        size_t total_received = 0;
        size_t received = 1;
        while (nbytes_to_receive > 0 && received > 0)
        {
            received = checked(read(sock, &buffer[total_received], nbytes_to_receive));
            total_received += received;
            nbytes_to_receive -= received;
        }
        *dest = buffer;
#ifdef __SERVEUR_HPP
        printf("Message received from %d (%zd bytes): '%s'\n", sock, total_received, (const char *)buffer);
#endif
        return total_received;
    }
};

template <typename... Args>
string format(const char *frmt, Args... args)
{
    int size = snprintf(nullptr, 0, frmt, args...);
    char *buffer = (char *)malloc(size + 1);
    memset(buffer, 0, size + 1);
    if (!buffer)
    {
        fprintf(stderr, "malloc could not allocate %d bytes", size + 1);
        perror("");
        exit(EXIT_FAILURE);
    }

    if (snprintf(buffer, size + 1, frmt, args...) != size)
    {
        free(buffer);
        return "";
    }
    string ret{buffer};
    free(buffer);
    return ret;
}

void replace(string &str, const char old, const char nw);

int len(string);
vector<string> split(string, char);
string vectToStr(vector<string>);


#endif
