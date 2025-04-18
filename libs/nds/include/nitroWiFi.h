#ifndef NITROWIFI_H
#define NITROWIFI_H

// === TODO (below here are placeholders)

#define SOC_PF_INET 0
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

typedef struct SOInAddr {
    int addr;
} SOInAddr;

struct SOSockAddrIn {
    int family;
    int port;
    SOInAddr addr;
};

struct SOSockAddr {
    int len;
};

struct SOHostEnt {
    const char* name;
    int aliases;
    int addrType;
    int length;
    u8** addrList;
};

typedef struct SOPollFD {
    int fd;
    int events;
    int revents;
} SOPollFD;

#endif /* NITROWIFI_H */
