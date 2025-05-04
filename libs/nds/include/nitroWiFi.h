#ifndef NITROWIFI_H
#define NITROWIFI_H

#include "nitro.h"

typedef struct SOHostEnt {
    u8 _pad[8];
    /* 0x08 */ const char* name;
    /* 0x0C */ char** aliases;
    /* 0x10 */ u16 addrType;
    /* 0x12 */ u16 length;
    /* 0x14 */ u8** addrList;
} SOHostEnt;

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

#define SOC_POLLRDNORM 0x01
#define SOC_POLLWRNORM 0x08
#define SOC_POLLERR 0x20
#define SOC_POLLHUP 0x40

#define SOC_EWOULDBLOCK -0x06
#define SOC_ECONNRESET -0x0F
#define SOC_EINPROGRESS -0x1A
#define SOC_EMSGSIZE -0x23
#define SOC_ENOBUFS -0x2A
#define SOC_ETIMEDOUT -0x4C

inline u32 SOC_HtoNl(u32 val) {
    return ((val >> 0x18) & 0x000000FF) | ((val >> 0x08) & 0x0000FF00) |
           ((val << 0x08) & 0x00FF0000) | ((val << 0x18) & 0xFF000000);
}

inline u32 SOC_NtoHl(u32 val) {
    return ((val >> 0x18) & 0x000000FF) | ((val >> 0x08) & 0x0000FF00) |
           ((val << 0x08) & 0x00FF0000) | ((val << 0x18) & 0xFF000000);
};

inline u32 SOC_HtoNs(u16 val) {
    return ((val >> 0x08) & 0x000000FF) | ((val << 0x08) & 0x0000FF00);
}

inline u32 SOC_NtoHs(u16 val) {
    return ((val >> 0x08) & 0x000000FF) | ((val << 0x08) & 0x0000FF00);
}

typedef struct SOSockAddr {
    int len;
} SOSockAddr;

typedef struct SOPollFD {
    int fd;
    int events;
    int revents;
} SOPollFD;

char* SOC_InetNtoA(SOInAddr);
int SOC_Bind(int sock, const SOSockAddr* addr);
int SOC_Socket(int pf, int type, int protocol);
int SOC_Close(int sock);
int SOC_Shutdown(int sock, int how);
int SOC_Connect(int sock, SOSockAddr* addr);
int SOC_Listen(int sock, int backlog);
int SOC_Accept(int sock, SOSockAddr* addr);
int SOC_Recv(int sock, char* buf, int len, int flags);
int SOC_RecvFrom(int sock, char* buf, int len, int flags, SOSockAddr* addr);
int SOC_Send(int sock, const char* buf, int len, int flags);
int SOC_SendTo(int sock, const char* buf, int len, int flags, SOSockAddr* addr);
int SOC_GetSockOpt(int sock, int level, int optname, char* optval, int* optlen);
int SOC_SetSockOpt(int sock, int level, int optname, const char* optval, int optlen);
int SOC_InetAtoN(const char* name, SOInAddr* addr);
int SOC_GetSockName(int sock, SOSockAddr* addr);
int SOC_Fcntl(int sock, int, int);
int SOC_Poll(SOPollFD* pollFD, int, int);
SOHostEnt* SOC_GetHostByName(const char* host);
SOHostEnt* SOC_GetHostByAddr(const char*, int, int);

// === TODO (below here are placeholders)

#define SOC_EHOSTUNREACH 0
#define SOC_F_GETFL 0
#define SOC_SOL_SOCKET 0
#define SOC_SO_SNDBUF 0
#define SOC_SO_RCVBUF 0
#define SOC_O_NONBLOCK 0
#define SOC_F_SETFL 0
#define SOC_SOCK_DGRAM 0
#define SOC_SOCK_STREAM 0
#define SOC_INADDR_ANY 0

void IP_GetAddr(void*, u8* ip_out);
void IP_GetBroadcastAddr(void*, u8* ip_out);
void IP_GetMacAddr(void*, u8* mac_out);

#endif /* NITROWIFI_H */
