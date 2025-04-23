#ifndef NITROWIFI_H
#define NITROWIFI_H

#include "nitro.h"

struct SOHostEnt {
    u8 _pad[8];
    /* 0x08 */ const char* name;
    /* 0x0C */ char** aliases;
    /* 0x10 */ u16 addrType;
    /* 0x12 */ u16 length;
    /* 0x14 */ u8** addrList;
};

typedef struct SOInAddr {
    /* 0x00 */ int addr;
} SOInAddr;

struct SOSockAddrIn {
    u8 _pad;
    /* 0x01 */ u8 family;
    /* 0x02 */ u16 port;
    /* 0x04 */ SOInAddr addr;
};

#define SOC_PF_INET 2

inline u32 SOC_HtoNl(u32 hval) {
    return ((hval >> 0x18) & 0x000000FF) | ((hval >> 0x08) & 0x0000FF00) |
           ((hval << 0x08) & 0x00FF0000) | ((hval << 0x18) & 0xFF000000);
}

inline u32 SOC_HtoNs(u16 hval) {
    return ((hval >> 0x08) & 0x000000FF) | ((hval << 0x08) & 0x0000FF00);
}

// === TODO (below here are placeholders)

#define SOC_F_GETFL 0
#define SOC_SOL_SOCKET 0
#define SOC_SO_SNDBUF 0
#define SOC_SO_RCVBUF 0
#define SOC_O_NONBLOCK 0
#define SOC_F_SETFL 0
#define SOC_POLLRDNORM 0
#define SOC_POLLWRNORM 0
#define SOC_POLLHUP 0
#define SOC_POLLERR 0
#define SOC_SOCK_DGRAM 0
#define SOC_SOCK_STREAM 0
#define SOC_INADDR_ANY 0
#define SOC_ECONNRESET 0
#define SOC_EHOSTUNREACH 0
#define SOC_ENOBUFS 0
#define SOC_EMSGSIZE 0
#define SOC_EWOULDBLOCK 0
#define SOC_EINPROGRESS 0
#define SOC_ETIMEDOUT 0

struct SOSockAddr {
    int len;
};

typedef struct SOPollFD {
    int fd;
    int events;
    int revents;
} SOPollFD;

char* SOC_InetNtoA(SOInAddr);
int SOC_Bind(int sock, const struct SOSockAddr* addr);

#endif /* NITROWIFI_H */
